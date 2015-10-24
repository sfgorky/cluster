#ifndef _GrahamScan_h_
#define _GrahamScan_h_

#include <vector>

class Point;


enum PointOrientationType
{
        PointOrientationColinear
    ,   PointOrientationClockwise
    ,   PointOrientationConterClockWise
};

// To find orientation of ordered triplet (p, q, r).
PointOrientationType ComputePointOrientation(const Point* p, const Point* q, const Point* r);

void computeConvexHull(const std::vector<Point*>& pointArray,
                       std::vector<Point*>&       curve,
                       const bool                 bVerbose=true);


int testGrahamScan(const int argv, const char** argc);


#endif
