# ~/ECell_Engine/CMakeLists.txt

cmake_minimum_required (VERSION 3.12)

# Add ECell_Engine source files
add_library(ECellEngine SHARED
    "src/Core/Engine.cpp"
    "src/Core/Simulation.cpp"
    "src/Core/SimulationsManager.cpp"
    "src/Core/Timer.cpp"
    "src/Core/Track.cpp"

    "src/Core/Events/ModifyDataStateValueEvent.cpp"

    "src/Data/BiochemicalModule.cpp"
    "src/Data/Constant.cpp"
    "src/Data/DataState.cpp"
    "src/Data/DependenciesDatabase.cpp"
    "src/Data/Parameter.cpp"
    "src/Data/Species.cpp"
    "src/Data/IndexedMinHeap.cpp"

    "src/IO/Command.cpp"
    "src/IO/Commands.cpp"
    "src/IO/CommandsManager.cpp"
    "src/IO/ModuleImporterManager.cpp"
    "src/IO/SBMLModuleImporter.cpp"
    "src/IO/SbmlParser.cpp"
    #"src/IO/SbmlWriter.cpp"
    
    "src/Maths/Function.cpp"
    "src/Maths/Logic.cpp"
    "src/Maths/LogicOperation.cpp"
    "src/Maths/MathUtility.cpp"
    "src/Maths/Operand.cpp"
    "src/Maths/Operation.cpp"
    "src/Maths/revrand.c"

    "src/Logging/Logger.cpp"

    "src/Solver/BiochemicalSolver.cpp"
    "src/Solver/ODE/ExplicitRKCoefficients.cpp"
    "src/Solver/ODE/GeneralizedExplicitRK.cpp"
    "src/Solver/ODE/ODEStepper.cpp"
    "src/Solver/Stochastic/GillespieNRMRSolver.cpp"
    #"src/Solver/Stochastic/Gillespie_NRM_R.cpp"
    "src/Solver/Stochastic/GillespieNRMRStepper.cpp"

    "src/Util/StrUtil.cpp"
)

target_compile_features(ECellEngine PUBLIC cxx_std_17)

find_package(Threads REQUIRED)

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

if (MSVC)

    target_compile_options(ECellEngine PRIVATE /MP)	# Multithread compilation

endif()

target_link_libraries(ECellEngine
						PUBLIC
                            ${LIBSBML_LIBRARY}
                        PRIVATE
                            "Threads::Threads"
)

target_include_directories(ECellEngine
						PUBLIC
#							"${VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}/include"	# headers of dependencies installed through VCPKG
							"./headers"
							"./engine_system_files"
							"${LIBSBML_ROOT_BINARY_DIR}/src" # include sbml generated headers
							"${LIBSBML_ROOT_SOURCE_DIR}/src" 
)