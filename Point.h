#ifndef _Point_h_
#define _Point_h_

#include <set>
#include <string>
#include <vector>
#include <assert.h>

// a coordinate
typedef double Coord;

// distance
typedef double DistanceType;
    
class PointId
{
////////////////////////////////////////////////////////////////////////////////
    public:
////////////////////////////////////////////////////////////////////////////////
    
    PointId( )
        : m_idx(0)
    { }
    
    PointId(const size_t idx)
        : m_idx(idx)
    { }
    
    PointId(const PointId& other)
        : m_idx(other.m_idx)
    { }
    
    PointId& operator=(const PointId& other)
    { m_idx = other.m_idx; return  *this; }
    
    bool operator==(const PointId& other)const
    { return (m_idx == other.m_idx); }
    
    bool operator!=(const PointId& other)const
    { return (m_idx != other.m_idx); }
    
    bool operator<(const PointId& other)const
    { return (m_idx < other.m_idx); }
    
    bool operator>(const PointId& other)const
    { return (m_idx > other.m_idx); }
    
    PointId& operator++()
    {
        m_idx++;
        return *this;
    }
    size_t value()const
    {  return m_idx; }

////////////////////////////////////////////////////////////////////////////////
    private:
////////////////////////////////////////////////////////////////////////////////
    
    size_t                      m_idx                                          ;
};

////////////////////////////////////////////////////////////////////////////////

// set of points
typedef std::set<PointId> PointIdSet;

class Point
{
////////////////////////////////////////////////////////////////////////////////
    public:
////////////////////////////////////////////////////////////////////////////////
    
    typedef std::vector<Coord> CoordVector;
    
    Point( )
        : m_id(0)
    {
    }
    
    Point(const PointId& _id, const size_t dim)
    :   m_id(_id)
    {
        m_coordenateVector.resize(dim);
    }
    
    Point(const PointId& _id, const CoordVector& other)
    {
        m_id = _id;
        m_coordenateVector = other;
    }
    
    Point(const Point& other)
    {
        operator=(other);
    }
    
    Point&              operator=          (const Point& other)
    {
        m_id = other.m_id;
        m_coordenateVector = other.m_coordenateVector;
        return *this;
    }

    ///
    /// \brief getId The id of a given point. The PointId object corresponds to
    ///              the index of of this point in the database.
    /// \return PointId
    ///
    const PointId&              getId         ( )                         const
    { return m_id;  }
    
    ///
    /// \brief operator [] The i-th coordenate of the point
    /// \param i the coordenate
    /// \return
    ///
    Coord                       operator[]     (const size_t i)           const
    { return m_coordenateVector[i]; }
    
    Coord                        coord          (const size_t i)          const
    { return m_coordenateVector[i]; }
    
    size_t                      dim            ( )                        const
    { return m_coordenateVector.size(); }
    
    void                        set             (const size_t idx, Coord val)
    { m_coordenateVector[idx] = val; }
    
    Point&                      operator+=      (const Point& other)           ;
    Point&                      operator/=      (double other)                 ;
    
    //! coordenate vector
    const std::vector<Coord>&   coordVector     ( )                       const
    { return m_coordenateVector; }
    
    std::vector<Coord>&         coordVector     ( )
    { return m_coordenateVector;  }
    
    //! Number of coordenates
    //@return the nb of coordenates
    size_t                      size            ( )                       const
    { return m_coordenateVector.size(); }


    // alias for the first coordenate
    Coord                       x               ( )                       const
    { return m_coordenateVector[0]; }

    // alias for the 2nd coordenate
    Coord                       y               ( )                       const
    { return m_coordenateVector[1]; }

    // alias for the 3rd coordenate
    Coord                       z               ( )                       const
    { return m_coordenateVector[2]; }

    void                        clear           ( )                           ;
    void                        print      (FILE* p, const char* sep= " ")const;
    
    //! A string representation of the point's corrdenate.
    std::string                 toString        (const bool useBrackets = true) const ;
    
    //! Computes the distance of this point to another point
    DistanceType                 distanceTo      (const Point& pt)       const ;

    //! distance from this point to another: (p0.x-pt.x)^2 + (p0.y-pt)^2 ...
    DistanceType                 squareDistanceTo(const Point& pt)       const ;
    
    //! the dot product: (p0.x-pt.x) * (p0.y-pt.y) * ...
    double                      dotProduct      (const Point& pt)       const ;
    
////////////////////////////////////////////////////////////////////////////////
private:
////////////////////////////////////////////////////////////////////////////////
    
    CoordVector     m_coordenateVector                                         ;
    PointId         m_id                                                       ;
};


double pointDiffNorm(const Point& p1, const Point& p2);

std::ostream& operator<< (std::ostream& os, const Point& p);
std::ostream& operator<< (std::ostream& os, const PointId& p);
std::ostream& operator<< (std::ostream& os, const PointIdSet& sp);



#endif

