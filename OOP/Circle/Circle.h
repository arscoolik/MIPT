#pragma once
#include <cmath>
#include <iostream>
#include <cmath>

#include "point.h"

using namespace std;

class Circle
{
private:
	Point center;
	float radius;
public:
    Circle(const Point& acenter, float aradius);
    Circle();

    float get_radius() const;

    Point get_center() const;

    void set_radius(float radius);

    void set_center(const Point& center);

    Circle(const Circle& circle);

    float get_area() const;

    float get_distance(const Point& p) const;

    bool is_colliding(const Circle& c) const;

    void move(const Point& p);

};
