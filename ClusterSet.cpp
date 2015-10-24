#include <stdlib.h>
#include <fstream>
#include <string>
#include <string.h>
#include <stdlib.h>
#include <boost/foreach.hpp>
#include <boost/tokenizer.hpp>
#include <cmath>

#include "Util.h"
#include "Point.h"
#include "Util.h"
#include "ClusterFunctions.h"
#include "Random.h"
#include "ClusterSet.h"


////////////////////////////////////////////////////////////////////////////////
// Dump a Set of points
//
std::ostream& operator<<(std::ostream& os, const PointIdSet& sp)
{
    BOOST_FOREACH(const PointIdSet::value_type& pid, sp)
    {
        os << "pid=" << pid << " " ;
    }
    return os;
}

////////////////////////////////////////////////////////////////////////////////

std::ostream& operator<<(std::ostream& os, const ClustersToPoints& cp)
{
    ClusterId cid = 0;
    BOOST_FOREACH(ClustersToPoints::value_type set, cp)
    {
        os << "clusterid["  << cid << "]" << "=(" << set << ")" << std::endl; 
        cid++;
    }
    return os;
}

////////////////////////////////////////////////////////////////////////////////

std::ostream& operator <<(std::ostream& os, const PointsToClusters& pc)
{
    PointId pid = 0;
    BOOST_FOREACH(PointsToClusters::value_type cid, pc)
    {
        std::cout << "pid[" << pid << "]=" << cid << std::endl;      
        ++pid;
    }
    return os;
}

////////////////////////////////////////////////////////////////////////////////

std::ostream& operator<<(std::ostream& os, const ClusterSet& cp)
{
    ClusterId cid = 0;
    BOOST_FOREACH(const ClustersToPoints::value_type& set, cp.clusterToPoints( ))
    {
        os << "Cluster[" << cid << "]=(";
        BOOST_FOREACH(PointIdSet::value_type pid, set)
        {
            const Point& p = cp.point(pid);
            os << "(" << p << ")";
        }
        os << ")" << std::endl;
        ++cid;
    }
    return os;
}
    
///////////////////////////////////////////////////////////////////////////////

ClusterSet::ClusterSet(const DataSet& ds,
                       size_t         nbCluster,
                       PointIdVector* pointIdVector)
    :   m_ds(ds)
    ,   m_nb_cluster(nbCluster)
    ,   m_pointsToCluster(ds.size(), 0)
{
    init(pointIdVector);
}

////////////////////////////////////////////////////////////////////////////////

ClusterSet::ClusterSet(const ClusterSet&   c,
                       size_t              nbCluster,
                       PointIdVector*      pointIdVector)
    :   m_ds(c.m_ds)
    ,   m_nb_cluster(nbCluster)
    ,   m_pointsToCluster(m_ds.size(), 0)
{
    init(pointIdVector);
}

////////////////////////////////////////////////////////////////////////////////

bool ClusterSet::init(PointIdVector* pointIdVector)
{
    bool bOk = true;
    if(m_ds.size()==0)
    {
        fprintf(stdout, "Error: data set not initialized. (zerpo points).\n");
        bOk = false;
    }
    else
    {
        if(pointIdVector)
        {
            const size_t iNbPoint = pointIdVector->size();

            m_pointIdVector.reserve(iNbPoint);
            m_pointIdVector.resize(0);
            for(size_t i=0; i<iNbPoint; i++)
            {
                m_pointIdVector.push_back((*pointIdVector)[i]);
            }
        }
        else
        {
            const size_t iNbPoint = m_ds.size();

            m_pointIdVector.reserve(iNbPoint);
            m_pointIdVector.resize(0);
            for(size_t i=0; i<iNbPoint; i++)
            {
                m_pointIdVector.push_back(m_ds[i].getId());
            }
        }
        //take the dimension as the size of the 1st point
        size_t dim = point(0).dim();

        for(size_t i=0; i<nbCluster(); i++)
        {
            // init centroids
            m_centroidVector.push_back(new Point(i, dim));

            // init cid -> set of points
            PointIdSet set_of_points;
            m_clustersToPoints.push_back(set_of_points);
         }
    }
    return bOk;
}

////////////////////////////////////////////////////////////////////////////////
// set the coordenates to all centroids
void ClusterSet::zero_centroids( )
{
    BOOST_FOREACH(CentroidVector::value_type& point, m_centroidVector)
    {
        point->clear();
    }
}
    
////////////////////////////////////////////////////////////////////////////////
    
void ClusterSet::printClusters(std::string fname)const
{
    FILE* f = fopen(fname.c_str(), "wt");
    for(ClusterId cid=0; cid<nbCluster(); cid++)
    {
        BOOST_FOREACH(const PointIdSet::value_type pid, pointsInCluster(cid))
        {
            const Point& p = point(pid);
        
            fprintf(f, "%s %u\n", p.toString().c_str(), cid);
        }
    }
    fclose(f);
}

////////////////////////////////////////////////////////////////////////////////

void ClusterSet::printCentroid(std::string fname)const
{
    FILE* f = fopen(fname.c_str(), "wt");
    
    for(ClusterId cid=0; cid<nbCluster(); cid++)
    {
        const Point& centroid = getCentroid(cid);
        fprintf(f, "%s %u\n", centroid.toString().c_str(), cid);
    }
    fclose(f);
}

////////////////////////////////////////////////////////////////////////////////

void ClusterSet::addPointToCluster(const Point&     pt,
                                   const ClusterId  cid)
{
    m_pointsToCluster[pt.getId().value()] = cid;
    m_clustersToPoints[cid].insert(pt.getId().value());
}

////////////////////////////////////////////////////////////////////////////////
/// \brief ClusterSet::addPointToCluster
/// \param pidset
/// \param cid
///
void ClusterSet::addPointToCluster(const PointIdSet& pidset,
                                   const ClusterId cid)
{

    for(PointIdSet::iterator it=pidset.begin(); it!=pidset.end(); it++)
    {
        m_pointsToCluster[it->value()] = cid;
        m_clustersToPoints[cid].insert(it->value());
    }
}

////////////////////////////////////////////////////////////////////////////////
// remove point from its current cluster
void ClusterSet::removePointFromCluster(const Point& pt)
{
    ClusterId cid = clusterContainingPoint(pt.getId());
    m_clustersToPoints[cid].erase(pt.getId());
}

////////////////////////////////////////////////////////////////////////////////

ClusterId ClusterSet::clusterContainingPoint (const PointId& pid)const
{
    return m_pointsToCluster[pid.value()];
}

////////////////////////////////////////////////////////////////////////////////
// the set of pointId associated to each cluster.
const PointIdSet& ClusterSet::pointsInCluster(const ClusterId cid)const
{
    return m_clustersToPoints[cid];
}

////////////////////////////////////////////////////////////////////////////////
// the set of pointId associated to each cluster.

PointIdSet& ClusterSet::pointsInCluster(const ClusterId cid)
{
    return m_clustersToPoints[cid];
}

////////////////////////////////////////////////////////////////////////////////
// number of points in a given cluster.
size_t ClusterSet::clusterSize(const ClusterId cid)const
{
    return m_clustersToPoints[cid].size();
}

////////////////////////////////////////////////////////////////////////////////
//
// Compute Centroids
void ClusterSet::compute_centroids( )
{
    // For ech centroid
    for(ClusterId cid=0; cid<nbCluster(); cid++)
    {
        computeCentroids(m_ds, pointsInCluster(cid), getCentroid(cid));
    }
}
    
////////////////////////////////////////////////////////////////////////////////

Point ClusterSet::getClusterCenter(const ClusterId cid)const
{
    Point center(0, m_ds[0].dim());

    const PointIdSet& pointIdSet = pointsInCluster(cid);
    if(pointIdSet.size() != 0)
    {
        // For earch PointId in this set
        BOOST_FOREACH(const PointIdSet::value_type& pid, pointIdSet)
        {
            const Point& p = point(pid);
            center += p;
        }
        center /= (double)clusterSize(cid);
    }
    return center;
}
    
////////////////////////////////////////////////////////////////////////////////
// Initial partition points among available clusters
// divide the points equally on all the clusters
//
/*
Forgy Method:
    * Randomly chooses k observations from the data set and uses these
      as the initial means.

    * Tends to spread the initial means out
    * Perferable for expectation maximization and standard k-means algorithm;


Random Partition:
    * Randomly assigns a cluster to each observation and then proceeds
      to the update step, thus computing the initial mean to be the centroid
      of the cluster's randomly assigned points.

    * Places all means of them close to the center of the data set.
    * Preferable for algorithms such as the k-harmonic means and fuzzy k-means.
*/

void ClusterSet::initial_partition_points(InitMethod method)
{
    if(method == RandomPartition)
    {
        int nbCluster = m_nb_cluster;
        
        // Clear the vector
        for(size_t idx=0; idx<nbPoints(); idx++)
        {
            ClusterId cid = intRandomValue(nbCluster);
            addPointToCluster(point(idx), cid);
        }
    }
    else
    {
        // select the centroid, by randomly piking on the data set population
        const size_t nb = nbPoints( );
        std::set<size_t> points;
        
        for(ClusterId cid=0; cid<nbCluster(); cid++)
        {
            size_t p = intRandomValue(nb);
            while(points.count(p) != 0)
            {
                p = intRandomValue(nb);
            }
            points.insert(p);
            
            getCentroid(cid) = point(p);
            std::cout << "adding centroid at point " << p <<  " " << nb << std::endl;
        }
    
        // assign the points to the closer centroids.....
        for(size_t pid=0; pid<nb; pid++)
        {
            const Point& p = point(pid);
            
            // find closer centroid
            Distance min_dist = 1e38;
            ClusterId closestCentroid = 0;
            for(ClusterId cid=0; cid<nbCluster(); cid++)
            {
                Distance dist = p.distanceTo(getCentroid(cid));
                
                if(dist < min_dist)
                {
                    closestCentroid = cid;
                    min_dist = dist;
                }
            }
            std::cout << "add " << pid << " " << closestCentroid << std::endl;
            addPointToCluster(p, closestCentroid);
        }
    }
}

    
///////////////////////////////////////////////////////////////////////////////
//
// k-means
//


