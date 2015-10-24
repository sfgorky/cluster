#ifndef _ClusterSet_h_
#define _ClusterSet_h_

#include <stdlib.h>
#include <stdio.h>
#include <set>
#include <vector>
#include <iostream>

#include "DataSetUtil.h"
#include "Point.h"
#include "DataSet.h"

//http://codingplayground.blogspot.com/2009/03/k-means-in-c.html

// the id of a given cluster
typedef unsigned int ClusterId;


// Different initialization methods
enum InitMethod
{
        ForgyMethod
    ,   RandomPartition
};
    
// vector lf centroids centroids
typedef std::vector<Point*> CentroidVector;

// ClusterId -> (PointId, PointId, PointId, .... )
typedef std::vector<PointIdSet> ClustersToPoints;

// PointId -> ClusterId
typedef std::vector<ClusterId> PointsToClusters;

typedef std::vector<PointId> PointIdVector;

//  This class represents a cluster Set.
//
class ClusterSet
{
///////////////////////////////////////////////////////////////////////////////
    public:
///////////////////////////////////////////////////////////////////////////////

                // pointIdVector: Pointer to a vector of Ids. All the point Ids
                // on this vector must be represented in the current point space.
                ClusterSet        (const DataSet&    ds,
                                  size_t             nbCluster,
                                  PointIdVector*     pointIdVector=0)         ;
    
                 ClusterSet       (const ClusterSet&  c,
                                  size_t              nbCluster,
                                  PointIdVector*      pointIdVector=0)         ;

    ////////////////////////////////////////////////////////////////////////////
    // Access functions
    
    const ClustersToPoints&     clusterToPoints         ( )              const
    { return m_clustersToPoints; }
   
    /// \brief addPointToCluster Adds a pointId to a given cluster. the Point
    ///                          will not be removed from its original cluster.
    /// \param point   Point object to be removed
    /// \param cluster  ClusterId of the cluster to be inserted.
    void                       addPointToCluster      (const Point&    pt,
                                                       const ClusterId cid)  ;

    void                       addPointToCluster      (const PointIdSet& pidset,
                                                       const ClusterId cid)  ;

    
    /// \brief removePointFromCluster remove a given point of its own cluster.
    ///                               it is assumed that the point is located in
    ///                               a single cluster.
    /// \param pt Point to be removed.
    void                       removePointFromCluster  (const Point& pt)   ;

    
    // the clusterId associated to this point
    ///
    /// \brief clusterContainingPoint returns the cluster Id associated to a given
    ///                               point
    /// \param pointId point id associated to cluster
    /// \return ClusterId
    ///
    ClusterId                  clusterContainingPoint (const PointId& pid)const;

    ///
    /// \brief pointsInCluster The set of pointId associated to each cluseter
    /// \param cid ClusterId
    /// \return set<PointId> associated to cluser (const)
    const PointIdSet&          pointsInCluster        (const ClusterId cid) const ;

    ///
    /// \brief pointsInCluster The set of pointId associated to each cluseter
    /// \param cid ClusterId
    /// \return set<PointId> associated to cluser (non const)
    PointIdSet&                pointsInCluster        (const ClusterId cid)        ;

    /// \brief clusterSize number of points i na given cluster
    /// \param cid ClusterId
    /// \return size_t
    size_t                     clusterSize            (const ClusterId cid)const ;
    
    /// \brief nbPoints otal number of points in the current data base
    /// \return size
    ///
    size_t                     nbPoints               ( )                 const
    { return m_pointIdVector.size(); }

    /// \brief nbCluster total number of clusters
    /// \return size_t
    size_t                     nbCluster              ( )                const
    { return m_nb_cluster; }
    
    // a given point object.
    // @param[in] index  the index, on the current list of points (not a poindId).
    //                   if the list of points on the sub-data set is smaller
    //                   than the total number of points
    //                   point(i).getId() != ds.point(i).getId()

    ///
    /// \brief point  A given point object in the cluster set
    /// \param index  the index, on the current list of points (not a poindId).
    ///               if the list of points on the sub-data set is smaller
    ///               than the total number of points
    ///                point(i).getId() != ds.point(i).getId()
    /// \return Point object
    ///
    const Point&               point              (const size_t idx)       const
    { return m_ds[m_pointIdVector[idx].value()]; }

    const Point&               point                  (const PointId& pid) const
    { return m_ds[pid.value()]; }

    // the centroid point associated to point 'pt'
    const Point&               getCentroidOfPoint     (const PointId& pid) const
    { return *m_centroidVector[clusterContainingPoint(pid)]; }
    
    // the centroid of the 'given cluster'
    const Point&              getCentroid           (const ClusterId cid) const
    { return *m_centroidVector[cid]; }

    Point&                    getCentroid           (const ClusterId cid)
    { return *m_centroidVector[cid]; }
    
    // the center of mass of the cluster
    Point                     getClusterCenter      (ClusterId cid)      const ;
    
    void                      printClusters         (std::string fname)  const ;
    void                      printCentroid         (std::string fname)  const ;
    
    const DataSet&            dataSet               ( )                  const
    { return m_ds; }

    ////////////////////////////////////////////////////////////////////////////
    // processing functions
    
    // initialize all centroids to a zero position
    void                zero_centroids              ( )                       ;
    
    // Zero centroids
    void                compute_centroids           ( )                       ;
    
    // Initial partition points among available clusters
    void                initial_partition_points    (InitMethod method)        ;
    
///////////////////////////////////////////////////////////////////////////////
    private:
///////////////////////////////////////////////////////////////////////////////

    bool                init                   (PointIdVector* pointIdVector) ;

    // the point space - store are a reference
    const DataSet&              m_ds                                          ;

    // number of clusters
    size_t                      m_nb_cluster                                  ;
    
    // the list of point id on this cluster
    PointIdVector               m_pointIdVector                               ;
    
    // given a cluster, the list of its points
    ClustersToPoints            m_clustersToPoints                            ;

    // given any point, which clusterId it belongs
    PointsToClusters            m_pointsToCluster                             ;

    CentroidVector              m_centroidVector                              ;
};

void initRandomPoints(DataSet& dataSet, const size_t iNbPoints);
    
//
// distance between two points
//
    

std::ostream& operator<< (std::ostream& os, const CentroidVector& cp);
    
std::ostream& operator<< (std::ostream& os, const ClustersToPoints& cp);
std::ostream& operator<< (std::ostream& os, const PointsToClusters& pc);
std::ostream& operator<< (std::ostream& os, const ClusterSet& cp);
std::ostream& operator<< (std::ostream& os, const DataSet& sp);
 


int testClusterSet(const int argc, const char** argv);


#endif
