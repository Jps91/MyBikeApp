#pragma once
#include <iostream>


class Search_bisection
{
public:
    double bisection(double (*function)(double), double lower_bound, double upper_bound, double tolerance = 1e-9);
       
private:

};
