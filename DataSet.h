#ifndef _DataSet_h_
#define _DataSet_h_

#include <string>
#include <vector>
#include "Point.h"

//
// This class stores all the points available in the model
//
class DataSet
{
///////////////////////////////////////////////////////////////////////////////
    public:
///////////////////////////////////////////////////////////////////////////////
    
                        DataSet             ( )                               ;
                        DataSet             (const std::vector<Point*>& v)    ;

                       ~DataSet             ( )                               ;
    
    // total number of points
    size_t              size                ( )                         const ;

    // dimention of first point
    size_t              dim                 ( )                         const ;

    // the ith point in the data set
    const Point&        operator[]          (const size_t i)            const ;
    
    // the ith point in the data set
    const Point&        operator[]          (const PointId& id)         const ;
    
    /// \brief addPoint Adds new point for the dataset. The dataset will take
    ///                 ownership of the externally allocated pointer.
    /// \param point   Point* object.
    ///
    void                addPoint            (Point* point)                    ;

    /// \brief addPointList adds a set of points to the data set. The pointer
    ///                     objects will be copied internally, and deleted at
    ///                     the dataset destructor
    /// \param vec Vector of points
    ///
    void                addPointList        (const std::vector<Point*>& vec)  ;

    void                write               (const std::string fname)   const ;

    ///
    /// \brief read the data set from a file. The file is expected to be csv,
    ///             with a point per line.
    /// \param fname filename
    /// \param separator the line separator
    /// \return true/false.
    ///
    bool                read                (const std::string fname, 
                                             const char* separator=",")        ;
    
///////////////////////////////////////////////////////////////////////////////
    private:
///////////////////////////////////////////////////////////////////////////////
    
    // Init collection of points
    void                init_points         ( )                               ;
    
    size_t              m_nb_dimension                                        ;
    std::vector<Point*> m_pointVector                                         ;
};

#endif

