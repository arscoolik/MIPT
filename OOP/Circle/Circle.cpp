#include <iostream>
#include <cmath>

#include "point.h"
#include "circle.h"
const float pi = 3.14159265358979;
using namespace std;


Circle::Circle(const Point& acenter, float aradius): center(acenter), radius(aradius){}
Circle::Circle() : center({0, 0}), radius(1) {}

    float Circle::get_radius() const {
	return radius;
    }

    Point Circle::get_center() const {
        return center;
    }

    void Circle::set_radius(float radius) {
        if (radius<0)
            radius = 0;
        this->radius = radius;
    }

    void Circle::set_center(const Point& center) {
        this->center = center;
    }

    Circle::Circle(const Circle& circle){
        radius = circle.get_radius();
        center = circle.get_center();
    }

    float Circle::get_area() const{
        return pi * radius*radius;
    }

    float Circle::get_distance(const Point& p) const{
        return abs(center.distance(p) - radius);
    }

    bool Circle::is_colliding(const Circle& c) const{
        if (c.get_center().distance(center) < radius+c.get_radius())
            return true;
        return false;
    }
    void Circle::move(const Point& p){
        center = center+p;
    }
