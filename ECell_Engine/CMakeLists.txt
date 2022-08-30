# ~/ECell_Engine/CMakeLists.txt

cmake_minimum_required (VERSION 3.12)

message(STATUS "CMAKE_LIBRARY_OUTPUT_DIRECTORY: ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}")
message(STATUS "CMAKE_INSTALL_PREFIX: ${CMAKE_INSTALL_PREFIX}")
message(STATUS "CMAKE_BINARY_DIR: ${CMAKE_BINARY_DIR}")

#Specify the CMAKE_MODULE_PATH so that libsbml can find LibXml2 with its custom FindLIBXML.cmake
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/libsbml/CMakeModules")
message(STATUS "CMAKE_MODULE_PATH: ${CMAKE_MODULE_PATH}")

# Look for libsbml in the CMAKE_BINARY_DIR since we want the submodule version
# that was added as a subdirectory in the root CMakeLists.txt
find_package(libsbml REQUIRED PATHS "${CMAKE_BINARY_DIR}/libsbml/src")
message(STATUS "libsbml_FOUND: ${libsbml_FOUND}")
message(STATUS "LIBSBML_LIBRARIES: ${LIBSBML_LIBRARIES}")
message(STATUS "libsbml_BINARY_DIR: ${libsbml_BINARY_DIR}")
message(STATUS "LIBSBML_CMAKE_DIR: ${LIBSBML_CMAKE_DIR}")
message(STATUS "libsbml_DIR: ${libsbml_DIR}")
message(STATUS "LibSBML_INCLUDEDIR: ${LibSBML_INCLUDEDIR}")
message(STATUS "LibSBML_LIBDIR: ${LibSBML_LIBDIR}")
message(STATUS "LibSBML_LIBS: ${LibSBML_LIBS}")

find_package(glfw3 CONFIG REQUIRED)
find_package(imgui CONFIG REQUIRED)
find_package(Vulkan REQUIRED)
include_directories(${Vulkan_INCLUDE_DIRS})

# Add ECell_Engine source file
file(GLOB_RECURSE SOURCE_FILES CONFIGURE_DEPENDS "src/*.c" "src/*.cpp")
file(GLOB_RECURSE HEADER_FILES CONFIGURE_DEPENDS "headers/*.h" "headers/*.hpp" "engine_system_files/*.hpp")

add_executable(ECellEngine "Main.cpp" ${SOURCE_FILES} ${HEADER_FILES})

target_link_libraries(ECellEngine PRIVATE ${LIBSBML_LIBRARIES})
target_link_libraries(ECellEngine PRIVATE imgui::imgui)
target_link_libraries(ECellEngine PRIVATE glfw)
target_link_libraries(ECellEngine PRIVATE Vulkan::Vulkan)

target_include_directories(ECellEngine
						PRIVATE
							"./headers"
							"./engine_system_files"
							${libsbml_DIR}
							"../libsbml/src"
)