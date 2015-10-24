#include <iostream>
#include <sstream>
#include <cmath>
#include <memory>

#include "ClusterFunctions.h"
#include "ClusterSet.h"
#include "DataSetUtil.h"

#include "KMeanTest.h"


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
                   const double      clusterSize)
{
    DataSet ds;
    std::vector<Point*> ptList;
    createRandomDataSet(ptList,
                        iNbPoints,
                        iNbNoisePoints,
                        iPointDim,
                        nbCluster,
                        minVal,
                        maxVal,
                        clusterSize,
                        clusterType,
                        true);

    std::random_shuffle( ptList.begin(), ptList.end() );
    for(size_t i=0; i<ptList.size(); i++)
    {
        ds.addPoint(ptList[i]);
    }
    ds.write(dataset_fname);
}

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
                   const bool           bVerbose)
{
    ClusterSet cs(ds, iNbCluster);
    computeKMeans(cs);
    for(ClusterId cid=0; cid<cs.nbCluster(); cid++)
    {
        std::vector<Point*> curve;

        std::string curveName = clusterName + ".region." + toString(cid) + ".txt";
        computeClusterBondary(cs, cid, curve, curveName, bVerbose);
    }
    // writes the pointid list for all clusters.
    for(ClusterId cid=0; cid<cs.nbCluster(); cid++)
    {
        std::string fname = "clusterPid" + toString(cid) + ".txt";
        writeClusterPointIdFile(ds, cs.pointsInCluster(cid), cs.getCentroid(cid), fname, bVerbose);
    }
    clustersCreatePlots(cs, clusterName, iNbCluster);
}

///////////////////////////////////////////////////////////////////////////////
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
                   const bool           bVerbose)
{
    std::auto_ptr<ClusterSet> pcs(createSubCluster(ds, clusterPidFname, iNbCluster));
    ClusterSet& cs = *pcs;

    computeKMeans(cs);

    for(ClusterId cid=0; cid<cs.nbCluster(); cid++)
    {
        std::vector<Point*> curve;

        std::string curveName = clusterName + ".region." + toString(cid) + ".txt";
        computeClusterBondary(cs, cid, curve, curveName, bVerbose);
    }

    // writes the pointid list for all clusters.
    for(ClusterId cid=0; cid<cs.nbCluster(); cid++)
    {
        std::string fname = clusterName + ".pid." + toString(cid) + ".txt";
        writeClusterPointIdFile(ds, cs.pointsInCluster(cid), cs.getCentroid(cid), fname, bVerbose);
    }
    clustersCreatePlots(cs, clusterName, iNbCluster);
}


////////////////////////////////////////////////////////////////////////////////

int KMeanTest(const int argc, const char** argv)
{
    fprintf(stdout, "begin: testClusterSet\n");

    std::string dataset_fname = "dataset.txt";
    std::string clusterFile1  = "cluster1";
    std::string clusterFile2  = "cluster2";

    if(1)
    {
        size_t iNbPoints = 1000;
        size_t iNbNoisePoints = 0;
        double minVal = -10.0;
        double maxVal =  10.0;
        double clusterSize = 1;
        
        size_t iNbCluster = 4;
        size_t iPointDim = 2;
        
        createDataSet(dataset_fname,
                      iNbPoints,
                      iNbNoisePoints,
                      iPointDim,
                      iNbCluster,
                      CLUSTER_CIRCLE,
                      minVal,
                      maxVal,
                      clusterSize);

        fprintf(stdout, "# Creates a random data set for cluster analysis: \n");
        fprintf(stdout, "  data set filename: %s\n", dataset_fname.c_str());
        fprintf(stdout, "  nb_points:        %d\n",   iNbPoints);
        fprintf(stdout, "  range:            [%g, %g]\n",  minVal, maxVal);
        fprintf(stdout, "  cluster_size:     %g\n", clusterSize);
        fprintf(stdout, "  nb_cluster:       %d\n",   iNbCluster);
        fprintf(stdout, "  cluster_type:     circular\n");
        fprintf(stdout, "  nb noise points: %d\n", iNbNoisePoints);
        fprintf(stdout, "\n");
    }

    DataSet ds;
    if(1)
    {

        bool bVerbose = true;
        const int iNbCluster = 3;
        ds.read(dataset_fname);

        fprintf(stdout, "****************************************************************\n");
        fprintf(stdout, "** Compute KMeans clustring (iNbCluter: %d)\n", iNbCluster);
        fprintf(stdout, "   Generated cluster files (basename:%s)\n", clusterFile1.c_str());
        computeKMeans(ds, iNbCluster, clusterFile1, bVerbose);
    }
    bool bVerbose = true;
    if(1)
    {
        fprintf(stdout, "****************************************************************\n");
        fprintf(stdout, "** Picks ths 2nd cluster of c1, and create 2 sub-clusters.\n\n");
        fprintf(stdout, "   Generated cluster files (basename:%s)\n", clusterFile2.c_str());

        computeKMeans(ds,"clusterPid1.txt", 2, clusterFile2, bVerbose);
    }

    fprintf(stdout, "\n\nend.\n");
    return 0;
}

////////////////////////////////////////////////////////////////////////////////


#ifdef _CLUSTER_SET_MAIN_

int main(int argc, char* argv[])
{
    ClusterSetTest( );
}

#endif






