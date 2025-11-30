cmake_minimum_required(VERSION 3.5)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_C_VERSION 11)

project(pgbench)
add_executable(${PROJECT_NAME} pgbench.c)