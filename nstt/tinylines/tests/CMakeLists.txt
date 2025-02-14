include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
FetchContent_MakeAvailable(googletest)
enable_testing()

add_executable(
  tline_test
  tline_test.cpp
)

target_link_libraries(
  tline_test
  tline
  GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(tline_test)
