# ~/ECell_Engine/CMakeLists.txt

cmake_minimum_required (VERSION 3.12)

file(GLOB_RECURSE SOURCE_FILES CONFIGURE_DEPENDS "src/*.cpp")
file(GLOB_RECURSE HEADER_FILES CONFIGURE_DEPENDS "headers/*.hpp" "./engine_system_files/*.hpp")

add_executable(ECell_Engine "ECell_Engine.cpp" ${SOURCE_FILES} ${HEADER_FILES})

target_include_directories(ECell_Engine PRIVATE
							"./headers"
							"./engine_system_files")