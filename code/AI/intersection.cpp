#include <iostream>
using namespace std;
 
struct MyPoint
{
    int x;
    int y;
};
 
// Given three colinear MyPoints p, q, r, the function checks if
// MyPoint q lies on line segment 'pr'
bool onSegment(MyPoint p, MyPoint q, MyPoint r)
{
    if (q.x <= max(p.x, r.x) && q.x >= min(p.x, r.x) &&
        q.y <= max(p.y, r.y) && q.y >= min(p.y, r.y))
       return true;
 
    return false;
}
 
// To find orientation of ordered triplet (p, q, r).
// The function returns following values
// 0 --> p, q and r are colinear
// 1 --> Clockwise
// 2 --> Counterclockwise
int orientation(MyPoint p, MyPoint q, MyPoint r)
{
    // See 10th slides from following link for derivation of the formula
    // http://www.dcs.gla.ac.uk/~pat/52233/slides/Geometry1x1.pdf
    int val = (q.y - p.y) * (r.x - q.x) -
              (q.x - p.x) * (r.y - q.y);
 
    if (val == 0) return 0;  // colinear
 
    return (val > 0)? 1: 2; // clock or counterclock wise
}
 
// The main function that returns true if line segment 'p1q1'
// and 'p2q2' intersect.

bool get_line_intersection(float p0_x, float p0_y, float p1_x, float p1_y, 
    float p2_x, float p2_y, float p3_x, float p3_y, float i_x, float i_y)
{
    float s1_x, s1_y, s2_x, s2_y;
    s1_x = p1_x - p0_x;     s1_y = p1_y - p0_y;
    s2_x = p3_x - p2_x;     s2_y = p3_y - p2_y;

    float s, t;
    s = (-s1_y * (p0_x - p2_x) + s1_x * (p0_y - p2_y)) / (-s2_x * s1_y + s1_x * s2_y);
    t = ( s2_x * (p0_y - p2_y) - s2_y * (p0_x - p2_x)) / (-s2_x * s1_y + s1_x * s2_y);

    if (s >= 0 && s <= 1 && t >= 0 && t <= 1)
    {
        // Collision detected
        if (i_x != NULL)
            i_x = p0_x + (t * s1_x);
        if (i_y != NULL)
            i_y = p0_y + (t * s1_y);
        return true;
    }

    return false; // No collision
}

bool doIntersect(MyPoint p1, MyPoint q1, MyPoint p2, MyPoint q2)
{
    // Find the four orientations needed for general and
    // special cases
    int o1 = orientation(p1, q1, p2);
    int o2 = orientation(p1, q1, q2);
    int o3 = orientation(p2, q2, p1);
    int o4 = orientation(p2, q2, q1);
 
    // General case
    if (o1 != o2 && o3 != o4)
        return true;
 
    // Special Cases
    // p1, q1 and p2 are colinear and p2 lies on segment p1q1
    if (o1 == 0 && onSegment(p1, p2, q1)) return true;
 
    // p1, q1 and p2 are colinear and q2 lies on segment p1q1
    if (o2 == 0 && onSegment(p1, q2, q1)) return true;
 
    // p2, q2 and p1 are colinear and p1 lies on segment p2q2
    if (o3 == 0 && onSegment(p2, p1, q2)) return true;
 
     // p2, q2 and q1 are colinear and q1 lies on segment p2q2
    if (o4 == 0 && onSegment(p2, q1, q2)) return true;
 
    return false; // Doesn't fall in any of the above cases
}
 
// Driver program to test above functions
int main()
{
    struct MyPoint p1 = {69 , 168}, q1 = {500 , 300};
    struct MyPoint p2 = {133, 168}, q2 = {500 , 450};
 
    doIntersect(p1, q1, p2, q2)? cout << "Yes\n": cout << "No\n";

    cout<<get_line_intersection( p1.x,  p1.y,  p2.x,  p2.y,  q1.x,  q1.y,  q2.x,  q2.y, 0 , 0)<<endl;
 
    p1 = {0, 0}, q1 = {1000, 1000};
    p2 = {10, 5}, q2 = {100, 100};
    doIntersect(p1, q1, p2, q2)? cout << "Yes\n": cout << "No\n";
 
    p1 = {0 , 10}, q1 = {0, 0};
    p2 = {1, 10}, q2 = {0 , 0};
    doIntersect(p1, q1, p2, q2)? cout << "Yes\n": cout << "No\n";
 
    return 0;
}