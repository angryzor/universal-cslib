cmake_minimum_required(VERSION 3.28)
project(universal-cslib VERSION 1.0.5
                 DESCRIPTION "Universal CSLib"
                 LANGUAGES CXX)

option(UCSL_EIGEN_MATH "Use Eigen for math objects.")

include(FetchContent)
if(${UCSL_EIGEN_MATH})
    FetchContent_Declare(Eigen3 GIT_REPOSITORY https://gitlab.com/libeigen/eigen.git GIT_TAG 3.4.0 EXCLUDE_FROM_ALL FIND_PACKAGE_ARGS 3.4)
    FetchContent_MakeAvailable(Eigen3)
endif()
FetchContent_Declare(simple-reflection GIT_REPOSITORY https://github.com/angryzor/simple-reflection.git GIT_TAG v1.0.0 EXCLUDE_FROM_ALL FIND_PACKAGE_ARGS 1.0)
FetchContent_MakeAvailable(simple-reflection)

add_subdirectory(universal-cslib)
add_subdirectory(universal-cslib-reflection)
