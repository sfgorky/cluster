#include <string.h>
#include <cstdlib>

#include "CommandLine.h"


double _string2real(const std::string s)
{
    return std::atof(s.c_str());
}

///////////////////////////////////////////////////////////////////////////////

CommandLine::CommandLine(const int argc, const char** argv)
        :   m_pos(0)
{
    for(int i=0; i<argc; i++)
    {
       //fprintf(stdout, "%s\n", argv[i]);
        m_arg.push_back(std::string(argv[i]));
    }
}

///////////////////////////////////////////////////////////////////////////////

CommandLine::CommandLine(const std::vector<std::string>& arg)
:   m_pos(0)
{
    m_arg = arg;
}

///////////////////////////////////////////////////////////////////////////////

std::string CommandLine::getArgList( )const
{
    std::string s;
    for(size_t i=0; i<m_arg.size(); i++)
    {
        s += m_arg[i];
    }
    return s;
}

///////////////////////////////////////////////////////////////////////////////

size_t CommandLine::pos( )const
{
    return m_pos;
}

///////////////////////////////////////////////////////////////////////////////

size_t CommandLine::nbToken( )const
{
    return m_arg.size();
}

///////////////////////////////////////////////////////////////////////////////

std::string CommandLine::next( )
{
    std::string ret;
    if(!end())
    {
        ret = m_arg[m_pos];
        m_pos++;
     }
     return ret;
}

///////////////////////////////////////////////////////////////////////////////

bool CommandLine::isCommand( )const
{
    bool bOk = true;
    if(end())
    {
        bOk = false;
    }
    else
    {
        std::string ret = m_arg[m_pos];
        bOk = ((ret.size()>0 && ret[0] == '-') ? true : false);
    }
    return bOk;
}

///////////////////////////////////////////////////////////////////////////////

double CommandLine::nextDouble(const double dDefault)
{
    double ret = end()
                    ?   dDefault
                    :   _string2real(next());
    return ret;
}

///////////////////////////////////////////////////////////////////////////////

int CommandLine::nextInt(const int iDefault)
{
    int ret = end()
                ?   iDefault
                :   (int)_string2real(next());
    return ret;
}

///////////////////////////////////////////////////////////////////////////////

bool CommandLine::end( )const
{
     return (m_pos >= m_arg.size()) ? true : false;
}

///////////////////////////////////////////////////////////////////////////////

std::vector<std::string> CommandLine::nextStringArray( )
{
    std::vector<std::string> array;
    array.resize(0);
    array.push_back(next( ));
    
    while(!isCommand( ) && !end())
    {
        array.push_back(next( ));
    }
    return array;
}

///////////////////////////////////////////////////////////////////////////////

std::vector<double> CommandLine::nextDoubleArray( )
{
    std::vector<double> array;
    array.resize(0);
    array.push_back(nextDouble());
    while(!isCommand() && !end())
    {
        array.push_back(nextDouble());
    }
    return array;
}

///////////////////////////////////////////////////////////////////////////////

std::vector<int> CommandLine::nextIntArray( )
{
    std::vector<int> array;
    array.resize(0);
    
    array.push_back(nextInt());
    while(!isCommand( ) && !end())
    {
        array.push_back(nextInt());
    }
    return array;
}


///////////////////////////////////////////////////////////////////////////////

int testCommandLine(const int argc, const char **argv)
{
    if(argc <= 1)
    {
        fprintf(stdout, "%s -i <inputFile> -o <outputFile> -d <double> -flag -values <arrayDouble>\n", argv[0]);
        return 0;
    }
    std::string inputFile;
    std::string outputFile;
    double dValue = 0.0;
    bool bFlag = false;
    std::vector<double> values;
    for(CommandLine commandLine(argc,argv); !commandLine.end();  )
    {
        const std::string key = commandLine.next();
        if(key == "-i")
        {
            inputFile = commandLine.next();
        }
        else if(key == "-o")
        {
            outputFile = commandLine.next();
        }
        else if(key == "-d")
        {
            dValue = commandLine.nextDouble(-1.0);
        }
        else if(key == "-flag")
        {
            bFlag = true;
        }
        else if(key == "-values")
        {
            values = commandLine.nextDoubleArray( );
        }
    }
    
    
    fprintf(stdout, "input file:   %s\n", inputFile.c_str());
    fprintf(stdout, "output file:  %s\n", outputFile.c_str());
    fprintf(stdout, "double value: %g\n", dValue);
    fprintf(stdout, "flag value:   %d\n", (int)bFlag);
    fprintf(stdout, "values:      " );
    for(size_t i=0; i<values.size(); i++) {
        fprintf(stdout, "%g ", values[i]);
    }
    fprintf(stdout, "\n");
    return 0;
}

