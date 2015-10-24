#ifndef _Sort_h_
#define _Sort_h_

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <vector>

///////////////////////////////////////////////////////////////////////////////

template<class T>
void Swap(std::vector<T>& arr, const int i, const int j)
{
    if(i ==j) return;
    //fprintf(stdout, "swap[%d, %d]\n", i, j);

    T      p = arr[i];
    arr[i]   = arr[j];
    arr[j]   = p;
}

////////////////////////////////////////////////////////////////////////////////

template<class T>
class SortCompare
{
public:
    SortCompare( )
    { }

    virtual ~SortCompare( )
    { }

    virtual bool operator()(const T p1, const T p2)const=0;
};


////////////////////////////////////////////////////////////////////////////////

template<class T>
void QuickSort(std::vector<T>&  arr,
               const int        left,
               const int        right,
               SortCompare<T>&  compare)
{
    if(left<right)
    {
        int p=0;
        {
            const int x = right;       // pivot
            int   i   = (left-1);      // Index of smaller element
            
            for (int j=left; j<=right-1; j++)
            {
                // If current element is smaller than or equal to pivot
                if(compare(arr[j], arr[x]))
                {
                    i++;             // increment index of smaller element
                    Swap(arr, i, j); // Swap current element with index
                }
            }
            Swap(arr, i+1, right);
            p = i+1;
        }
        QuickSort(arr, left, p-1  , compare);
        QuickSort(arr, p+1,  right, compare);
    }
}

////////////////////////////////////////////////////////////////////////////////

int testSort(const int argc, const char** argv);


#endif


