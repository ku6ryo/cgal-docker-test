#include <iostream>
#include <vector>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/convex_hull_2.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef K::Point_2 Point_2;

int main()
{
    std::vector<Point_2> points;
    points.push_back(Point_2(5,2));
    points.push_back(Point_2(6,4));
    points.push_back(Point_2(9,5));
    points.push_back(Point_2(5,8));
    points.push_back(Point_2(2,4));

    // compute convex hull
    std::vector<Point_2> result;
    CGAL::convex_hull_2(points.begin(), points.end(), std::back_inserter(result));

    std::cout << "The convex hull contains " << result.size() << " points.\n";
    for(auto p : result){
        std::cout << "(" << p.x() << ", " << p.y() << ")\n";
    }

    return 0;
}