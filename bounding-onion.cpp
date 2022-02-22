#include <CGAL/Simple_cartesian.h>
#include <CGAL/Polygon_2.h>
#include <CGAL/point_generators_2.h>
#include <CGAL/random_convex_set_2.h>
#include <CGAL/min_quadrilateral_2.h>
#include <CGAL/optimal_bounding_box.h>
#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Exact_predicates_exact_constructions_kernel_with_sqrt.h>
#include <CGAL/IO/OBJ.h>
#include <CGAL/Optimal_bounding_box/Oriented_bounding_box_traits_3.h>

#include <iostream>

using K = CGAL::Exact_predicates_inexact_constructions_kernel;
int main(int argc, char** argv)
{
    if (argc < 2) {
        std::cerr << "please provide input OBJ file" << std::endl;
        return 1;
    }

    std::vector<K::Point_3> points_ref;
    std::vector<std::vector<std::size_t> > faces_ref;
    if (!CGAL::IO::read_OBJ(argv[1], points_ref, faces_ref)) {
        std::cerr << "error reading OBJ" << std::endl;
        return 1;
    }

    std::array<K::Point_3, 8> output;
    CGAL::oriented_bounding_box(points_ref, output);

    for(int i = 0; i < 8; i += 1) {
        std::cout << output[i] << std::endl;
    }

    return 0;
}
