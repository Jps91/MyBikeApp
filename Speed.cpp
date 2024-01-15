#include "Speed.h"

Speed::Speed(ACG acg)
{
	
	acg.correctMounting();
	GPS gps("");
	time = acg.time;
	speed.resize(acg.entries + 1);
	double speed_temp = 0;
	double gpsspeed = 0;
	double ratio = 0.998;	//ratio of GPS speed and ACG speed
	for (size_t i = 1; i < acg.entries; i++)
	{
		size_t gpsI = gps.findClosestElement(acg.time[i].x);
		gpsspeed = gps.speed[gpsI].x;
		speed_temp = speed_temp + (acg.time[i].x - acg.time[i - 1].x) * acg.ytrue[i].x;

		speed_temp = speed_temp * ratio + gpsspeed * (1 - ratio);
		//std::cout << speed_temp<<std::endl;
		if (isnan(speed_temp))
		{
			speed_temp = 0;
		}
		speed[i].x = speed_temp;
		
	}

}

void Speed::filter()
{
	filtertspeed.resize(time.size() + 1);
	FastAverageDouble av(50);
	FastAverageDouble av2(10);
	for (size_t i = 0; i < time.size(); i++)
	{
		filtertspeed[i].x = av2.additionalValue(av.additionalValue(speed[i].x));
	}
}

Speed::~Speed()
{
}
