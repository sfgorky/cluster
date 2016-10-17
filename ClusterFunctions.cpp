#include <ostream>
#include <fstream>
#include <boost/foreach.hpp>
#include <cmath>

#include "GrahamScan.h"
#include "DataSet.h"
#include "Point.h"

#include "ClusterFunctions.h"

// Compute Centroids
void computeCentroids(const DataSet&    db,
                      const PointIdSet& pts,
                      Point&            centroid)
{
    centroid.clear( );
    size_t iNb = 0;
    for(PointIdSet::iterator it=pts.begin(); it!=pts.end(); it++)
    {
        const Point& pt = db[it->value()];
        centroid += pt;
        iNb++;
    }
    centroid /= (double)iNb;
}

////////////////////////////////////////////////////////////////////////////////

// creates a sub-cluster, from the data on cluster cid
ClusterSet* createSubCluster(const ClusterSet&  from,
                             const ClusterId    cid,
                             const size_t       nbCluster)
{
    ClusterSet* newClusters = 0;
    
    // collect the list of pointsId, related to the given cluster
    PointIdVector pointList;
    BOOST_FOREACH(const PointIdSet::value_type pid, from.pointsInCluster(cid))
    {
        pointList.push_back(from.point(pid).getId());
    }
    newClusters = new ClusterSet(from, nbCluster, &pointList);
    
    return newClusters;
}

////////////////////////////////////////////////////////////////////////////////
// creates a sub-cluster, from the data on cluster cid
ClusterSet* createSubCluster(const DataSet&     ds,
                             const std::string  pointIdFile,
                             const size_t       nbCluster)
{
    ClusterSet* newClusters = 0;
    
    // collect the list of pointsId, related to the given cluster
    PointIdVector pointIdVector;
    
    std::fstream file;
    file.open(pointIdFile.c_str(), std::ios::in);
    if(file.is_open())
    {
        std::string line;
        while(std::getline(file, line))
        {
            if(line[0] == '#') continue;
            pointIdVector.push_back( atoi( line.c_str() ) );
        }
    }
    newClusters = new ClusterSet(ds, nbCluster, &pointIdVector);
    return newClusters;
}

////////////////////////////////////////////////////////////////////////////////


void writeClusterPointIdFile(const DataSet&    ds,
                             const PointIdSet& pointSet,
                             const Point&      centroid,
                             const std::string fname,
                             bool              bVerbose)
{
    const bool addBracket(false);
    std::ofstream file;
    file.open(fname.c_str(), std::ios::out);
    if(file.is_open())
    {
        //file << std::string("# cluster:  ") << cid << std::endl;
        file << std::string("# centroid: ") << centroid.toString() << std::endl;
        file << std::string("# nbPoints: ") << pointSet.size() << std::endl;
        
        BOOST_FOREACH(const PointIdSet::value_type pid, pointSet)
        {
            file << ds[pid.value()].toString(addBracket) << std::endl;
        }
    }
}

////////////////////////////////////////////////////////////////////////////////

void clustersCreatePlots(const ClusterSet& cs,
                         const std::string head,
                         const size_t      nbRegion)
{
    // creates the gnu file
    std::string gnuplotfile     = head + ".gnuplot";
    std::string pt_file         = head + ".points.txt";
    std::string centroid_file   = head + ".centroid.txt";


    FILE* p = fopen(gnuplotfile.c_str(), "wt");
    if(p)
    {
        fprintf(p, "set palette defined ( 0 'blue', 1 'green', 2 'red', 3 'coral', 4 'brown', 5 'cyan', 6 'yellow', 10 'red' )\n");
        fprintf(p, "set grid\n");
        fprintf(p, "unset key\n");
        
        fprintf(p, "plot \\\n");

        fprintf(p, "     \"%s\" using 1:2:3 with points pt 6 ps 1  palette, \\\n", pt_file.c_str());

        for(size_t i=0; i<nbRegion; i++)
        {
            fprintf(p, "     \"%s.region.%ld.txt\" using 1:2 with lines palette, \\\n",
                    head.c_str(), i);
        }

        fprintf(p, "     \"%s\" using 1:2:3 with points pt 7 ps 2  palette \n", centroid_file.c_str());
        fprintf(p, "\n\n");

        fprintf(p, "pause -1");
        fclose(p);
    }
    cs.printClusters(pt_file);
    cs.printCentroid(centroid_file);
}

////////////////////////////////////////////////////////////////////////////////

DistanceType computeClusterRadius(const ClusterSet& cs,
                                  const ClusterId  cid)
{
    DistanceType maxDist(0);

    const Point& centroid = cs.getCentroid(cid);
    // For earch PointId in this set
    BOOST_FOREACH(const PointIdSet::value_type pid, cs.pointsInCluster(cid))
    {
        const Point& pt = cs.point(pid);
        DistanceType dist = centroid.distanceTo(pt);

        maxDist = std::max(maxDist, dist);
    }
    return maxDist;
}

////////////////////////////////////////////////////////////////////////////////

void computeClusterRange(const DataSet&      ds,
                         const PointIdSet&   pts,
                         std::vector<Coord>& minCoord,
                         std::vector<Coord>& maxCoord)
{
    if(pts.size()>0)
    {
        const Point& p0 = ds[pts.begin()->value()];
        size_t dim = p0.size();
        
        minCoord.resize(dim);
        maxCoord.resize(dim);
        
        // Compute the of each coordenate on the cluster
        for(size_t i=0; i<dim; i++)
        {
            minCoord[i] = p0[i];
            maxCoord[i] = p0[i];
        }
        // compute range
        for(PointIdSet::iterator it=pts.begin(); it!=pts.end(); it++)
        {
            const Point& pt = ds[it->value()];
            for(size_t i=0; i<dim; i++)
            {
                maxCoord[i] = std::max(maxCoord[i], pt[i]);
                minCoord[i] = std::min(minCoord[i], pt[i]);
            }
        }
    }
}

////////////////////////////////////////////////////////////////////////////////

void computeClusterBondary(const ClusterSet&    cs,
                           const ClusterId      cid,
                           std::vector<Point*>& curve,
                           const std::string    curveFileName,
                           bool                 bVerbose)
{
    if(bVerbose)
    {
        fprintf(stdout, "computeClusterBondary...\n");
    }
    const PointIdSet& pts = cs.pointsInCluster(cid);                            

    std::vector<Point*> cluster;

    for(PointIdSet::iterator it=pts.begin(); it!=pts.end(); it++)           
    {
        const Point& pt = cs.point(*it); 
        cluster.push_back((Point*)(&pt));
    }
    computeConvexHull(cluster, curve, bVerbose);

    if(bVerbose)
    {
        fprintf(stdout, "Compute Bondary: clusterId: %u, cluster:%ld, curve:%ld\n", cid, cluster.size(), curve.size());
    }
    if(!curveFileName.empty())                                                  
    { 
        const bool bPrintBracket = false;
        FILE* f = fopen(curveFileName.c_str(), "wt");                           
        if(f)
        {
            if(curve.size()>0)
            {
                for(size_t i=0; i<curve.size(); i++)
                {
                    fprintf(f, "%s\n", curve[i]->toString(bPrintBracket).c_str());
                }
                // adds the first point to make the curve closed
                fprintf(f, "%s\n", curve[0]->toString(bPrintBracket).c_str());
            }
            fclose(f);
        }
    }
}

///////////////////////////////////////////////////////////////////////////////

void xcomputeClusterBondary(const ClusterSet&    cs,
                            const ClusterId      cid,
                            std::vector<Point*>& curve,
                            const std::string    curveFileName)
{
    static const double BIGVALUE   =  1e38;
    static const double M_BIGVALUE = -1e38;
    
    size_t iGridSize = 10;
    const PointIdSet& pts = cs.pointsInCluster(cid);
    curve.resize(0);
    if(pts.size()>0)
    {
        std::vector<Coord> minCoord;
        std::vector<Coord> maxCoord;
        
        computeClusterRange(cs.dataSet(), pts,  minCoord, maxCoord);
        std::vector<double> x(iGridSize);
        
        // Range for the y values, at difference x
        std::vector<double> min_y(iGridSize);
        std::vector<double> max_y(iGridSize);
        
        double deltax = (maxCoord[0]-minCoord[0])/(double)(iGridSize-1);
        for(size_t j=0; j<iGridSize; j++)
        {
            x[j] = minCoord[0] + j * deltax;
            min_y[j] =   BIGVALUE;
            max_y[j] = M_BIGVALUE;
        }
        for(PointIdSet::iterator it=pts.begin(); it!=pts.end(); it++)
        {
            const Point& pt = cs.point(*it);
            size_t iGridPos = round( ((pt[0] - minCoord[0] ) / deltax) );
            
            min_y[iGridPos] = std::min( min_y[iGridPos], pt[1] );
            max_y[iGridPos] = std::max( max_y[iGridPos], pt[1] );
        }
        
        Point::CoordVector coord(2);
        size_t idx = 0;
        // lower part
        for(size_t i=0; i<iGridSize; i++, idx++)
        {
            if(min_y[i] != BIGVALUE)
            {
                coord[0] = x[i];
                coord[1] = min_y[i];
                
                curve.push_back(new Point(PointId(idx), coord));
            }
        }
        // upper part of the region
        for(int i=iGridSize-1; i>=0; i--, idx++)
        {
            if(max_y[i] != M_BIGVALUE)
            {
                coord[0] = x[i];
                coord[1] = max_y[i];
                
                curve.push_back(new Point(PointId(idx), coord));
            }
        }
        coord[0] = x[0];
        coord[1] = min_y[0];
        curve.push_back(new Point(PointId(idx), coord));
    }
    if(!curveFileName.empty())
    {
        FILE* f = fopen(curveFileName.c_str(), "wt");
        if(f)
        {
            for(size_t i=0; i<curve.size(); i++)
            {
                fprintf(f, "%s\n", curve[i]->toString().c_str());
            }
            fclose(f);
        }
    }
}

////////////////////////////////////////////////////////////////////////////////

double computeSilhouette(const ClusterSet& cs,
                         const PointId&  pid)
{
    // a(i): the avearate dissimilarity of i with all othe data within the same
    //       cluster. a(i) represents how well the point i is integreated to its
    //       cluster.
    
    const ClusterId cid = cs.clusterContainingPoint(pid);
    
    DistanceType a_i = computeAverageDistance(cs, pid, cid);
    
    DistanceType b_i = 1e9;
    for(size_t other_cid=0; other_cid<cs.nbCluster(); other_cid++)
    {
        if(other_cid != cid)
        {
            b_i = std::min(b_i, computeAverageDistance(cs, pid, other_cid));
        }
    }
    return (b_i - a_i) / std::max(a_i, b_i);
}

////////////////////////////////////////////////////////////////////////////////

double computeAverageSilouette(const ClusterSet& cs,
                               const ClusterId   cid)
{
    const PointIdSet&  pointsInSet = cs.pointsInCluster(cid);

    double s = 0.0;
    const size_t iNb = pointsInSet.size();
    for(PointIdSet::iterator it = pointsInSet.begin(); it != pointsInSet.end(); it++)
    {
        s += computeSilhouette(cs, *it) / (double)iNb;
    }
    return s;
}

////////////////////////////////////////////////////////////////////////////////

DistanceType computeAverageDistance(const ClusterSet& cs,
                                const PointId&   pid,
                                const ClusterId  to_cid)
{
    ClusterId pt_cid = cs.clusterContainingPoint(pid);
    
    bool sameCluster = (to_cid == pt_cid);
    DistanceType   dist = 0.0;
    size_t iNb  = 0;
    
    const PointIdSet&  pointsInSet = cs.pointsInCluster(to_cid);
    const Point&       p0          = cs.point(pid);
    for(PointIdSet::iterator it = pointsInSet.begin(); it != pointsInSet.end(); it++)
    {
        const Point& pt = cs.point(*it);
        bool add = sameCluster
            ?   pt.getId() != p0.getId()
            :   true;
        if(add)
        {
            dist += p0.distanceTo(pt);
            iNb  += 1;
        }
    }
    dist /= iNb;
    return dist;
    
}

////////////////////////////////////////////////////////////////////////////////

double computeEnergy(const DataSet&    ds,
                     const PointIdSet& clusterSet,
                     const Point&      centroid)
{
    DistanceType dist = 0.0;
    size_t iNb = 0;
    for(PointIdSet::iterator it = clusterSet.begin(); it != clusterSet.end(); it++)
    {
        const Point& pt = ds[it->value()];
        dist += pointDiffNorm(centroid, pt);
        iNb  += 1;
    }
    dist /= (double)iNb;
    return dist;
}

////////////////////////////////////////////////////////////////////////////////

void printClusterSynopsis(const ClusterSet& cs)
{
    std::cout << "* Clusters: "  << std::endl;
    std::cout << "nb_points:   " << cs.nbPoints() << std::endl;
    std::cout << "nb_clusters: " << cs.nbCluster() << std::endl;
    for(ClusterId cid=0; cid<cs.nbCluster(); cid++)
    {
        const Point& centroid = cs.getCentroid(cid);
        const size_t nbPoints = cs.clusterSize(cid);
        
        fprintf(stdout, 
            "cluster: %d, size: %4ld, centroid: %s, center: %s, radius: %10.2f, meanSilouette:%f\n",
                    cid, nbPoints, 
                    centroid.toString().c_str(), 
                    cs.getClusterCenter(cid).toString().c_str(), 
                    computeClusterRadius(cs, cid),
                    computeAverageSilouette(cs, cid));
    }
}

///////////////////////////////////////////////////////////////////////////////

void computeKMeans(ClusterSet& cs, const size_t maxIter, const bool bPrintIteration)
{
    bool bPrintSynodsis  = true;
   
    const size_t nbCluster = cs.nbCluster( );
    
    //
    // Initial partition of points
    //
    //    initial_partition_points(ForgyMethod);
    cs.initial_partition_points(RandomPartition);
    
    if(bPrintSynodsis)
    {
        std::cout << "*********************************************************" << std::endl;
        std::cout << "* K-Means begin"                                           << std::endl;
        std::cout << "*"                                                         << std::endl;
        printClusterSynopsis(cs);
    }
    
    size_t iter = 0;
    bool some_point_is_moving = true;
    
    // the K-Mean Loop
    while(some_point_is_moving)
    {
        some_point_is_moving = false;
        size_t nbMove  = 0;
        
        cs.compute_centroids( );
        if(bPrintIteration)
        {
            fprintf(stdout, "\n");
            fprintf(stdout, "*** Iterations %ld\n", iter);
            for(size_t cid=0; cid<nbCluster; cid++)
            {
                fprintf(stdout, "    * cluster [%ld] size: %4ld, radius: %10.2f, centroid: %s, center: %s\n", 
                        cid, cs.clusterSize(cid), computeClusterRadius(cs, cid), cs.getCentroid(cid).toString().c_str(), cs.getClusterCenter(cid).toString().c_str());
            }
        }
                   
        // for each point
        for(size_t pid=0; pid<cs.nbPoints(); pid++)
        {
            const Point&   p             = cs.point(pid);
            const Point&   p_centroid    = cs.getCentroidOfPoint(p.getId());
            bool           moveThisPoint = false;
            
            
            // distance point pid to its centroid
            const DistanceType distPointToCentroid = p_centroid.distanceTo(p);
            
            // fprintf(stdout, "pid: %ld %s distance to centroid %g\n", pid, p.toString().c_str(), distPointToCentroid);
            
            // foreach centroid, find the closest centroid
            DistanceType minDistance = distPointToCentroid;
            
            ClusterId to_cluster = 0;
    
            //std::cout << "****** " << pid << " " << p.getId() << "\n";
            
            for(size_t cid=0; cid<nbCluster; cid++)
            {
                const Point& centroid = cs.getCentroid(cid);
                
                //std::cout << " centroidID: " << centroid.getId() << " pCentroid: " << p_centroid.getId() << std::endl;
                if(centroid.getId() == p_centroid.getId()) continue;
                
                // distance from point to a centroid
                const DistanceType distPointToOtherCentroid = p.distanceTo(centroid);
                
                //std::cout << "pid: " << pid << ", cid: " << cid  << ", distToCent: " << distPointToCentroid << " " << distPointToOtherCentroid <<  " : "  << (distPointToOtherCentroid<distPointToCentroid) << "\n";
                if(distPointToOtherCentroid < distPointToCentroid)
                {
                    minDistance = distPointToOtherCentroid;
                    to_cluster = cid;
                    moveThisPoint = true;
                }
            }
            // move towards a closer centroid
            if(moveThisPoint>0)
            {
                // remove point from its current cluster
                cs.removePointFromCluster(p);
                if(bPrintIteration)
                {
                    // std::cout << " Moving " << nbMove << " points\n";
                }
                cs.addPointToCluster(p, to_cluster);
                some_point_is_moving = true;
                nbMove++;
                // std::cout << "\t\tmove to cluster:" << to_cluster << std::endl;
            }
        }
        if(bPrintIteration)
        {
            fprintf(stdout, "     > Moving points %ld", nbMove);
        }
        
        if(0 && bPrintIteration)
        {
            // print cluster table
            for(size_t cid=0; cid<nbCluster; cid++)
            {
                std::cout << cid << " ";
                BOOST_FOREACH(const PointId& id, cs.pointsInCluster(cid))
                {
                    std::cout << id << " ";
                }
                std::cout << std::endl;
            }
        }
        iter++;
        
        if(iter > maxIter) break;
    } // end while (some_point_is_moving)
    
    if(bPrintSynodsis)
    {
        std::cout << std::endl;
        std::cout << std::endl;
        std::cout << "K-Means: end: nb_iter: " << iter << std::endl;
        printClusterSynopsis(cs);
        std::cout << "*********************************************************" << std::endl;
    }

}

////////////////////////////////////////////////////////////////////////////////


