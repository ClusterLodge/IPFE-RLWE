cmake_minimum_required(VERSION 4.0)
project(cifer_bench)

find_package(benchmark REQUIRED)

link_directories(
    ../
    /opt/homebrew/Cellar/gmp/6.3.0/lib
)

include_directories(
    ../src
)

add_subdirectory(../ ipfe_rlwe)

add_executable(ipfe_rlwe_bench ipfe-rlwe-bench.cpp)
target_link_libraries(ipfe_rlwe_bench benchmark::benchmark ipfe_rlwe)
