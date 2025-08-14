#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Surface_mesh.h>
#include <CGAL/Polyhedron_3.h>
#include <CGAL/Nef_polyhedron_3.h>
#include <CGAL/IO/OBJ.h>
#include <fstream>
#include <iostream>
#include <CGAL/Polygon_mesh_processing/self_intersections.h>
#include <CGAL/Polygon_mesh_processing/repair.h>
#include <CGAL/boost/graph/graph_traits_Polyhedron_3.h>
#include <CGAL/boost/graph/helpers.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel     Kernel;
typedef CGAL::Surface_mesh<Kernel::Point_3>                     SurfaceMesh;
typedef CGAL::Polyhedron_3<Kernel>                              Polyhedron;
typedef CGAL::Nef_polyhedron_3<Kernel>                          NefPolyhedron;

namespace PMP = CGAL::Polygon_mesh_processing;

bool load_obj_as_polyhedron(const std::string& path, Polyhedron& poly)
{
    std::ifstream input(path);
    if (!input) {
        std::cerr << "Failed to open OBJ file: " << path << std::endl;
        return false;
    }
    if (!CGAL::IO::read_OBJ(input, poly)) {
        std::cerr << "Failed to read OBJ data from: " << path << std::endl;
        return false;
    }

    std::cout << "Successfully loaded " << path << std::endl;
    return true;
}

bool save_polyhedron_to_obj(const Polyhedron& poly, const std::string& output_path)
{
    std::ofstream output(output_path);
    if (!output) {
        std::cerr << "Failed to open output file: " << output_path << std::endl;
        return false;
    }
    if (!CGAL::IO::write_OBJ(output, poly)) {
        std::cerr << "Failed to write OBJ file: " << output_path << std::endl;
        return false;
    }
    return true;
}
bool check_polyhedron_validity(const Polyhedron& poly, const std::string& label)
{
    bool valid = true;

    // Check if empty
    if (poly.empty()) {
        std::cerr << label << " is empty." << std::endl;
        valid = false;
    }

    // Check if manifold
    if (!CGAL::is_valid(poly)) {
        std::cerr << label << " is not a valid manifold." << std::endl;
        valid = false;
    }

    // Check if closed
    if (!poly.is_closed()) {
        std::cerr << label << " is not closed (has holes)." << std::endl;
        valid = false;
    }

    // Check for self-intersections
    std::vector<std::pair<Polyhedron::Facet_const_handle, Polyhedron::Facet_const_handle>> self_intersections;
    if (PMP::does_self_intersect(poly, std::back_inserter(self_intersections))) {
        std::cerr << label << " has " << self_intersections.size() << " self-intersection(s)." << std::endl;
        valid = false;
    }

    return valid;
}


int main(int argc, char** argv)
{
    if (argc != 4) {
        std::cerr << "Usage: " << argv[0] << " <obj1> <obj2> <output.obj>" << std::endl;
        return 1;
    }

    std::string obj1_path = argv[1];
    std::string obj2_path = argv[2];
    std::string output_path = argv[3];

    Polyhedron poly1, poly2;

        if (!load_obj_as_polyhedron(obj1_path, poly1) || !load_obj_as_polyhedron(obj2_path, poly2))
        return 1;

    bool valid1 = check_polyhedron_validity(poly1, "Input 1");
    bool valid2 = check_polyhedron_validity(poly2, "Input 2");

    // Optionally attempt repair
    if (!poly1.is_closed()) {
        std::cerr << "Input 1 is not closed. Attempting to close holes..." << std::endl;
        PMP::close_holes(poly1);
    }
    if (!poly2.is_closed()) {
        std::cerr << "Input 2 is not closed. Attempting to close holes..." << std::endl;
        PMP::close_holes(poly2);
    }

    // Re-check after repair attempt
    valid1 = check_polyhedron_validity(poly1, "Input 1 (after repair)");
    valid2 = check_polyhedron_validity(poly2, "Input 2 (after repair)");

    if (!valid1 || !valid2) {
        std::cerr << "Aborting due to unrepaired mesh issues." << std::endl;
        return 1;
    }

    // Convert to Nef
    NefPolyhedron nef1(poly1);
    NefPolyhedron nef2(poly2);

    // Boolean Intersection
    NefPolyhedron nef_result = nef1.intersection(nef2);

    // Convert back to Polyhedron
    if (nef_result.is_simple()) {
        Polyhedron result_poly;
        nef_result.convert_to_polyhedron(result_poly);

        if (!save_polyhedron_to_obj(result_poly, output_path))
            return 1;

        std::cout << "Intersection saved to " << output_path << std::endl;
    } else {
        std::cerr << "Resulting Nef polyhedron is not simple and cannot be converted to a mesh." << std::endl;
        return 1;
    }

    return 0;
}
