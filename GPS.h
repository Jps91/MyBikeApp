#pragma once
#include <vector>
#include <fstream>
#include <istream>
#include <fstream>
#include <math.h>  
#include <iostream>

#ifndef M_PI
#define M_PI (3.14159265358979323846264338327950288)
#endif

class GPS
{
public:
	std::vector<_CRT_DOUBLE> time;				//Seconds as Double
	std::vector<_CRT_DOUBLE> latitude;
	std::vector<_CRT_DOUBLE> longitude;
	std::vector<_CRT_DOUBLE> altitude;			//height
	std::vector<_CRT_DOUBLE> altitude_WGS84;	//heigt based on maps
	std::vector<_CRT_DOUBLE> speed;				//velociety
	std::vector<_CRT_DOUBLE> bearing;			//Angle to north
	std::vector<_CRT_DOUBLE> distance;			//Total distance
	std::vector<_CRT_DOUBLE> horizontalAccuracy;//Accuracy of long and alt
	std::vector<_CRT_DOUBLE> verticalAccuracy;	//height Accuracy without map
	std::vector<_CRT_DOUBLE> satellites;	//count how many satellits
	
	std::vector<_CRT_DOUBLE> myDist;


	size_t entries = 0;
	GPS();
	GPS(std::string filePath);
	size_t findClosestElement(double target);
	~GPS();
private:
	std::string fileName_m = "Location.csv";
};

