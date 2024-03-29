# ~/CMakeLists.txt

cmake_minimum_required (VERSION 3.12)

project ("ECell_Engine")

# In Windows, generated dlls and .exe must be at the same place
# This makes sure that the libsbml.dll will be at the same place
# as the ECellEngine.exe 
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin" CACHE STRING "" )
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin" CACHE STRING "" )

# Same purpose as above but for the dlls added with vcpkg
set(X_VCPKG_APPLOCAL_DEPS_INSTALL ON)

# For consistency we separate the potential .lib generated
set (CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib" CACHE STRING "")

set(BUILD_SHARED_LIBS True)
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS True)

#Use Expat instead of libxml2
#set(WITH_EXPAT ON)
#set(WITH_XERCES ON)
#set(WITH_LIBXML OFF)

# Include sub-directories
add_subdirectory ("libsbml")
add_subdirectory ("ECell_Engine")
add_subdirectory ("Editor")