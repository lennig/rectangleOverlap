# rectangleOverlap
Given two rectangles in (x,y) space, computes whether or not they overlap one another.  
If they do not overlap, optionally provides their axis of separation.

**How to use:**  
Instantiate a rectangle rect1 like this: 
 
  ```
Rectangle rect = new Rectangle(x, y, w, h, r)  
  ```
  
  where  
   (x,y) = center of rect  
   (w,h) = width and height of rect  
     r   = rotation in degrees of rect  

Given two rectangles, rect1 and rect2,  

  ```     
bool overlapped = rect1.overlapped(rect2) 
  ```     
    
returns true iff the two rectangles are overlapped.  

  ```
overlapped = rect2.overlapped(rect1)  
  ```
returns the same value.  

If desired, the axis of separation may be obtained.  
The axis of separation is an axis upon which the orthogonal  
projections of the vertices of the two rectangles do not overlap.    

To obtain an axis of separation, the caller must pass in an  
empty Axis object, for example:  
  ```
Axis aos = new Axis(0,0);  
overlapped = rect1.overlapped(rect2, aos)
  ```  
If overlapped() returns false, then aos contains an axis of separation  
If overlapped() returns true, then aos is undefined  

The algorithm implemented here is the one described in this article:  
 https://jkh.me/files/tutorials/Separating%20Axis%20Theorem%20for%20Oriented%20Bounding%20Boxes.pdf  
which is a special case (for rectangles) of the Separating Axis Theorem for convex polygons  

**How to build example:**  
 g++ -std=c++17 -o rectangleOverlap  rectangleOverlap.cpp main.cpp  

