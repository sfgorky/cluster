#ifndef _ClusterFunctions_h_
#define _ClusterFunctions_h_

#include <vector>

#include "ClusterSet.h"

///
/// \brief createSubCluster Creates a sub-clusterset, from a given clusterID.
///                         Can be use to re-apply cluster algorithms to a
///                         given cluster. Only the point IDs will be copied.
/// \param from ClusterSet main object - it will inherit the data set.
/// \param cid  CluseterId to be copyied from
/// \param nbCluster number to be allocated in the new object/
/// \return a new ClusterSet object. The client is reponsible for deleting
///               the new created object.
///
ClusterSet* createSubCluster(const ClusterSet& from,
                             const ClusterId   cid,
                             const size_t      nbCluster);

///
/// \brief createSubCluster Creates a new cluser object, based on the DataSet
///                         data. The file 'clusterFile' will contain the index
///                         of the points in the data set to consider
/// \param ds DataSet to use
/// \param clusterFile a file with the point index on the dataset to consider
/// \param nbCluster number of points to consider
/// \return a new ClusterSet object. The client is reponsible for deleting
///               the new created object.
///
ClusterSet* createSubCluster(const DataSet&     ds,
                             const std::string  pointIdFile,
                             const size_t       nbCluster);

///
/// \brief clustersCreatePlots
/// \param c
/// \param head
///
void clustersCreatePlots(const ClusterSet& c,
                         const std::string head,
                         const size_t      nbRegion=0);

///
/// \brief writeClusterPointIdFile Writes to a file the pointId list of a given cluster
///                                can be used to recreate a new ClusterSet
/// \param ds DataSet
/// \param pointSet  points of the cluster
/// \param centroid centroid
/// \param fname the filename to be created
///
void writeClusterPointIdFile(const DataSet&    ds,
                             const PointIdSet& pointSet,
                             const Point&      centroid,
                             const std::string fname,
                             bool              bVerbose);

///
/// \brief computeClusterRange Compute the range [min,max]
///                            of each dimension on a set of points
/// \param ds DataSet containing the points
/// \param pts Set of point-id on the dataset
/// \param minCoord
/// \param maxCoord
///
void computeClusterRange(const DataSet&      ds,
                         const PointIdSet&   pts,
                         std::vector<Coord>& minCoord,
                         std::vector<Coord>& maxCoord);


///
/// \brief computeCentroids Computes the centroid of a set of points
/// \param db the data base object containing the points
/// \param pts The set containing the index of the points in the data base
/// \param centroid the computed centroid.
///
void computeCentroids(const DataSet&    db,
                      const PointIdSet& pts,
                      Point&            centroid);
///
/// \brief computeClusterBondary Computes the bondary of a given cluster.
/// \param cs   ClusterSet the the cluster dta
/// \param cid  a given cluster id
/// \param curve  the returned curve, that constitue the bondary.
/// \param curveFileName the name of a file to dump the curve. If given.
///
void computeClusterBondary(const ClusterSet&    cs,
                           const ClusterId      cid,
                           std::vector<Point*>& curve,
                           const std::string    curveFileName,
                           const bool           bVerbose);

///
/// \brief computeClusterRadius
/// \param cs
/// \param cid
/// \return
///
DistanceType computeClusterRadius(const ClusterSet& cs,
                                  const ClusterId  cid);


// Computes the average distance from point pid, to all elements on cluster cid

///
/// \brief computeAverageDistance
/// \param cs
/// \param pid
/// \param cid
/// \return
///
DistanceType computeAverageDistance(const ClusterSet& cs,
                                    const PointId&    pid,
                                    const ClusterId   cid);

///
/// \brief computeSilhouette
/// \param cs
/// \param pid
/// \return
///
double computeSilhouette(const ClusterSet& cs,
                         const PointId& pid);

///
/// \brief computeAverageSilouette
/// \param cs
/// \param cid
/// \return
///
double computeAverageSilouette(const ClusterSet& cs,
                               const ClusterId cid);

///
/// \brief computeEnergy Computes the total energy of a cluster of points
/// \param ds Data Set containing all the pointsr
/// \param clusterSet the set of point index (in the dataset), respresenting a cluster
/// \param centroid The cluster centroid
/// \return The total SSE energy of this cluster
///
double computeEnergy(const DataSet&    ds,
                     const PointIdSet& clusterSet,
                     const Point&      centroid);

///
/// \brief computeKMeans
/// \param c
///
void computeKMeans(ClusterSet& c, const size_t nbIter, const bool printIter);


void printClusterSynopsis(const ClusterSet& cs);


#endif
