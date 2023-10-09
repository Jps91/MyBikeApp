#pragma once

#include <vector>
#include "GPS.h"
#include "ACG.h"
#include "FastAverageDouble.h"
#include "Rotation.h"
#include "Speed.h"

class Position
{
public:

	std::vector<_CRT_DOUBLE> time;				//Seconds as Double
	std::vector<_CRT_DOUBLE> x;
	std::vector<_CRT_DOUBLE> y;
	std::vector<_CRT_DOUBLE> z;

	Position();
	Position(GPS gps,ACG acg,Rotation rot);

};

