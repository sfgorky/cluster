#ifndef _computeDBSCAN_h_
#define _computeDBSCAN_h_

#include <string>
#include "Point.h"
#include "DataSet.h"


void computeDBSCAN(const DataSet&       ds,
                   const std::string    clusterPidFname,
                   const std::string    clusterName,
                   const size_t         minPts,
                   const double         eps,
                   const bool           bVerbose);


void compute_DBSCAN(const DataSet&              dbase,
                    const double                eps,
                    const size_t                minPts,
                    std::vector<PointIdSet >&   clusters,
                    PointIdSet&                 noise,
                    const bool                  bVerbose);


void DBScanTest(const int argv, const char** argc);


#endif
