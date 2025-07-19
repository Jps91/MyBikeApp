#pragma once

#include <vector>
#include "GPS.h"
#include "ACG.h"
#include "FastAverageDouble.h"
#include "Rotation.h"
#include "Speed.h"
#include "Interpolation.h"
class Position
{
public:

	std::vector<_CRT_DOUBLE> time;				//Seconds as Double
	std::vector<_CRT_DOUBLE> x;
	std::vector<_CRT_DOUBLE> y;
	std::vector<_CRT_DOUBLE> z;

	Position();
	Position(GPS_old gps);
private:
	const double EarthRadius = 6371.0*1000;
	std::tuple <double, double, double>gps_to_cartesian(double latitude, double longitude, double altitude);
};

