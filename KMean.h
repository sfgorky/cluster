#ifndef _KMean_h_
#define _KMean_h_

#include <string>
#include <vector>
#include "DataSet.h"

typedef std::pair<size_t, double> DistPair;

void KMean(const DataSet&          dbase,
          size_t                  k,
          std::vector<DistPair >& dist,
          std::string             fname = std::string(),
          bool                    bVerbose=false);



///
/// \brief computeKMeans Computes the K-Mean from a given clusterPid file
/// \param ds   Data Set
/// \param iNbCluster   number of clusster
/// \param clusterName  cluster name, used to save data files
/// \param createRegionPlot string: if given a region file will be created.
///
void computeKMeans(const DataSet&       ds,
                   const std::string    clusterPidFname,
                   const size_t         iNbCluster,
                   const std::string    clusterName,
                   const size_t         maxIter,
                   const bool           bVerbose);


///////////////////////////////////////////////////////////////////////////////
///
/// \brief computeKMeans Computes the K-Mean for a given dataset
/// \param ds   Data Set
/// \param iNbCluster   number of clusster
/// \param clusterName  cluster name, used to save data files
/// \param createRegionPlot string: if given a region file will be created.
///
void computeKMeans(const DataSet&       ds,
                   const size_t         iNbCluster,
                   const std::string    clusterName,
                   const size_t         maxIter,
                   const bool           bVerbose);


///////////////////////////////////////////////////////////////////////////////
///
/// \brief createDataSet     Creates a data set container with random points.
///                          Points are create in a cube of values [minVal, maxVal]
///                          The data are clustered with 'nbCluster', and it
///                          is created in a square around each (random) cluster center
///                          The size of cluseter is determined by the 'clusterSize'
/// \param dataset_fname     output filename
/// \param iNbPoints         Total Number of points
/// \param iPointDim         Points dimension.
/// \param minVal            Min value of the point space
/// \param maxVal            Max value of the point space
/// \param nbCluster         Number of clusters to create. if zero, the data will be
///                          randomly spaced
/// \param clusterSize       size of the cluster, in corrdenates
///
void createDataSet(const std::string dataset_fname,
                   const size_t      iNbPoints,
                   const size_t     iNbNoisePoints,
                   const size_t      iPointDim,
                   const size_t      nbCluster,
                   const int         clusterType,
                   const double      minVal,
                   const double      maxVal,
                   const double      clusterSize);
                   
#endif


