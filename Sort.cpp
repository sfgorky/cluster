#include "Sort.h"

////////////////////////////////////////////////////////////////////////////////

template<class T>
class IntCompare : public SortCompare<T>
{
///////////////////////////////////////////////////////////////////////////////
    public:
///////////////////////////////////////////////////////////////////////////////
    
    IntCompare( )
    { }
    
    virtual bool operator()(const T d1, const T d2)const
    {
        return d1<=d2;
    }
};


////////////////////////////////////////////////////////////////////////////////

int testSort(const int argc, const char** argv)
{
    const int v[] = { 3, 5, 1, 4, 6, 9 };
    
    std::vector<int> array;
    
    for(size_t i=0; i<sizeof(v)/sizeof(v[0]); i++)
    {
        array.push_back(v[i]);
    }
    
    for(size_t i=0; i<array.size(); i++)
    {
        fprintf(stdout, "%d ", array[i]);
    }
    fprintf(stdout, "\n");
    
    IntCompare<int> c;
    QuickSort(array, 0, array.size()-1, c);
    
    for(size_t i=0; i<array.size(); i++)
    {
        fprintf(stdout, "%d ", array[i]);
    }
    fprintf(stdout, "\n");
    
    
    return 0;
}
