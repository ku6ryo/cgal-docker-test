#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <nlohmann/json.hpp>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Triangulation_vertex_base_with_info_2.h>
#include <CGAL/Constrained_Delaunay_triangulation_2.h>
#include <CGAL/Polygon_2.h>
#include <map>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Triangulation_vertex_base_with_info_2<unsigned, K> Vb;
typedef CGAL::Constrained_triangulation_face_base_2<K> Fb;
typedef CGAL::Triangulation_data_structure_2<Vb, Fb> TDS;
typedef CGAL::Exact_predicates_tag Itag;
typedef CGAL::Constrained_Delaunay_triangulation_2<K, TDS, Itag> CDT;
typedef CGAL::Polygon_2<K> Polygon_2;

using json = nlohmann::json;

int main(int argc, char **argv)
{

    // get the first argument as the input file name
    if (argc < 2)
    {
        std::cout << "Usage: " << argv[0] << " <input file>" << std::endl;
        return 1;
    }
    std::string inputFileName = argv[1];

    json j_input;
    std::ifstream inFile(inputFileName);

    if (!inFile)
    {
        std::cout << "Error reading file" << std::endl;
        return 1;
    }
    inFile >> j_input;
    inFile.close();

    Polygon_2 polygon;
    std::map<K::Point_2, unsigned> point_indices;
    for (const auto &element : j_input["points"])
    {
        float x = element[0].get<float>();
        float y = element[1].get<float>();
        K::Point_2 p = K::Point_2(x, y);
        polygon.push_back(p);
        point_indices[p] = polygon.size() - 1;
    }

    CDT cdt;
    cdt.insert_constraint(polygon.vertices_begin(), polygon.vertices_end(), true);

    json j_output;
    json j_points = json::array();
    json j_indices = json::array();
    for (CDT::Finite_faces_iterator fit = cdt.finite_faces_begin(); fit != cdt.finite_faces_end(); ++fit)
    {
        CDT::Triangle t = cdt.triangle(fit);

        float cx = (t.vertex(0).x() + t.vertex(1).x() + t.vertex(2).x()) / 3;
        float cy = (t.vertex(0).y() + t.vertex(1).y() + t.vertex(2).y()) / 3;

        if (polygon.bounded_side(K::Point_2(cx, cy)) == CGAL::ON_BOUNDED_SIDE)
        {
            unsigned i0 = point_indices[fit->vertex(0)->point()];
            unsigned i1 = point_indices[fit->vertex(1)->point()];
            unsigned i2 = point_indices[fit->vertex(2)->point()];

            json indices = {i0, i1, i2};
            j_indices.push_back(indices);
            json points = {
                {t.vertex(0).x(), t.vertex(0).y()},
                {t.vertex(1).x(), t.vertex(1).y()},
                {t.vertex(2).x(), t.vertex(2).y()}};
            j_points.push_back(points);
        }
    }
    j_output["points"] = j_points;
    j_output["indices"] = j_indices;
    std::string outputJsonStr = j_output.dump();
    std::fstream outFile("triangles.json", std::ios::out | std::ios::trunc);
    if (outFile.is_open())
    {
        outFile << outputJsonStr;
        outFile.close();
    }
    else
    {
        std::cerr << "Unable to open file";
    }
}
