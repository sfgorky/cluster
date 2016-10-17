#include <iostream>
#include <sstream>
#include <cmath>
#include <memory>

#include "ClusterFunctions.h"
#include "ClusterSet.h"
#include "KMean.h"
#include "KMeanTest.h"


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
        fprintf(stdout, "  nb_points:        %ld\n",   iNbPoints);
        fprintf(stdout, "  range:            [%g, %g]\n",  minVal, maxVal);
        fprintf(stdout, "  cluster_size:     %g\n", clusterSize);
        fprintf(stdout, "  nb_cluster:       %ld\n",   iNbCluster);
        fprintf(stdout, "  cluster_type:     circular\n");
        fprintf(stdout, "  nb noise points: %ld\n", iNbNoisePoints);
        fprintf(stdout, "\n");
    }

    DataSet ds;
    size_t maxIter = 10;
        
    if(1)
    {

        bool bVerbose = true;
        const int iNbCluster = 3;
        ds.read(dataset_fname);

        fprintf(stdout, "****************************************************************\n");
        fprintf(stdout, "** Compute KMeans clustring (iNbCluter: %d)\n", iNbCluster);
        fprintf(stdout, "   Generated cluster files (basename:%s)\n", clusterFile1.c_str());
        computeKMeans(ds, iNbCluster, clusterFile1, maxIter, bVerbose);
    }
    bool bVerbose = true;
    if(1)
    {
        fprintf(stdout, "****************************************************************\n");
        fprintf(stdout, "** Picks ths 2nd cluster of c1, and create 2 sub-clusters.\n\n");
        fprintf(stdout, "   Generated cluster files (basename:%s)\n", clusterFile2.c_str());

        computeKMeans(ds,"clusterPid1.txt", 2, clusterFile2, maxIter , bVerbose);
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






