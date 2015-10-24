#include <cmath>
#include <time.h>
#include <stdlib.h>
#include "Random.h"

double randomNormal(double mean, double stddev)
{   //Box muller method
    static double n2 = 0.0;
    static int n2_cached = 0;

    if (!n2_cached)
    {
        double x = 0.0;
        double y = 0.0;
        double r = 0.0;
        do
        {
            x = 2.0*rand()/RAND_MAX - 1;
            y = 2.0*rand()/RAND_MAX - 1;

            r = x*x + y*y;
        }
        while (r == 0.0 || r > 1.0);
        {
            double d = sqrt(-2.0*log(r)/r);
            double n1 = x*d;
            n2 = y*d;
            double result = n1*stddev + mean;
            n2_cached = 1;
            return result;
        }
    }
    else
    {
        n2_cached = 0;
        return n2*stddev + mean;
    }
}

////////////////////////////////////////////////////////////////////////////////

double randomValue(const double min_val, const double max_val)
{
    double v =  min_val + (max_val-min_val) * ((rand() % RAND_MAX)/(double)RAND_MAX);
    return v;
}

////////////////////////////////////////////////////////////////////////////////

size_t intRandomValue(const size_t max_val)
{
    return (rand() % max_val);
}

////////////////////////////////////////////////////////////////////////////////

std::vector<double> getRandomVector(const size_t    iNb,
                                    const double    minVal,
                                    const double    maxVal)
{
    std::vector<double> v(iNb);
    for(size_t i=0; i<iNb; i++)
    {
        v[i] = randomValue(minVal, maxVal);
    }
    return v;
}

////////////////////////////////////////////////////////////////////////////////

std::vector<double> getNormalVector(const size_t    iNb,
                                    const double    mean,
                                    const double    stddev)
{
    std::vector<double> v(iNb);
    for(size_t i=0; i<iNb; i++)
    {
        v[i] = randomNormal(mean, stddev);
    }
    return v;
}

////////////////////////////////////////////////////////////////////////////////

