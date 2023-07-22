# specify the CMake minimum version
cmake_minimum_required(VERSION 3.1...3.15)

# Give your project a name
project(Triangulation)

# Find CGAL
find_package(CGAL REQUIRED)

# Create an executable from your cpp file
add_executable(triangulation triangulation.cpp)

# Link the CGAL library to your executable
target_link_libraries(triangulation CGAL::CGAL)