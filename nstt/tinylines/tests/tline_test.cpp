#include <gtest/gtest.h>
#include "../src/tline/tline.hpp"

TEST(LineTest, LineInitializes) {
    Line l1(1, 2, 3);
    Line l2(Point(1, 2), Point(3, 9));
    EXPECT_EQ(l2.a(), 7);
    EXPECT_EQ(l2.b(), -2);
    EXPECT_EQ(l2.c(), -3);
}

TEST(LineTest, LinesIntersect) {
    Line l1(5, -1, 0);
    Line l2(0, 1, -5);
    // y = 5x and y = 2
    EXPECT_EQ(l1.intersects(l2), true);
}

TEST(LineTest, LinesNotIntersect) {
    Line l1(5, -1, 0);
    Line l2(5, -1, 10);
    // y = 5x and y = 5x + 10
    EXPECT_EQ(l1.intersects(l2), false);
}

TEST(LineTest, LinesIntersectWithZeroes) {
    Line l1(1, 0, 128);
    Line l2(1, -1, 10);
    EXPECT_EQ(l1.intersects(l2), true);
}

TEST(LineTest, LinesIntersectPerpendicular) {
    Line l1(0, 5, 0);
    Line l2(10, 3, 0);
    EXPECT_EQ(l1.intersects(l2), true);
}

TEST(LineTest, PerpToLine) {
    Line l(2, -1, 1);
    Line pl = l.getPerpendicular(Point(2, 4));
    EXPECT_EQ(pl.a(), -1);
    EXPECT_EQ(pl.b(), -2);
    EXPECT_EQ(pl.c(), 10);
    EXPECT_EQ(l.intersects(pl), true);
}
