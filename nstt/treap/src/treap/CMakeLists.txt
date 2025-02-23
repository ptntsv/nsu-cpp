set(SRC_LIST treap.cpp treap_node.cpp)
add_library(treap ${SRC_LIST})
target_link_libraries(treap GTest::gtest_main)
