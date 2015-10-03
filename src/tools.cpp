#include "tools.hpp"

double malahidan_distance(double * vector_x, double * vector_y, int size)
{
    double sum = 0, temp;
    for(int i = 0; i < size; ++i)
    {
        temp = vector_x[i] - vector_y[i];
        sum += temp*temp;
    }
    return sum;
}
