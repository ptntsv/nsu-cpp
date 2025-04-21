cmake_minimum_required(VERSION 3.29)
project(meta-prog)

set(CMAKE_CXX_STANDARD 17)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)

FetchContent_MakeAvailable(googletest)
enable_testing()

add_executable(
    nth_prime
    nth_prime.cpp
)

# target_link_libraries(
#     nth_prime
#     GTest::gtest_main
# )
#
# include(GoogleTest)
# gtest_discover_tests(nth_prime_test)
