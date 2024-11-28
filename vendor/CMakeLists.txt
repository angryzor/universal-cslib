include(FetchContent)
FetchContent_Declare(
    Eigen3
    GIT_REPOSITORY https://gitlab.com/libeigen/eigen.git
    GIT_TAG 3.4.0
    FIND_PACKAGE_ARGS 3.4
)
FetchContent_Declare(
    simple-reflection
    GIT_REPOSITORY https://github.com/angryzor/simple-reflection.git
    GIT_TAG v1.0.0
    FIND_PACKAGE_ARGS 1.0
)
FetchContent_MakeAvailable(Eigen3 simple-reflection)
