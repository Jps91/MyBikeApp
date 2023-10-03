#pragma once
#include <vector>
#include <fstream>
#include <istream>
#include <fstream>
#include <math.h>  
#include <iostream>


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

	size_t entries = 0;
	GPS();
	GPS(std::string filePath);
	size_t getIndexByTime(double timestamp);
	void terrain();
	double fitLineToPointsAngle(const std::vector<double>& xValues, const std::vector<double>& yValues);
	~GPS();
private:
	std::string fileName_m = "Location.csv";
};

