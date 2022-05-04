cmake_minimum_required(VERSION 3.0.0)
project(todd VERSION 0.1.0)

include(CTest)
enable_testing()

aux_source_directory(src SRC)

add_executable(todd main.cpp ${SRC})

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
