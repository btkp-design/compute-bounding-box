// #include <boost/multiprecision/gmp.hpp>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/IO/OBJ.h>
#include <CGAL/IO/STL.h>
#include <CGAL/optimal_bounding_box.h>
#include <CGAL/Optimal_bounding_box/Oriented_bounding_box_traits_3.h>
#include <CGAL/Polygon_mesh_processing/IO/polygon_mesh_io.h>
#include <CGAL/Polygon_mesh_processing/triangulate_faces.h>
#include <CGAL/random_convex_set_2.h>
#include <CGAL/Surface_mesh.h>
#include <iostream>
#include <fstream>
#include <cstring>
#include <filesystem>
#include <vector>
#include <sstream>
#include <string>

using K = CGAL::Exact_predicates_inexact_constructions_kernel;
typedef CGAL::Surface_mesh<K::Point_3> Surface_mesh;
typedef K::Point_3 Point_3;
typedef K::Plane_3 Plane_3;
namespace PMP = CGAL::Polygon_mesh_processing;

std::string& ltrim(std::string& s, const char* t = " \t\n\r\f\v")
{
    s.erase(0, s.find_first_not_of(t));
    return s;
}

void printUsage() {
    std::cerr << "Usage: ./compute-bounding-box.exe <input.obj> <output.obj> <output.csv>" << std::endl;
    std::cerr << "  <input.obj>   - Full path and filename of the input OBJ file" << std::endl;
    std::cerr << "  <output.obj>  - Full path and filename for the output OBJ file" << std::endl;
    std::cerr << "  <output.csv>  - Full path and filename for the output CSV file" << std::endl;
}

int main(int argc, char** argv)
{
    if (argc != 4) {
        std::cerr << "Error: Wrong number of arguments." << std::endl;
        printUsage();
        return 1;
    }

    const std::string input_path  = argv[1];
    const std::string output_path_obj = argv[2];
    const std::string output_path_csv = argv[3];

    // Validate that input path is not empty
    if (input_path.empty()) {
        std::cerr << "Error: Input file path cannot be empty." << std::endl;
        printUsage();
        return 1;
    }

    // Validate that output paths are not empty
    if (output_path_obj.empty() || output_path_csv.empty()) {
        std::cerr << "Error: Output file paths cannot be empty." << std::endl;
        printUsage();
        return 1;
    }

    // Ensure output directories exist
    auto obj_parent = std::filesystem::path(output_path_obj).parent_path();
    auto csv_parent = std::filesystem::path(output_path_csv).parent_path();

    if (!obj_parent.empty() && !std::filesystem::exists(obj_parent)) {
        std::cerr << "Error: Output OBJ directory does not exist: " << obj_parent << std::endl;
        return 1;
    }
    if (!csv_parent.empty() && !std::filesystem::exists(csv_parent)) {
        std::cerr << "Error: Output CSV directory does not exist: " << csv_parent << std::endl;
        return 1;
    }

    std::vector<K::Point_3> points_ref;
    std::vector<std::vector<std::size_t>> faces_ref;

    if (!CGAL::IO::read_OBJ(input_path.c_str(), points_ref, faces_ref)) {
        std::cerr << "Error: Could not read OBJ file: " << input_path << std::endl;
        return 1;
    }

    std::array<K::Point_3, 8> obb_points;
    CGAL::oriented_bounding_box(points_ref, obb_points);

    // Print bounding box points to terminal
    for (int i = 0; i < 8; i++) {
        std::cout << "Point " << i << ": " << obb_points[i] << std::endl;
    }

    // Construct and write OBB mesh
    Surface_mesh obb_sm;
    CGAL::make_hexahedron(obb_points[0], obb_points[1], obb_points[2], obb_points[3],
                          obb_points[4], obb_points[5], obb_points[6], obb_points[7], obb_sm);
    PMP::triangulate_faces(obb_sm);

    if (!CGAL::IO::write_OBJ(output_path_obj, obb_sm)) {
        std::cerr << "Error: Could not write OBJ file: " << output_path_obj << std::endl;
        return 1;
    }
    std::cout << "OBJ written to: " << output_path_obj << std::endl;

    // Write OBB points to CSV
    try {
        std::ofstream myFile(output_path_csv);
        if (!myFile.is_open()) {
            std::cerr << "Error: Could not open CSV file for writing: " << output_path_csv << std::endl;
            return 1;
        }
        myFile << obb_points[0] << "," << obb_points[1] << "," << obb_points[2] << "," << obb_points[3]
               << "," << obb_points[4] << "," << obb_points[5] << "," << obb_points[6] << "," << obb_points[7];
        myFile.close();
        std::cout << "CSV written to: " << output_path_csv << std::endl;
    }
    catch (const std::exception& e) {
        std::cerr << "Error writing CSV: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
