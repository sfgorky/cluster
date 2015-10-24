#ifndef _Random_h_
#define _Random_h_

#include <vector>
///
/// \brief randomValue returns a random double number in the given interval
/// \param min_val
/// \param max_val
/// \return
double randomValue(const double min_val, const double max_val);

///
/// \brief intRandomValue random integer number between [0, maxVal-1]
/// \param maxval
/// \return
///
size_t intRandomValue(const size_t maxVal);

double randomNormal(double mean, double stddev);

std::vector<double> getRandomVector(const size_t    iNb,
                                    const double    minVal,
                                    const double    maxVal);

std::vector<double> getNormalVector(const size_t    iNb,
                                    const double    mean,
                                    const double    stddev);

#endif


