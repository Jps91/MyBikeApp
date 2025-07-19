#pragma once
#include <vector>
#include <map>
#include "GPS.h"
enum SegmentType
{
	CORNER, STRAIGHT
};

class SegmentsRide
{
public:
	std::vector<_CRT_DOUBLE> x;
	std::vector<_CRT_DOUBLE> y;
	std::vector<_CRT_DOUBLE> z;
	
	std::vector<double>abweichungen;

	std::map<int, SegmentType, size_t> map;

	SegmentsRide(GPS_old gps);
	double calculateStandardDeviation(const std::vector<double>& data);
private:
	int mapNumber = 0;
};

