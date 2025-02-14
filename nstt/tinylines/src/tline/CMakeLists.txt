set(SRC_LIST tline.cpp tline.hpp)
add_library(tline ${SRC_LIST})
target_link_libraries(tline GTest::gtest_main)
