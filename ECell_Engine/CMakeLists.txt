# ~/ECell_Engine/CMakeLists.txt

cmake_minimum_required (VERSION 3.12)

find_package (Threads)

file(GLOB_RECURSE SOURCE_FILES CONFIGURE_DEPENDS "src/*.c" "src/*.cpp")
file(GLOB_RECURSE HEADER_FILES CONFIGURE_DEPENDS "headers/*.h" "headers/*.hpp" "engine_system_files/*.hpp")



add_executable(ECell_Engine "launcher.cpp" ${SOURCE_FILES} ${HEADER_FILES})

target_link_libraries (ECell_Engine Threads::Threads)

target_include_directories(ECell_Engine PRIVATE
							"./headers"
							"./engine_system_files")
