# specify the CMake minimum version
cmake_minimum_required(VERSION 3.1...3.15)

# Give your project a name
project(ConvexHull)

# Find CGAL
find_package(CGAL REQUIRED)

# Create an executable from your cpp file
add_executable(convex_hull convex_hull.cpp)

# Link the CGAL library to your executable
target_link_libraries(convex_hull CGAL::CGAL)