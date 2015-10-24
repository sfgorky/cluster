#ifndef _DataSetUtil_h_
#define _DataSetUtil_h_

#include <iostream>
#include <sstream>
#include <string>
#include <stdlib.h>
#include "DataSet.h"

template<typename T>
std::string toString(const T value)
{
    std::ostringstream oss;
    oss << value;
    return oss.str();
}


#define CLUSTER_SQUARE 0
#define CLUSTER_CIRCLE 1
///
/// \brief createRandomDataSet Creates a random data set, to be used on testing.
/// \param pointList  List of points to be created
/// \param iNbPoint total number of points in the cluster space
/// \param iNbNoise number of noisy points, added on the entire space.
/// \param pointDim the number of coordenates on each cluster
/// \param iNbCentroid  total number of clusters. If zero, the data will be spaced equally
/// \param min_val  min value of the point space
/// \param max_val  max value f the point space
/// \param cluster_size the size of each cluser
/// \param clusterType  CLUSTER_SQUARE, CLUSTER_CIRCLE. (Circle is only valid of pointDim==2)
/// \param bVerbose if true, some information will be printed to stdout
///
void createRandomDataSet(std::vector<Point*>& pointList,
                         const size_t iNbPoint,
                         const size_t iNbNoise,
                         const size_t pointDim,
                         const size_t iNbCentroid,
                         double       min_val,
                         double       max_val,
                         double       cluster_size,
                         int          clusterType,
                         bool         bVerbose);


#endif
