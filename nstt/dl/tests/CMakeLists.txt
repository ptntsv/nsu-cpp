include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
FetchContent_MakeAvailable(googletest)
enable_testing()

# add_executable(
# )

# target_link_libraries(
#     GTest::gtest_main
# )

include(GoogleTest)
# gtest_discover_tests(#[[executable]])
