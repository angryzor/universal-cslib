add_library(universal-cslib INTERFACE)
target_compile_features(universal-cslib INTERFACE cxx_std_20)
add_subdirectory(src)
