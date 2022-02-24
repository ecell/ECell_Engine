# ~/CMakeLists.txt


cmake_minimum_required (VERSION 3.12)

project ("ECell_Engine")

# specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

find_package(PkgConfig REQUIRED)

# Incluez les sous-projets.
add_subdirectory ("ECell_Engine")
