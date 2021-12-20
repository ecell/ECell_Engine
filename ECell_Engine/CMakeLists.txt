# ~/ECell_Engine/CMakeLists.txt

cmake_minimum_required (VERSION 3.8)

add_executable(ECell_Engine "ECell_Engine.cpp")

target_include_directories(ECell_Engine PRIVATE "./headers")