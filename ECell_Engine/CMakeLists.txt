# ~/ECell_Engine/CMakeLists.txt

cmake_minimum_required (VERSION 3.12)

# Add ECell_Engine source file
file(GLOB_RECURSE SOURCE_FILES CONFIGURE_DEPENDS "src/*.c" "src/*.cpp")

add_executable(ECellEngine "Main.cpp" ${SOURCE_FILES})
target_compile_features(ECellEngine PUBLIC cxx_std_17)

# Make a variable to hold the name of the produced sbml library.
# This is picked from the libsbml/CMakeLists file line 701
if(UNIX OR CYGWIN)
    set(LIBSBML_LIBRARY sbml)
else()
    if(MINGW)
        set(LIBSBML_LIBRARY sbml)
    else()
        set(LIBSBML_LIBRARY libsbml)
    endif()
endif()

target_link_libraries(ECellEngine ${LIBSBML_LIBRARY})

target_include_directories(ECellEngine
						PUBLIC
							"./headers"
							"./engine_system_files"
							"${LIBSBML_ROOT_BINARY_DIR}/src" # include sbml generated headers
							"${LIBSBML_ROOT_SOURCE_DIR}/src" 
)