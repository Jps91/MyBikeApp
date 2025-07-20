#pragma once
#include <vector>
#include "GPS.h"
#include "ACG.h"
#include "FastAverageDouble.h"


class Speed
{
public:

	std::vector<_CRT_DOUBLE> time;				//Seconds as Double
	std::vector<_CRT_DOUBLE> speed;
	std::vector<_CRT_DOUBLE> filtertspeed;


	Speed() = default;
	Speed(ACG_old acg);
	void filter();
	~Speed();

};

