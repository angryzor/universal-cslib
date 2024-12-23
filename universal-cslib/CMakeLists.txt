add_library(universal-cslib INTERFACE)
target_compile_features(universal-cslib INTERFACE cxx_std_20)
target_link_libraries(universal-cslib INTERFACE Eigen3::Eigen)
add_subdirectory(src)
