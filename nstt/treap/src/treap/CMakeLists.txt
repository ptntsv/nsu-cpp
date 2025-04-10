set(SRC_LIST treap.hpp)
add_library(treap ${SRC_LIST})
target_link_libraries(treap GTest::gtest_main)
