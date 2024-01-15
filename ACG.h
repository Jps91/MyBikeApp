#pragma once
#include <vector>
#include <fstream>
#include <istream>
#include <fstream>
#include <math.h>  
#include <iostream>
#include <filesystem>

#include "FastAverageDouble.h"
#include "Filter.h"
#include "Search_bisection.h"
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif


class ACG
{
public:
	std::vector<_CRT_DOUBLE> time;				//Seconds as Double
	std::vector<_CRT_DOUBLE> x;
	std::vector<_CRT_DOUBLE> y;
	std::vector<_CRT_DOUBLE> z;

	std::vector<_CRT_DOUBLE> xm;
	std::vector<_CRT_DOUBLE> ym;
	std::vector<_CRT_DOUBLE> zm;

	std::vector<_CRT_DOUBLE> xtrue;
	std::vector<_CRT_DOUBLE> ytrue;
	std::vector<_CRT_DOUBLE> ztrue;

	std::vector<_CRT_DOUBLE> xfilter;
	std::vector<_CRT_DOUBLE> yfilter;
	std::vector<_CRT_DOUBLE> zfilter;

	size_t entries = 0;
	ACG();
	ACG(std::string filepath);

	void smoothIt();
	void correctMounting();
	size_t findClosestElemen(double target);
	void filter();
	double findRoationAngle();
	double getMountigAngleDegree();
	~ACG();

private:
	std::string fileName_m = "Location.csv";
	double constrad = 0;
	double createRotationTestResult(double angleDegrees);
	double bisearch(double lower_bound, double upper_bound, double tolerance = 1e-9, double target_value = 9.807);
	double mountigAngleDegree = 0;
};

