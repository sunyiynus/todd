Include(FetchContent)

FetchContent_Declare(
  Catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG        v3.0.0-preview5
)

FetchContent_MakeAvailable(Catch2)
#FetchContent_GetProperties(Catch2 Catch2_SOURCE_DIR)
#set(CATCH_INCLUDE_DIR ${source_dir}/include CACHE INTERNAL "Path")
file(GLOB SRC ./*.cpp)

add_executable(tests ${SRC})
target_link_libraries(tests PRIVATE Catch2::Catch2WithMain)
message("this is funcing mess ${Catch2_SOURCE_DIR}")

target_include_directories(tests INTERFACE ${SRC_INCLUDE_PATH})

#target_include_directories(tests INTERFACE ${Catch2_SOURCE_DIR}/src)

add_test (NAME MyTest COMMAND Test)




