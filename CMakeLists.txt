cmake_minimum_required(VERSION 3.12)
project(DefTh_Z3p)

##########################################################################
set(LIB_SRC ./lib_init.hpp ./lib_init.cpp
            ./Generators/generate.cpp ./Generators/generate.hpp 
            ./Relation/Relation.cpp ./Relation/Relation.hpp 
            ./Automorphism/Automorphism.hpp ./Automorphism/Automorphism.cpp
            ./Checkers/checkers.hpp ./Checkers/checkers.cpp
            ./Error/Error.hpp ./Error/Error.cpp

            ./Grid/IntVec/IntVec.hpp ./Grid/IntVec/IntVec.cpp
            ./Grid/Grid.hpp
            ./Grid/GridFunction.hpp ./Grid/GridFunction.cpp
            ./GridAutomorphism/GridAutomorphism.hpp ./GridAutomorphism/GridAutomorphism.cpp
            ./GridRelation/GridRelation.hpp ./GridRelation/GridRelation.cpp 
    )
set(MAIN_SRC ./example.cpp)
#########################################################################
include(FetchContent)
FetchContent_Declare(z3
        GIT_REPOSITORY https://github.com/Z3Prover/z3
        GIT_TAG        z3-4.12.1
        CMAKE_ARGS -DZ3_BUILD_EXAMPLES=OFF -DZ3_BUILD_TESTS=OFF
)
FetchContent_MakeAvailable(z3)

include_directories(${z3_SOURCE_DIR}/src/api/c++)
include_directories(${z3_SOURCE_DIR}/src)


##########################################################################

add_executable(MAIN ${MAIN_SRC} ${LIB_SRC})
set_target_properties(
    MAIN PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS OFF
)

target_link_libraries(MAIN libz3)
