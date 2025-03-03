#pragma once
struct Point {
    double x, y;
    Point(double x, double y) { this->x = x, this->y = y; }
};

class Line {
    double A, B, C;

  public:
    double a() const { return A; }
    double b() const { return B; }
    double c() const { return C; }

    Line(const Point& p1, const Point& p2) {
        A = p2.y - p1.y;
        B = p1.x - p2.x;
        C = p2.x * p1.y - p1.x * p2.y;
    }

    Line(double a, double b, double c) {
        A = a;
        B = b;
        C = c;
    }

    bool intersects(const Line& other) const;
    Line getPerpendicular(const Point& p) const;
};
