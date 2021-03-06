#include <stdlib.h>
#include  <stdio.h>

#include "CommandLine.h"
#include "computeDBSCAN.h"
#include "KMean.h"
#include "KMeanTest.h"

///////////////////////////////////////////////////////////////////////////////

enum Command
{
        Command_None
    ,   Command_DBSCAN
    ,   Command_KNN
    ,   Command_KNNTest
    ,   Command_DBSCANTest
};

struct CommandLineOptions
{
    CommandLineOptions( )
    {
        m_command  = Command_None;
        m_minpts   = 3;
        m_eps      = 0.1;
        m_knn      = 4;
        m_verbose  = false;
        m_outfile  = "data";
        m_maxIter  = 10;
        m_seed     = 45;
    }
    std::string m_dsfname;
    std::string m_outfile;
    Command     m_command;
    double      m_eps;
    size_t      m_knn;
    size_t      m_seed;
    size_t      m_maxIter;
    size_t      m_minpts;
    bool        m_verbose;
};

///////////////////////////////////////////////////////////////////////////////

bool parseCommandLine(CommandLineOptions& options, const int argc, const char** argv)
{
    if(argc <= 1)
    {
        fprintf(stdout, "%s\n", argv[0]);
        fprintf(stdout, "   -ds <dsfname>           # input data set (csv) format\n");
        fprintf(stdout, "   -dbscan <minpts> <eps>\n");
        fprintf(stdout, "   -knn <n>                # K-mean with clusters\n");
        fprintf(stdout, "   -out <outfile>          # output file\n");
        fprintf(stdout, "   -v                      # verbose\n");

        fprintf(stdout, "   -knn-test               # run K-Mean test\n");
        fprintf(stdout, "   -dbscan-test            # run DBScan test\n");
        fprintf(stdout, "   -max-iter               # max nb iter (K-mean\n");
        fprintf(stdout, "   -seed <value>           # seed value for random generator\n");
        return true;
    }
    for(CommandLine arg(argc,argv); !arg.end();  )
    {
        const std::string key = arg.next();
        if(key == "-v")
        {
            options.m_verbose = true;
        }
        else if(key == "-ds")
        {
            options.m_dsfname = arg.next();
        }
        else if(key == "-dbscan")
        {
            std::vector<double> next = arg.nextDoubleArray( );
            options.m_command = Command_DBSCAN;
            options.m_minpts = next.size()>0 ? (size_t)next[0] : 3;
            options.m_eps    = next.size()>1 ? next[1] : 0.1;
        }
        else if(key == "-knn")
        {
            options.m_command = Command_KNN;
            options.m_knn     = arg.nextInt(4);
        }
        else if(key == "-v")
        {
            options.m_verbose = true;
        }
        else if(key == "-out")
        {
            options.m_outfile = arg.next();
        }
        else if(key == "-max-iter")
        {
            options.m_maxIter = arg.nextInt();
        }
        else if(key == "-knn-test")
        {
            options.m_command = Command_KNNTest;
        }
        else if(key == "dbdcan-test")
        {
            options.m_command = Command_DBSCANTest;
        }
        else if(key == "-seed")
        {
            options.m_seed = arg.nextInt();
        }
    }
    return true;
}

///////////////////////////////////////////////////////////////////////////////

int main(const int argc, const char** argv)
{
    CommandLineOptions options;
    bool bOk = parseCommandLine(options, argc, argv);

    DataSet ds;
    if(bOk && !options.m_dsfname.empty())
    {
        // opens te ds file
        bOk = ds.read(options.m_dsfname);
        fprintf(stdout, "* Data set '%s'\n", options.m_dsfname.c_str());
        fprintf(stdout, "  Size     %ld\n", ds.size());
        fprintf(stdout, "  Dim      %ld\n", ds.dim());
    }
    if(bOk)
    {
        if(options.m_seed == 0)
        {
            size_t new_seed = time(NULL);
             srand(new_seed);
             fprintf(stdout, "Using random seed: %ld\n", new_seed);
        }
        else
        {
            srand(options.m_seed);
        }
        
        switch(options.m_command)
        {
            case Command_None:
                    break;
            case Command_DBSCAN:
                computeDBSCAN(ds,
                        "",
                        options.m_outfile,
                        options.m_minpts,
                        options.m_eps,
                        options.m_verbose);
                break;
            case Command_KNN:
            {
                fprintf(stdout, "computing xx knn: %s\n", options.m_outfile.c_str());
                     /*
                     std::vector<DistPair> dist;
                     KMean(ds, 
                           options.m_knn, 
                           dist, 
                           options.m_outfile,
                           options.m_verbose);
                     */      
                           
                computeKMeans(ds, options.m_knn, "cluster", options.m_maxIter, options.m_verbose);
                break;
            }
            break;
            case Command_KNNTest:
                KMeanTest(0, 0);
                break;
            case Command_DBSCANTest:
                DBScanTest(0, 0);
                break;
        }
    }
    return 0;
}

