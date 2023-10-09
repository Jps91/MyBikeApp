#include "Rotation.h"

Rotation::Rotation()
{
}

Rotation::Rotation(GYRO gyro, ACG acg, GPS gps)
{
	size_t size = gyro.entries;
	time.resize(size);
	pitch.resize(size);
	roll.resize(size);
	yaw.resize(size);
	double ratio = 0.05;

	double corrector = 0;
	double rollCorrector = 0;
	FastAverageDouble av(1000);
	FastAverageDouble RollAv(5000);
	for (size_t i = 1; i < size; i++)
	{
		//pitch=Steepness{Distance&Height&time}

		//double steepness = (gps.altitude[i].x - gps.altitude[i - 1].x) / gps.distance[i];

		//Do weight by Distance and not time
		size_t gpsI = gps.getIndexByTime(gyro.time[i].x);

		//double steepness = atan2(gps.altitude[gpsI - 1].x - gps.altitude[gpsI].x, gps.myDist[gpsI - 1].x - gps.myDist[gpsI].x);

		//pitch[i].x = gyro.pitch[i].x * ratio + steepness * (1 - ratio);

		//Pitch usable to calculate acceleration and NOT correct it
		corrector = av.additionalValue(gyro.pitch[i].x);
		pitch[i].x = gyro.pitch[i].x - corrector;

		//std::cout << pitch[i].x << std::endl;

		rollCorrector = RollAv.additionalValue(gyro.roll[i].x);
		roll[i].x = gyro.roll[i].x - rollCorrector;

	}
}


Rotation::~Rotation()
{
}
