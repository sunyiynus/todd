cmake_minimum_required(VERSION 3.0.0)
project(todd VERSION 0.1.0)

include(CTest)
enable_testing()

aux_source_directory(src SRC)
add_library(tod STATIC ${SRC})

add_executable(todd main.cpp)
target_link_libraries(todd tod)
target_compile_options(todd PRIVATE -Werror)
target_compile_features(todd PRIVATE cxx_std_11)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)

add_subdirectory(test)