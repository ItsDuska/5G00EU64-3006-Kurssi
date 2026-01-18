cmake_minimum_required(VERSION 3.24)
project(life LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB_RECURSE SOURCE_FILES
    src/*.cpp
)

add_executable(${PROJECT_NAME} ${SOURCE_FILES})

target_compile_options(${PROJECT_NAME} PRIVATE
    -Wall -Wextra -pedantic -Werror -O3
)

target_include_directories(${PROJECT_NAME} PRIVATE
    src
)

# setup: cmake -S . -B build
# compile: cmake -- build .
