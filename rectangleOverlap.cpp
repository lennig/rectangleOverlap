//
//  rectangleOverlap.cpp
//  rectangleOverlap
//
//  Created by matt on 9/2/23.

//  How to build example:
//  g++ -std=c++17 -o rectangleOverlap  rectangleOverlap.cpp main.cpp


#include "rectangleOverlap.hpp"

namespace ro {

Point::Point(double const x, double const y){
    this->x = x;
    this->y = y;
};

void Point::print() const {
    std::cout << "(" << x << ", "  << y << ")   ";
};

Axis::Axis(double const x, double const y) : Point(x,y){
    // Axis is constructed to have length one
    // except in the degenerate case when both x and y are zero, i.e.:
    //   Axis axis = new Axis(0,0)
    // In the degenerate case, both  isnan(axis.x) and isnan(axis.y) will be true
    // Guaranteeing Axis objects have length one removes the division operation
    // from Axis::proj()

    double mag = sqrt( x*x + y*y );
    this->x /= mag;
    this->y /= mag;
};

double Axis::proj(const Point& p) const {
    // Return the scalar projection of p onto this axis
    // No division necessary because Axis has unit length
    return this->x*p.x + this->y*p.y ;
};

void Rectangle::rotate(double const r){
    // rotate the rectangle r degrees
    double cosAngle = cos(r * 3.14159/180);
    double sinAngle = sin(r * 3.14159/180);
    
    for (int i = 0; i < 4; i++){
        double x1 = vertices[i].x - xCenter;
        double y1 = vertices[i].y - yCenter;
        
        double x2 = x1 * cosAngle - y1 * sinAngle;
        double y2 = x1 * sinAngle + y1 * cosAngle;
        
        vertices.at(i).x = x2 + xCenter;
        vertices.at(i).y = y2 + yCenter;
    }
};

Rectangle::Rectangle(const double x, const double y, const double w, const double h, const double r){
    // Make a rectangle given center (x,y), width w, height h, and rotation angle r in degrees
    xCenter = x;
    yCenter = y;
    
    // Distance from center to side, horizontally and vertically
    double dw = w/2;
    double dh = h/2;
    
    // Initialize the rectangle unrotated, then rotate it
    vertices.emplace_back(x - dw,y - dh);
    vertices.emplace_back(x + dw,y - dh);
    vertices.emplace_back(x + dw,y + dh);
    vertices.emplace_back(x - dw,y + dh);
    rotate(r);
};

void Rectangle::print() const {
    std::cout << "Rectangle vertices: ";
    for (auto it = vertices.begin(); it != vertices.end(); it++){
        std::cout << "(" << (*it).x << ", "  << (*it).y << ")   ";
    };
    std::cout << std::endl;
}

double Rectangle::slope() const {
    // Returns the slope of one of the sides of the rectangle
    // Slope returned is guaranteed to be finite
    
    // Get two consecutive vertices of the rectangle; call them p1, p2
    auto it = vertices.begin();
    Point p1 = *it++;
    Point p2 = *it;
    
    if (p1.x - p2.x != 0) {
        return (p1.y - p2.y) / (p1.x - p2.x);
    }
    else{
        return 0;
    }
}

Axis Rectangle::separationAxis(Rectangle rect) const {
    // Compute the the projection axes onto which vertices will be projected
    std::vector<ro::Axis> axes;     // Will hold the projection axes
    
    // Compute two projection axes from rect
    double s = rect.slope();
    
    if (s != 0) {
        // Projection axes are not parallel to main axes
        axes.emplace_back(1,s);
        axes.emplace_back(1,-1/s);
    }
    else{
        // Projection axes are parallel to main axes
        axes.emplace_back(1,0);
        axes.emplace_back(0,1);
    }
    
    // Compute two projection axes from this Rectangle
    s = this->slope();
    
    if (s != 0) {
        // Rectangle has been rotated
        axes.emplace_back(1, s);
        axes.emplace_back(1, -1/s);
    }
    else{
        // Projection axes are parallel to the main axes
        axes.emplace_back(1,0);
        axes.emplace_back(0,1);
    }
    
    // Iterate through projection axes, projecting all 8 vertices onto each axis and checking for overlap
    for (auto theAxis=axes.begin(); theAxis != axes.end(); theAxis++){
        // Compute min and max projections of vertices of rect1 onto theAxis
        double minRect1Proj =  DBL_MAX;  // accumulates min projection onto theAxis of rect1 vertices
        double maxRect1Proj = -DBL_MAX;  // accumulates max projection onto theAxis of rect1 vertices
        
        for (auto vertex = rect.vertices.begin(); vertex != rect.vertices.end(); vertex++){
            double pr = theAxis->proj(*vertex);     // Compute projection of this vertex onto theAxis
            if (pr > maxRect1Proj) maxRect1Proj = pr;
            if (pr < minRect1Proj) minRect1Proj = pr;
        };
        
        // Compute min and max projections of vertices of second rectangle onto theAxis
        double minRect2Proj =  DBL_MAX;  // accumulates min projection onto theAxis of rect2 vertices
        double maxRect2Proj = -DBL_MAX;  // accumulates max projection onto theAxis of rect2 vertices
        
        for (auto vertex = vertices.begin(); vertex != vertices.end(); vertex++){
            double pr = theAxis->proj(*vertex);     // Compute projection of this vertex onto theAxis
            if (pr > maxRect2Proj) maxRect2Proj = pr;
            if (pr < minRect2Proj) minRect2Proj = pr;
        };
        
        bool overlapped = !(minRect1Proj > maxRect2Proj || minRect2Proj > maxRect1Proj);
        
        if ( !overlapped ){
            return *theAxis;
        }

    }  // end of axes loop
    
    return Axis(0,0);  // Indicates rectangles are overlapped
};

bool Rectangle::overlapped(Rectangle const& rect) const{
    Axis a = this->separationAxis(rect);
    if (isnan(a.x)){
        return false;
    }
    else {
        return true;
    }
};

bool Rectangle::overlapped(Rectangle const& rect, Axis& a) const {
    a = separationAxis(rect);
    if (isnan(a.x)){
        return true;
    }
    else {
        return false;
    }
}

} // end of namespace ro

