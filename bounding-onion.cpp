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
//#include "csv.h"
#include <filesystem>
#include <vector>
#include <sstream>
#include <string>

using K = CGAL::Exact_predicates_inexact_constructions_kernel;
typedef CGAL::Surface_mesh<K::Point_3> Surface_mesh;
typedef K::Point_3 Point_3;
typedef K::Plane_3 Plane_3;

namespace PMP = CGAL::Polygon_mesh_processing;
// argc = number of args passed to script,
// argv = array of strings (char*'s) representing arguments to script

std::string& ltrim(std::string& s, const char* t = " \t\n\r\f\v")
{
    s.erase(0, s.find_first_not_of(t));
    return s;
}

int main(int argc, char** argv) 
{
    if (argc < 2) {
        std::cerr << "Please use: ./compute-bounding-box.exe <input.obj> <output_filename>" << std::endl;
        return 1;
    }

    const char* input_path = argv[1]; //Filename prepends to argv[0], hence why we use argv[1]

    std::string output_path = "";
    std::string output_path_obj = "";
    std::string output_path_stl = "";
    std::string output_path_csv = "";

    if (argc == 3) {    //Check to make sure the right number of arguments were passed
        output_path = argv[2];
        output_path_obj = output_path + "_OBB.obj";
        output_path_stl = output_path + "_OBB.stl";
        output_path_csv = output_path + "_OBB.csv";

        if (output_path.empty()){   //Check to make sure there's a legitimate output specified
            std::cerr << "You didn't specify an output filename. Please use: ./compute-bounding-box.exe <input.obj> <output_filename>" << std::endl;
            return 1;
        }
    } else {
        std::cerr << "Wrong number of arguments. Please use: ./compute-bounding-box.exe <input.obj> <output_filename>" << std::endl;
        return 1;
    }

    std::vector<K::Point_3> points_ref;     //Initialize vector of 3D points to store obj points
    std::vector<std::vector<std::size_t> > faces_ref;   //Initialize vector to store obj faces

    //Read the obj file, output error if no path
    if (!CGAL::IO::read_OBJ(input_path, points_ref, faces_ref)) {
        std::cerr << "error reading OBJ" << std::endl;
        return 1;
    }

    std::array<K::Point_3, 8> obb_points;       //Create array for storing bounding box points
    CGAL::oriented_bounding_box(points_ref, obb_points);    //create OBB

    // This just lists off all the points in the terminal
    for(int i = 0; i < 8; i += 1) {
        std::cout << obb_points[i] << std::endl;
    }

    // Construct OBB
    Surface_mesh obb_sm;
    CGAL::make_hexahedron(obb_points[0], obb_points[1], obb_points[2], obb_points[3],
                            obb_points[4], obb_points[5], obb_points[6], obb_points[7], obb_sm);
    PMP::triangulate_faces(obb_sm);
    CGAL::IO::write_OBJ(output_path_obj, obb_sm);

    //Start by creating objects to store the .csv lines
    std::ifstream fin;
    std::string line_in;
    std::string CSV_line;

    // Save OBB points to file - Needed for generating Slicer fiducials
    try 
    {
        std::ofstream myFile(output_path_csv);
        myFile << obb_points[0] << "," << obb_points[1] << "," << obb_points[2] << "," << obb_points[3] <<
                "," << obb_points[4] << "," << obb_points[5] << "," << obb_points[6] << "," << obb_points[7];
        myFile.close();
    }
    catch (const std::exception &e) 
    {
        std::cerr << "Caught " << e.what() << " while trying to save OBB .csv file" << std::endl;
    }
    

    return 0;
}
