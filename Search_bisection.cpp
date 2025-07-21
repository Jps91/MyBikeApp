#include "Search_bisection.h"

double Search_bisection::bisection(double(*function)(double), double lower_bound, double upper_bound, double tolerance)     //calculates the squarroot
{
    if (function(lower_bound) * function(upper_bound) > 0) 
    {
        throw std::invalid_argument("The function has no root within the specified interval.");
    }

    while ((upper_bound - lower_bound) > tolerance) 
    {
        double middle = (lower_bound + upper_bound) / 2;

        if (function(middle) == 0) 
        {
            return middle;
        }
        else if (function(middle) * function(lower_bound) < 0) 
        {
            upper_bound = middle;
        }
        else 
        {
            lower_bound = middle;
        }
    }

    return (lower_bound + upper_bound) / 2;
}
