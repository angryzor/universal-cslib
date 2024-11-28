cmake_minimum_required(VERSION 3.28)
project(universal-cslib VERSION 1.0.0
                 DESCRIPTION "Universal CSLib"
                 LANGUAGES CXX)

option(UCSL_EIGEN_MATH "Use Eigen for math objects.")

add_subdirectory(vendor EXCLUDE_FROM_ALL)
add_subdirectory(universal-cslib)
add_subdirectory(universal-cslib-reflection)

export(TARGETS universal-cslib universal-cslib-reflection)
export(PACKAGE universal-cslib)
