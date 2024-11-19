cmake_minimum_required(VERSION 3.28)
project(universal-cslib VERSION 0.0.1
                 DESCRIPTION "Universal CSLib"
                 LANGUAGES CXX)

add_subdirectory(vendor EXCLUDE_FROM_ALL)
add_subdirectory(universal-cslib)
add_subdirectory(universal-cslib-reflection)
