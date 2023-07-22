#include <iostream>
#include <vector>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef K::Point_2 Point_2;
typedef CGAL::Delaunay_triangulation_2<K> Delaunay;

int main() {
    std::vector<Point_2> points = {
        Point_2(5,2),
        Point_2(6,4),
        Point_2(9,5),
        Point_2(5,8),
        Point_2(2,4)
    };

    Delaunay dt;
    dt.insert(points.begin(), points.end());

    // iterate over faces (triangles)
    for (auto face = dt.all_faces_begin(); face != dt.all_faces_end(); ++face) {
        // vertices of the face
        Point_2 p1 = face->vertex(0)->point();
        Point_2 p2 = face->vertex(1)->point();
        Point_2 p3 = face->vertex(2)->point();
        
        std::cout << "Triangle vertices: (" << p1 << "), (" << p2 << "), (" << p3 << ")\n";
    }

    return 0;
}