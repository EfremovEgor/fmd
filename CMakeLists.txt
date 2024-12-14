cmake_minimum_required(VERSION 3.0
)

project(fmd
    VERSION 0.0.7
    DESCRIPTION "fmd"
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(
    fmd
    src/main.cpp
    src/commands.cpp
)