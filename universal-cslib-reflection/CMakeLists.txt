add_library(universal-cslib-reflection INTERFACE)
target_compile_features(universal-cslib-reflection INTERFACE cxx_std_20)
target_link_libraries(universal-cslib-reflection INTERFACE universal-cslib simple-reflection)
add_subdirectory(src)
