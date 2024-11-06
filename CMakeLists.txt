cmake_minimum_required(VERSION 3.28)
project(universal-cslib VERSION 0.0.1
                 DESCRIPTION "Universal CSLib"
                 LANGUAGES CXX)

add_library(universal-cslib INTERFACE)
target_compile_features(universal-cslib INTERFACE cxx_std_20)
add_subdirectory(src)
