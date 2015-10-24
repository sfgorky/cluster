/* DBSCAN - density-based spatial clustering of applications with noise */

#include <algorithm>
#include <vector>
#include <cmath>

#include "Point.h"
#include "DataSet.h"
#include "DataSetUtil.h"
#include "ClusterFunctions.h"

///////////////////////////////////////////////////////////////////////////////

double distance(const Point& p1, const Point& p2)
{
    double dist = sqrt(pow((p1.x() - p2.x()), 2) +
                       pow((p1.y() - p2.y()), 2) );
    return dist;
}

///////////////////////////////////////////////////////////////////////////////

// pick the elements from the data base that are close to keypoint.
void findNeighborPoints(const DataSet&       dbase,
                        const size_t         pointId,
                       double                eps,
                       std::vector<size_t>&  queryRegion)
{
    queryRegion.resize(0);
    double maxdist = 0.0;
    double mindist = 1e38;

    for(size_t i=0; i< dbase.size(); i++)
    {
        if(pointId == i) continue;
        const double dist = distance(dbase[i], dbase[pointId]);

        maxdist = std::max(maxdist, dist);
        mindist = std::min(mindist, dist);

        //fprintf(stdout, "%g\n", dist);
        if(dist>0 && dist <= eps)
        {
            queryRegion.push_back(i);
        }
    }
    //fprintf(stdout, "poin:%ld, queryRegion size: %ld (eps:%g) dist:[%g, %g]\n",
    //       pointId, queryRegion.size(), eps, maxdist, mindist);
}

///////////////////////////////////////////////////////////////////////////////

void compute_DBSCAN(const DataSet&              dbase,
                    const double                eps,
                    const size_t                minPts,
                    std::vector<PointIdSet >&   clusters,
                    PointIdSet&                 noise,
                    bool                        bVerbose)
{
    if(bVerbose)
    {
        fprintf(stdout, "** Computing DBSCAN.(minPts:%ld, eps:%g, nbPts:%ld)\n", 
                    minPts, 
                    eps, 
                    dbase.size());
    }

    std::vector<size_t>              neighborPts;
    std::vector<size_t>              neighborPts_;

    const size_t nbPoints = dbase.size();

    // true if point is visited
    std::vector<bool> visited(nbPoints, false);

    // true if point is in a cluster
    std::vector<bool> in_cluster(nbPoints, false);

    // starts with an empty cluster
    clusters.resize(0);

    //for each un-visted point P in dataset dbase
    for(size_t i=0; i<nbPoints; i++)
    {
        if(visited[i]) continue;
        visited[i] = true;

        findNeighborPoints(dbase, i, eps, neighborPts);
        if(neighborPts.size() < minPts)
        {
            // Mark P noise, since there is less then minPts in the neighbourhood.
            noise.insert(dbase[i].getId());
        }
        else
        {
            // creates a new cluster
            clusters.push_back(PointIdSet());

            // current cluster id
            const size_t cid = clusters.size()-1;

            //expand cluster: add P to cluster c
            clusters[cid].insert(dbase[i].getId());

            // for each point P' in neighborPts
            for(size_t j=0; j<neighborPts.size(); j++)
            {
                const size_t neighbour_j = neighborPts[j];

                //if P' is not visited
                if(!visited[neighbour_j])
                {
                    //Mark P' as visited
                    visited[neighbour_j] = true;

                    findNeighborPoints(dbase, neighbour_j, eps, neighborPts_);
                    if(neighborPts_.size() >= minPts)
                    {
                        neighborPts.insert(neighborPts.end(), neighborPts_.begin(), neighborPts_.end());
                    }
                }
                // if P' is not yet a member of any cluster, add P' to cluster c
                if(!in_cluster[neighbour_j])
                {
                    clusters[cid].insert(dbase[neighbour_j].getId());
                    //fprintf(stdout, "====> adding lcuster: %ld\n", cid);
                }
            }
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
#include "DataSet.h"
#include "DataSetUtil.h"
#include "ClusterSet.h"


// creates a sub-cluster, from the data on cluster cid
ClusterSet* createClusterSet(const DataSet&                 ds,
                             const std::vector<PointIdSet>& clusters,
                             const PointIdSet&              noise)
{
    const size_t nbCluster = clusters.size() + (noise.size()>0 ? 1 : 0);
    ClusterSet* cs = new ClusterSet(ds, nbCluster);
    for(size_t cid=0; cid<clusters.size(); cid++)
    {
        cs->addPointToCluster(clusters[cid], cid);
    }
    if(noise.size())
    {
       cs->addPointToCluster(noise, nbCluster-1);
    }
    return cs;
}


///////////////////////////////////////////////////////////////////////////////
///
/// \brief computeKMeans Computes the K-Mean from a given clusterPid file
/// \param ds   Data Set
/// \param iNbCluster   number of clusster
/// \param clusterName  cluster name, used to save data files
/// \param createRegionPlot string: if given a region file will be created.
///
void computeDBSCAN(const DataSet&       ds,
                   const std::string    clusterPidFname,
                   const std::string    clusterName,
                   const size_t         minPts,
                   const double         eps,
                   const bool           bVerbose)
{
    //ClusterSet cs(ds, iNbCluster);

    if(ds.size() == 0)
    {
        fprintf(stdout, "data set is empty. Cannot compute dbscan.\n");
        return;
    }

    std::vector<PointIdSet >    clusters;
    PointIdSet                  noise;

    compute_DBSCAN(ds, eps,  minPts, clusters, noise, bVerbose);

    fprintf(stdout, "* DBSCAN results....\n");
    fprintf(stdout, "* nb clusters: %ld\n", clusters.size());
    fprintf(stdout, "* eps:         %g\n",  eps);
    fprintf(stdout, "* minPts:      %ld\n", minPts);

    std::auto_ptr<ClusterSet> cs(createClusterSet(ds, clusters, noise));


    for(ClusterId cid=0; cid<cs->nbCluster(); cid++)
    {
        std::vector<Point*> curve;

        std::string curveName = clusterName + ".region." + toString(cid) + ".txt";
        computeClusterBondary(*cs, cid, curve, curveName, bVerbose);
    }

    // writes the pointid list for all clusters.
    for(ClusterId cid=0; cid<cs->nbCluster(); cid++)
    {
        std::string fname = clusterName + ".pid." + toString(cid) + ".txt";
        writeClusterPointIdFile(ds, cs->pointsInCluster(cid), cs->getCentroid(cid), fname, bVerbose);
    }
    clustersCreatePlots(*cs, clusterName, cs->nbCluster());

    std::cout << std::endl << std::endl;
    printClusterSynopsis(*cs);
}

///////////////////////////////////////////////////////////////////////////////

void DBScanTest(const int argv, const char** argc)
{
    const size_t iNbPoints      = 1000;
    const size_t pointDim       = 2;
    const size_t iNbCentroid    = 4;
    const double minValue       = -10.0;
    const double maxValue       =  10.0;
    const double clusterSize    =  0.4;
    const size_t iNbNoisePoints =  100;

    std::vector<Point*> ptList;
    createRandomDataSet(ptList,
                        iNbPoints,
                        iNbNoisePoints,
                        pointDim,
                        iNbCentroid,
                        minValue,
                        maxValue,
                        clusterSize,
                        CLUSTER_CIRCLE,
                        true);
    bool bVerbose = true;

    size_t minPts = 3;
    double eps    = 0.2;
    DataSet ds;
    ds.addPointList(ptList);
    computeDBSCAN(ds, "dbscan", "dbscan", minPts, eps, bVerbose);
}


//TBD:
//    * add gausian to the random generator!
//    * Adapt the measures to deal with the vector of vector
//    * implement homogeniety, v-meareud, adjusted random index, adjusted mutual informattion

#ifdef _ADD_DBSCAN_MAIN_
int main(const int argv, const char** argc)
{
    DBScanTest(argv, argc);
    return 0;
}
#endif

