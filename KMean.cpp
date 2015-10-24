#include <cmath>
#include <algorithm>
#include "Sort.h"
#include "KMean.h"

template<class T>
class KNNCompare : public SortCompare<T>
{
///////////////////////////////////////////////////////////////////////////////
    public:
///////////////////////////////////////////////////////////////////////////////
   
     KNNCompare(const DataSet& dbase)
        :   m_dbase(dbase)
    { }

    virtual bool operator()(const T p1, const T p2)const
    {
        const Point p0(0, 2);
        const double d1 = m_dbase[p1].distanceTo(p0);
        const double d2 = m_dbase[p2].distanceTo(p0);
        return d1<d2;
    }
    const DataSet& m_dbase;
};

///////////////////////////////////////////////////////////////////////////////

class MyDistCompare
{
/////////////////////////////////////////////////////////////////////////////// 
    public:
/////////////////////////////////////////////////////////////////////////////// 

    virtual bool operator()(const DistPair p1, const DistPair p2)const
    {
        return p1.second>p2.second;
    }
};

///////////////////////////////////////////////////////////////////////////////

void KMean(const DataSet&          dbase,
          size_t                  k,
          std::vector<DistPair >& dist,
          std::string             fname,
          bool                    bVerbose)
{
    if(bVerbose)
    {
        fprintf(stdout, "** Computing KNN curve\n");
    }

    const size_t iNb = dbase.size();
    dist.resize(iNb);
    // fill the point ID value...
    std::vector<size_t> points(iNb);
    for(size_t i=0; i<iNb; i++)
    {
        points[i] = dbase[i].getId().value();
    }

    // Sort the points, in respect to its distance to the the origin.
    KNNCompare<size_t> c(dbase);
    QuickSort<size_t>(points, 0, points.size()-1, c);
    //std::sort(points.begin(), points.end(), c);

    // the origin reference point
    const Point p0(0, dbase[0].dim());

    // select a window of k-points to the left, and k-points to the right.
    // sort
    // pick the k-th point
    std::vector<double> kdist_window(2*k);
    for(size_t i=0; i<points.size(); i++)
    {
        size_t beg = std::max(((int)i-(int)k), 0);
        size_t end = std::min(i+k,             iNb-1);

        kdist_window.resize(0);
        // reference
        const double dist_pi = dbase[points[i]].distanceTo(p0);
        for(size_t j=beg; j<=end; j++)
        {
            if(i==j) continue;
            const double dist_pj = dbase[points[j]].distanceTo(p0);
            kdist_window.push_back(fabs(dist_pj - dist_pi));
        }
        // sort the window, and pick the 'k-th' one....
        std::sort(kdist_window.begin(), kdist_window.end());
        dist[i] = std::pair<size_t, double>(i, kdist_window[k-1]);
    }
    MyDistCompare c2;
    std::sort(dist.begin(), dist.end(), c2);

    if(!fname.empty())
    {
        if(bVerbose)                                                                
        {                                                                           
            fprintf(stdout, "** Writting knn data to file '%s'\n", fname.c_str());
        }
        FILE* f = fopen(fname.c_str(), "wt");
        if(f)
        {
            for(size_t i=0; i<dist.size(); i++)
            {
                fprintf(f, "%ld %ld %g\n", i, dist[i].first, dist[i].second);
            }
            fclose(f);
        }
    }
}

///////////////////////////////////////////////////////////////////////////////


