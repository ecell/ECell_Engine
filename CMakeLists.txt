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

#Use Expat instead of libxml2
set(WITH_LIBXML OFF)
set(WITH_EXPAT ON)

# Even in Debug, libsbml requires the release version of zlib1.dll
# (instead of zlibd1.dll). This is a problem because vcpkg only
# copies the dlls that matches the configuration. So, in debug
# only zlibd1.dll is copied to the runtime directory. The current
# workaround is to deactivate ZLIB in debug. It is not a critical
# issue as zlib only enables to read compressed sbml file which
# we can surely avoid while in debug.
IF(CMAKE_BUILD_TYPE STREQUAL "Debug")
    message (STATUS "Debug configuration detected, deactivating ZLIB support for libsbml.")
    set(WITH_ZLIB OFF)
ENDIF(CMAKE_BUILD_TYPE MATCHES DEBUG)


# Incluez les sous-projets.
add_subdirectory ("libsbml")
add_subdirectory ("ECell_Engine")