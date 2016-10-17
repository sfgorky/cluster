#include <iostream>
#include <cmath>

#include "Random.h"
#include "DataSetUtil.h"

std::string toString(const std::vector<double>& lst)
{
    std::string s;
    char buffer[1024];
    
    s += "(";
    for(size_t i=0; i<lst.size(); i++)
    {
        snprintf(buffer, sizeof(buffer), "%g ", lst[i]);
        s += buffer;
    }
    s += ")";
    return s;
}

////////////////////////////////////////////////////////////////////////////////
// Init collection of points
void initRandomPoints(DataSet&     ds,
                      const size_t iNbPoint,
                      const size_t pointDim)
{
    std::cout << "Adding random points: nb: " << iNbPoint <<  std::endl;
    
    for(size_t i=0; i<iNbPoint; i++)
    {
        Point::CoordVector coordVector;
        for(size_t d=0; d<pointDim; d++)
        {
            coordVector.push_back( rand() % 100 );
        }
        ds.addPoint(new Point(i, coordVector));
        // std::cout << "pid[" << i << "]= (" << p << ")" <<std::endl;
    }
}

////////////////////////////////////////////////////////////////////////////////
// Init collection of points
// clusterSize: 0: square, 1: circle
//
void createRandomDataSet(std::vector<Point*>& pointList,
                         const size_t iNbPoint,
                         const size_t iNbNoisePoints,
                         const size_t pointDim,
                         const size_t iNbCentroid,
                         const double min_val,
                         const double max_val,
                         const double cluster_size,
                         const int    clusterType,
                         const bool   bVerbose)
{
    if(bVerbose)
    {
        std::cout << "* Creating random data set"     << std::endl;
        std::cout << "  nb_points:   " << iNbPoint    << std::endl;
        std::cout << "   nb_noise:   " << iNbNoisePoints << std::endl;
        std::cout << "  space range: " << "[ " << min_val << ", " << max_val << " ]" << std::endl;
        std::cout << "  nb_centroid: " << iNbCentroid << std::endl;
        if(iNbCentroid) std::cout << "  cluster_size " << cluster_size << std::endl;
        std::cout << std::endl;
    }
    Point::CoordVector coord(pointDim);
    std::vector<double> centroid;
    std::vector<double> dev;

    size_t pid = 0;
    for(size_t ipos=0; ipos<iNbNoisePoints; ipos++)
    {
       coord = getRandomVector(pointDim, min_val, max_val);
       pointList.push_back( new Point(pid++, coord));
    }

    if(iNbCentroid == 0)
    {
        for(size_t ipos=0; ipos<iNbPoint; ipos++)
        {
           coord = getRandomVector(pointDim, min_val, max_val);
           pointList.push_back(new Point(pid++, coord));
        }
    }
    else
    {
        if(bVerbose)
        {
            std::cout << "* Centroid: " << std::endl;
        }
        // pick random
        int iNbCentroidPt = (double)iNbPoint / (double)iNbCentroid;
        for(size_t c=0, ipos=0; c<iNbCentroid; c++)
        {
            // pick the centroid coordenates
            centroid = getRandomVector(pointDim, min_val+cluster_size, max_val-cluster_size);

            if(bVerbose)
            {
                char sBuffer[1024];
                for(size_t i=0; i<centroid.size(); i++)
                {
                    fprintf(stdout, "%s ", toString(centroid[i]).c_str());
                }
                fprintf(stdout, "\n");
            }
            // pick points around the centroid.
            switch(clusterType)
            {
                case CLUSTER_SQUARE:
                    for(int j=0; j<iNbCentroidPt; j++)
                    {
                        dev = getRandomVector(pointDim, -cluster_size, cluster_size);
                        for(size_t d=0; d<pointDim; d++)
                        {
                            coord[d] = centroid[d] + dev[d];
                        }
                        pointList.push_back(new Point(ipos++, coord));
                    }
                    break;
                case CLUSTER_CIRCLE:
                        if(pointDim==2)
                        {
                            for(int j=0; j<iNbCentroidPt; j++)
                            {
                                const double radix =  randomValue(0, cluster_size);
                                //const double radix =  fabs( randomNormal(0.0, cluster_size) );
                                const double angle  = randomValue(0, 360);

                                const double cx     = radix * cos(angle/180.0 * M_PI);
                                const double cy     = radix * sin(angle/180.0 * M_PI);

                                coord[0] = centroid[0] + cx;
                                coord[1] = centroid[1] + cy;
                                pointList.push_back(new Point(pid++, coord));
                            }
                       }
                       break;
            }
        }
    }
}

