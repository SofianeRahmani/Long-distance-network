cmake_minimum_required(VERSION 3.5)
project(TP2)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

include (FetchContent)
FetchContent_Declare (
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG v1.12.0

)
FetchContent_MakeAvailable(googletest)


set(SOURCE_FILES
    Graphe.cpp
    Graphe.h
    ReseauInterurbain.cpp
    ReseauInterurbain.h
    Principal.cpp)

add_executable(TP2 ${SOURCE_FILES}
        Graphe.cpp
        Graphe.h
        ReseauInterurbain.cpp
        ReseauInterurbain.h
        Principal.cpp)
enable_testing()
add_subdirectory(Tests)