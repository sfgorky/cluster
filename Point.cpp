#include <ostream>
#include <cmath>
#include <set>
#include "Point.h"

double pointDiffNorm(const Point& p1, const Point& p2)
{
    double norm(0);
    size_t nb = p1.dim();
    for(size_t i=0; i<nb; i++)
    {
        norm += fabs(p1[i] - p2[i]);
    }
    return sqrt(norm);
}

///////////////////////////////////////////////////////////////////////////////
//
std::ostream& operator<<(std::ostream& os, const PointId& id)
{
    os << id.value();
    return os;
}

///////////////////////////////////////////////////////////////////////////////
//
std::ostream& operator<<(std::ostream& os, const Point& point)
{
    for(size_t i=0; i<point.size(); i++)
    {
        os << point[i] << " ";
    }
    return os;
}

////////////////////////////////////////////////////////////////////////////////

Point& Point::operator+=(const Point& other)
{
    assert(size() == other.size());
    std::vector<Coord>&       thisV = coordVector( );
    const std::vector<Coord>& thisO = other.coordVector( );
    
    for(size_t i =0; i<thisV.size(); i++)
    {
        thisV[i] += thisO[i];
    }
    return *this;
}

////////////////////////////////////////////////////////////////////////////////

Point& Point::operator/=(double other)
{
    assert(m_coordenateVector.size()!=0);
    
    std::vector<Coord>&  thisV = coordVector( );
    for(size_t i=0; i<thisV.size(); i++)
    {
        thisV[i] /= other;
    }
    return *this;
}

////////////////////////////////////////////////////////////////////////////////

void Point::clear( )
{
    std::vector<Coord>&  thisV = coordVector( );
    for(size_t i=0; i<thisV.size(); i++)
    {
        thisV[i] = 0.0;
    }
}

////////////////////////////////////////////////////////////////////////////////

void Point::print(FILE* p, const char* sep)const
{
    bool first = true;
    for(size_t i=0; i<m_coordenateVector.size(); i++)
    {
        const Coord coord = m_coordenateVector[i];
    
        if(!first) fprintf(p, "%s", sep);
        fprintf(p, "%15.10E", coord);
        first = false;
    }
    fprintf(p, "\n");
}

////////////////////////////////////////////////////////////////////////////////

std::string Point::toString( )const
{
    std::string s;
    for(size_t i=0; i<m_coordenateVector.size(); i++)
    {
        const Coord coord = m_coordenateVector[i];
        
        char sBuffer[64];
        snprintf(sBuffer,sizeof(sBuffer), "%15.10E ", coord);
        s += sBuffer;
    }
    return s;
}

////////////////////////////////////////////////////////////////////////////////

Distance Point::distanceTo(const Point& p)const
{
    const Point::CoordVector& xCoord =   coordVector( );
    const Point::CoordVector& yCoord = p.coordVector( );
    
    Point::CoordVector::const_iterator c1     = xCoord.begin();
    Point::CoordVector::const_iterator c2     = yCoord.begin();
    Point::CoordVector::const_iterator c1_end = xCoord.end();
    
    // euclidian diastance
    /*
     
     for(; cpx!=cpx_end; ++cpx,++cpy)
     {
     Distance diff = *cpx - *cpy;
     total += (diff * diff);
     }
     */
    Distance total(0);
    for(; c1!=c1_end; ++c1, ++c2)
    {
        const Distance diff = *c1 - *c2;
        total += (diff * diff);
    }
    total = sqrt(total);
    
    return total;
}
    
///////////////////////////////////////////////////////////////////////////////

Distance Point::squareDistanceTo(const Point& p)const
{
    const Point::CoordVector& xCoord =   coordVector( );
    const Point::CoordVector& yCoord = p.coordVector( );
    
    Point::CoordVector::const_iterator c1     = xCoord.begin();
    Point::CoordVector::const_iterator c2     = yCoord.begin();
    Point::CoordVector::const_iterator c1_end = xCoord.end();

    Distance total(0);
    for(; c1!=c1_end; ++c1, ++c2)
    {
        const Distance diff(*c1 - *c2);
        total += (diff * diff);
    }
    return total;
}






