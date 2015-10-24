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


#endif


