#include <boost/foreach.hpp>
#include <boost/tokenizer.hpp>

#include <fstream>
#include "DataSet.h"


////////////////////////////////////////////////////////////////////////////////

DataSet::DataSet( )
{
}

////////////////////////////////////////////////////////////////////////////////

DataSet::DataSet(const std::vector<Point*>& v)
{
    addPointList(v);
}

////////////////////////////////////////////////////////////////////////////////

DataSet::~DataSet( )
{
    for(size_t i=0; i< m_pointVector.size(); i++)
    {
        delete  m_pointVector[i];
    }
}

////////////////////////////////////////////////////////////////////////////////

size_t DataSet::size( )const
{
    return  m_pointVector.size();
}

////////////////////////////////////////////////////////////////////////////////

const Point& DataSet::operator[](const size_t i)const
{
    return *m_pointVector[i];
}

////////////////////////////////////////////////////////////////////////////////

const Point& DataSet::operator[](const PointId& id)const
{
    return *m_pointVector[id.value()];
}

////////////////////////////////////////////////////////////////////////////////

void DataSet::addPoint(Point* point)
{
    m_pointVector.push_back(point);
}

////////////////////////////////////////////////////////////////////////////////

void DataSet::addPointList(const std::vector<Point*>& vector)
{
    for(size_t i=0; i<vector.size(); i++)
    {
        m_pointVector.push_back(vector[i]);
    }
}

////////////////////////////////////////////////////////////////////////////////

void DataSet::write(const std::string fname)const
{
    FILE* p = fopen(fname.c_str(), "wt");
    if(p)
    {
        for(size_t i=0; i<size(); i++)
        {
            operator[](i).print(p, ",");
        }
        fclose(p);
    }
    else
    {
        fprintf(stdout, "Error: cannot open file '%s'\n", fname.c_str());
    }
}

////////////////////////////////////////////////////////////////////////////////

bool DataSet::read(const std::string fname)
{
    std::fstream file;
    file.open(fname.c_str(), std::ios::in);
    if(file.is_open())
    {
        typedef boost::tokenizer< boost::char_separator<char> > Tokenizer;
        boost::char_separator<char> sep(",");
        
        std::string line;
        PointId id;
        while(std::getline(file, line))
        {
            Tokenizer info(line, sep);   // tokenize the line of data
            std::vector<double> values;
            
            for(Tokenizer::iterator it = info.begin(); it != info.end(); ++it)
            {
                // convert data into double value, and store
                values.push_back(strtod(it->c_str(), 0));
            }
            addPoint(new Point(id, values));
            ++id;
        }
    }
    return true;
}


////////////////////////////////////////////////////////////////////////////////
