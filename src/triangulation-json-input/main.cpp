#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <nlohmann/json.hpp>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Triangulation_vertex_base_with_info_2.h>
#include <CGAL/Constrained_Delaunay_triangulation_2.h>
#include <CGAL/Polygon_2.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Triangulation_vertex_base_with_info_2<int, K> Vb;
typedef CGAL::Constrained_triangulation_face_base_2<K> Fb;
typedef CGAL::Triangulation_data_structure_2<Vb, Fb> TDS;
typedef CGAL::Exact_predicates_tag Itag;
typedef CGAL::Constrained_Delaunay_triangulation_2<K, TDS, Itag> CDT;
typedef CGAL::Polygon_2<K> Polygon_2;

using json = nlohmann::json;

int main() {

    json j;
    std::ifstream inFile("../json/contour.json");

    if (!inFile) {
        std::cout << "Error reading file" << std::endl;
        return 1;
    }
    inFile >> j;
    inFile.close();

    Polygon_2 polygon;
    for (const auto& element : j["points"]) {
        float x = element[0].get<float>();
        float y = element[1].get<float>();
        std::cout << "X: " << x << ", Y: " << y << std::endl;
        polygon.push_back(K::Point_2(x, y));
    }

    CDT cdt;
    cdt.insert_constraint(polygon.vertices_begin(), polygon.vertices_end(), true);

    json output;
    json triangles = json::array();
    for (CDT::Finite_faces_iterator fit=cdt.finite_faces_begin(); fit!=cdt.finite_faces_end();++fit) {
        CDT::Triangle t = cdt.triangle(fit);
        json triangle = {
            {t.vertex(0).x(), t.vertex(0).y()},
            {t.vertex(1).x(), t.vertex(1).y()},
            {t.vertex(2).x(), t.vertex(2).y()}
        };
        triangles.push_back(triangle);
        for (int i = 0; i < 3; i++){
            K::Point_2 p = t.vertex(i);
            std::cout << "(" << p.x() << ", " << p.y() << ")";
        }
        std::cout << std::endl;
    }
    output["triangles"] = triangles;
    std::string outputJsonStr = output.dump();
    std::cout << outputJsonStr << std::endl;

    std::fstream outFile("triangles.json", std::ios::in | std::ios::out | std::ios::app);
    if (outFile.is_open()) {
        outFile << outputJsonStr;
        outFile.close();
    } else {
        std::cerr << "Unable to open file";
    }
}
