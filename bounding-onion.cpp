#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/IO/OBJ.h>
#include <CGAL/optimal_bounding_box.h>
#include <CGAL/Optimal_bounding_box/Oriented_bounding_box_traits_3.h>
#include <CGAL/Polygon_mesh_processing/IO/polygon_mesh_io.h>
#include <CGAL/Polygon_mesh_processing/triangulate_faces.h>
#include <CGAL/random_convex_set_2.h>
#include <CGAL/Surface_mesh.h>
#include <iostream>

using K = CGAL::Exact_predicates_inexact_constructions_kernel;
typedef CGAL::Surface_mesh<K::Point_3> Surface_mesh;
namespace PMP = CGAL::Polygon_mesh_processing;
int main(int argc, char** argv)
{
    if (argc < 2) {
        std::cerr << "usage: bounding-onion <input.obj> [<output.obj>]" << std::endl;
        return 1;
    }

    const char* input_path = argv[1];
    char* output_path = NULL;
    if (argc == 3) {
        output_path = argv[2];
    }

    std::vector<K::Point_3> points_ref;
    std::vector<std::vector<std::size_t> > faces_ref;
    if (!CGAL::IO::read_OBJ(input_path, points_ref, faces_ref)) {
        std::cerr << "error reading OBJ" << std::endl;
        return 1;
    }

    std::array<K::Point_3, 8> obb_points;
    CGAL::oriented_bounding_box(points_ref, obb_points);

    for(int i = 0; i < 8; i += 1) {
        std::cout << obb_points[i] << std::endl;
    }

    if (output_path) {
        Surface_mesh obb_sm;
        CGAL::make_hexahedron(obb_points[0], obb_points[1], obb_points[2], obb_points[3],
                                obb_points[4], obb_points[5], obb_points[6], obb_points[7], obb_sm);
        PMP::triangulate_faces(obb_sm);
        CGAL::IO::write_OBJ(output_path, obb_sm);
    }

    return 0;
}
