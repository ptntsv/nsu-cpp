#include <gtest/gtest.h>

#define LOG
#include "../src/scopedPointer.hpp"

ScopedPointer<int> foo() {
    int *x = new int{42};
    if (*x < 12)
        return ScopedPointer<int>{nullptr};
    return ScopedPointer<int>(x);
}

TEST(SPTest, OwnershipTransfers) {
    ScopedPointer<int> sp1{foo()};
    ScopedPointer<int> sp2 = std::move(sp1);
    ASSERT_EQ(*sp2, 42);
    ASSERT_TRUE(sp1.empty());
}

TEST(SPTest, SelfMoving) {
    ScopedPointer<int> sp1{foo()};
    sp1 = std::move(sp1);
    ASSERT_TRUE(sp1.empty());
}

TEST(DCPTest, DeepCopies) {
    int *x = new int{42};
    DeepCopyScopedPointer<int> xp{x};
    DeepCopyScopedPointer<int> cxp{xp};
    ASSERT_EQ(*cxp, 42);
    ASSERT_NE(cxp.operator->(), xp.operator->());
}

TEST(DCPTest, DeepAssigns) {
    int *x = new int{42};
    DeepCopyScopedPointer<int> xp{x};
    DeepCopyScopedPointer<int> cxp{xp};
    ASSERT_EQ(*cxp, 42);
}
