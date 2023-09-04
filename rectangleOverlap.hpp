//
//  rectangleOverlap.hpp
//  rectangleOverlap
//
//  Created by matt on 9/2/23.
//
//
// Computes whether two rectangles in (x,y) space overlap each other

// How to use:
//  Instantiate a rectangle rect like this:
//    Rectangle rect = new Rectangle(x, y, w, h, r)
// where
//   (x,y) = center of rect
//   (w,h) = width and height of rect
//     r   = rotation in degrees of rect

//  Given two rectangles, rect1 and rect2,
//    bool overlapped = rect1.overlapped(rect2)
//  returns true iff the two rectangles are overlapped.
//    overlapped = rect2.overlapped(rect1)
//  returns the same value.
//
// Axis of Separation
// If desired, an axis of separation may be obtained.
// The axis of separation is an axis upon which the orthogonal
// projections of the vertices of the two rectangles do not overlap.

// To obtain an axis of separation, the caller must pass in an
// empty Axis object, for example:
//   Axis aos = new Axis(0,0);
//   overlapped = rect1.overlapped(rect2, aos)
// If overlapped() returns false, then aos contains the axis of separation
// If overlapped() returns true, then aos is undefined
//
// The algorithm implemented here is the one described in this article:
// https://jkh.me/files/tutorials/Separating%20Axis%20Theorem%20for%20Oriented%20Bounding%20Boxes.pdf
// which is a special case (for rectangles) of the Separating Axis Theorem for convex polygons

// How to build example:
// g++ -std=c++17 -o rectangleOverlap  rectangleOverlap.cpp main.cpp


#pragma once

#include <iostream>
#include <cfloat>
#include <vector>

namespace ro {

// Class Point
class Point {
public:
    double x;
    double y;
    
    Point(double const x, double const y);
    void print() const;
};

// Class Axis
class Axis : public Point {
public:
    Axis(double const x, double const y);
    double proj(const Point& p) const;
};

// Class Rectangle
class Rectangle {
private:
    void rotate(double const rdegrees);
    Axis separationAxis(const Rectangle& rect) const;
public:
    bool overlapped(Rectangle const& rect) const;
    bool overlapped(Rectangle const& rect, Axis& axisOfSeparation) const;
    
    Rectangle(double const x, double const y, double const w, double const h, double const r);
    
    std::vector<Point> vertices;
    double xCenter;
    double yCenter;

    void print() const;
    double slope() const;
};

} // end namespace ro



