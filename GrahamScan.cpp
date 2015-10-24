#include <algorithm>
#include <iostream>
#include <stack>
#include <stdlib.h>
#include <cmath>


#ifndef nullptr
#define nullptr 0
#endif

#include "Sort.h"
#include "Point.h"
#include "GrahamScan.h"

// A utility function to find next to top in a stack
static Point* nextToTop(std::stack<Point*>& S)
{
    Point* p = S.top();
    S.pop();
    Point* res = S.top();
    S.push(p);
    return res;
}

///////////////////////////////////////////////////////////////////////////////
// To find orientation of ordered triplet (p, q, r).
PointOrientationType ComputePointOrientation(const Point* p, const Point* q, const Point* r)
{
    static const double eps = 1e-6;
    // int val = (q.y - p.y) * (r.x - q.x) - (q.x - p.x) * (r.y - q.y);
    const Coord val = (q->y() - p->y()) * (r->x() - q->x()) -
                      (q->x() - p->x()) * (r->y() - q->y());

    return (fabs(val) <= eps)
                ?   PointOrientationColinear
                :   (val > eps
                        ?   PointOrientationClockwise
                        :   PointOrientationConterClockWise);
}

///////////////////////////////////////////////////////////////////////////////

template<class T>
class ComparePoints : public SortCompare<T>
{
public:

    ComparePoints(Point* p0)
        : m_p0(p0)
    {
    }
    bool operator()(const T p1, const T p2)const
    {
        //std::cout << p1 << " " << p2 << "\n";
        assert(m_p0);
        if(!p1) return false;
        if(!p2) return false;

        assert(p1);
        assert(p2);
        bool ret = true;
        const int o = ComputePointOrientation(m_p0, p1, p2);
        switch(o)
        {
            case 0: // p, q, r : colinear
                ret = (m_p0->squareDistanceTo(*p2) >= m_p0->squareDistanceTo(*p1))
                        ?  false
                        :  true;
                break;
            case 1: // clockwise
                ret = true;
                break;
            case 2: // counterclockwise
                ret = false;
                break;
            default:
                assert(0);
                break;
        }
        return !ret;
    }
    Point* m_p0;
};

///////////////////////////////////////////////////////////////////////////////

static bool isEqual(double d1, double d2, double eps=1e-6)
{
    return (fabs(d2-d1) < eps) ? true : false;
}

///////////////////////////////////////////////////////////////////////////////
// Prints convex hull of a set of n points.
void computeConvexHull(const std::vector<Point*>& pointArray,
                       std::vector<Point*>&       curve,
                       const bool                 bVerbose)
{
    if(bVerbose)
    {
        fprintf(stdout, "computeConvexHull: nbPts: %ld\n", pointArray.size());
    }
    curve.resize(0);
    if(pointArray.size() < 3)
    {
        return;
    }

    std::vector<Point*> points;
    points.resize(pointArray.size());
    for(size_t i=0; i<points.size(); i++)
    {
        points[i] = pointArray[i];
    }
    const size_t n = points.size();
    // Find the bottommost point
    size_t min = 0;
    Coord  ymin = points[min]->y();
    Coord  xmin = points[min]->x();

    for(size_t i=1; i<n; i++)
    {
        const Coord x = points[i]->x();
        const Coord y = points[i]->y();
        // Pick the bottom-most or chose the left most point in case of tie
        if ((y<ymin) || (isEqual(y,ymin) && x<xmin))
        {
            min  = i;
            xmin = x;
            ymin = y;
        }
    }
    // Place the bottom-most point at first position
    //std::swap(points[0], points[min]);
    if(min != 0)
    {
        Point* p    = points[0];
        points[0]   = points[min];
        points[min] = p;
    }
    if(0 && bVerbose)
    {
        fprintf(stdout, "pivot point: %s, at pos:%ld\n", points[0]->toString().c_str(), min);
    }
    // Sort n-1 points with respect to the first point. A point p1 comes
    // before p2 in sorted ouput if p2 has larger polar angle (in
    // counterclockwise direction) than p1
    // qsort(&points[1], n-1, sizeof(Point), compare);
    ComparePoints<Point*> compareFunction(points[0]);

    // Note: for some unknown reason, the std::sort does not work on apple c++ compiler.
    //       the swap of pointers is not working properly!!!
    // std::sort(points.begin()+1, points.end(), compareFunction);
    // BubbleSort(points, 1, compareFunction);
    QuickSort(points, 1, points.size()-1, compareFunction);

    // print the point array
    if(0 && bVerbose)
    {
        fprintf(stdout, "** List of sorted points\n");
        for(size_t i=0; i<n; i++)
        {
            if(points[i])
            {
                fprintf(stdout, "[%03ld]: %s \n", i, points[i]->toString().c_str());
            }
            else
            {
                fprintf(stdout, "[%ld] null\n", i);
            }
        }
        fprintf(stdout, "\n\n");
    }

    // Create an empty stack and push first three points to it.
    std::stack<Point*> stack;
    
    stack.push(points[0]);
    stack.push(points[1]);
    stack.push(points[2]);
    
    // Process remaining n-3 points
    for(size_t i=3; i<n; i++)
    {
        // Keep removing top while the angle formed by points next-to-top,
        // top, and points[i] makes a non-left turn
        while(stack.size()>1 &&
              ComputePointOrientation(nextToTop(stack), stack.top(), points[i]) != PointOrientationConterClockWise)
        {
            stack.pop();
        }
        stack.push(points[i]);
    }
    // collect the curve
    curve.resize(0);
    while (!stack.empty())
    {
        curve.push_back(stack.top());
        stack.pop();
    }
}

///////////////////////////////////////////////////////////////////////////////

Point* newPoint(int x, int y)
{
    Point* p = new Point(0, 2);
    p->set(0, x);
    p->set(1, y);
    return p;
}


///////////////////////////////////////////////////////////////////////////////
// Driver program to test above functions
int testGrahamScan(const int argv, const char** argc)
{
    double points[] = { 0, 3,
                        1, 1,
                        2, 2,
                        4, 4,
                         0, 0,
                         1, 2,
                         3, 1,
                         3, 3};
    size_t n = sizeof(points)/sizeof(points[0]) / 2;
    
    std::vector<Point*> v;
    std::vector<Point*> curve;


    for(size_t i=0, k=0; i<n; i++, k += 2)
    {
        v.push_back(newPoint(points[k], points[k+1]));
    }
    fprintf(stdout, "start\n");
    computeConvexHull(v, curve, true);

    for(size_t i=0; i<curve.size(); i++)
    {
        fprintf(stdout, "[%04ld] %s\n", i, curve[i]->toString().c_str());
    }
    return 0;
}
