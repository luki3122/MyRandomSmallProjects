cmake_minimum_required(VERSION 3.10)

include(FetchContent)

FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG        release-1.11.0
)

FetchContent_MakeAvailable(googletest)
add_library(GTest::GTest INTERFACE IMPORTED)

add_executable(AnyType_Tests tests.cpp)

target_link_libraries(GTest::GTest INTERFACE gtest_main)

target_link_libraries(AnyType_Tests PRIVATE GTest::GTest string_list)

enable_testing()

add_test(AnyType_Tests AnyType_Tests)