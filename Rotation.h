#pragma once
#include <vector>
#include <cmath>
#include "GYRO.h"
#include "GPS.h"
#include "ACG.h"
#include "FastAverageDouble.h"

class Rotation
{
public:
	std::vector<_CRT_DOUBLE> time;				//Seconds as Double
	std::vector<_CRT_DOUBLE> pitch;
	std::vector<_CRT_DOUBLE> roll;
	std::vector<_CRT_DOUBLE> yaw;
	std::vector<_CRT_DOUBLE> tiltX;
	std::vector<_CRT_DOUBLE> tiltY;

	Rotation();
	Rotation(GYRO gyro, ACG acg, GPS gps);
	~Rotation();
};

