#ifndef _CommandLine_h_
#define _CommandLine_h_

#include <vector>
#include <string>

// Very simple code to handle command line code.

class CommandLine
{
///////////////////////////////////////////////////////////////////////////////
    public:
///////////////////////////////////////////////////////////////////////////////

                    CommandLine  (const int argc, const char** argv)           ;
                    CommandLine  (const std::vector<std::string>& args)        ;
   
    // return a string with the join of all the args.
    std::string     getArgList   ( )                                    const ;
    
    // true if at the end of the list
    bool            end          ( )                                     const ;
    // true if the current token is a command. (starting with '-'
    bool            isCommand    ( )                                     const ;
    
    // current pos
    size_t          pos         ( )                                      const ;
    
    size_t         nbToken      ( )                                     const ;
    
    // returns the 'next' value, if it is not a command. Moves the current point
    std::string     next         ( )                                           ;
    
    // returns the 'next' value, as int if it is not a command. Moves the current point
    // if no value is present, returns 'iDefault' value.
    int             nextInt      (const int iDefault=0)                        ;
    
    // returns the 'next' value, as double if it is not a command. Moves the current point
    // if no value is present, returns 'dDefault' value.
    double          nextDouble   (const double dDefault=0.0)                   ;

    // returns the 'next' value, as vector of string if it is not a command.
    // Moves the current point
    std::vector<std::string>    nextStringArray   ( )                          ;

    // returns the 'next' value, as vector of double if it is not a command.
    // Moves the current point
    std::vector<double>         nextDoubleArray   ( )                          ;

    // returns the 'next' value, as vector of int if it is not a command.
    // Moves the current point
    std::vector<int>            nextIntArray      ( )                          ;

    ////////////////////////////////////////////////////////////////////////////////
    private:
////////////////////////////////////////////////////////////////////////////////

    CommandLine( )
    :   m_pos(0)
    { }
    
////////////////////////////////////////////////////////////////////////////////
    private:
////////////////////////////////////////////////////////////////////////////////

        size_t                   m_pos                                         ;
        std::vector<std::string> m_arg                                         ;

};

int testCommandLine(const int argc, const char **argv);

#endif

