include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
FetchContent_MakeAvailable(googletest)
enable_testing()

add_executable(
  int_treap_test
  int_treap_test.cpp
)

target_link_libraries(
  int_treap_test
  treap
  GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(int_treap_test)
