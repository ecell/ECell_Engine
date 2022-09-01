# ~/ECell_Engine/CMakeLists.txt

cmake_minimum_required (VERSION 3.12)

# Add ECell_Engine source file
file(GLOB_RECURSE SOURCE_FILES CONFIGURE_DEPENDS "src/*.c" "src/*.cpp")

add_library(ECellEngine SHARED ${SOURCE_FILES} ${HEADER_FILES})
target_compile_features(ECellEngine PUBLIC cxx_std_17)

target_link_libraries (ECellEngine libsbml)

target_include_directories(ECellEngine
						PUBLIC
							"./headers"
							"./engine_system_files"
							"${LIBSBML_ROOT_BINARY_DIR}/src" # include sbml generated headers
							"${LIBSBML_ROOT_SOURCE_DIR}/src" 
)