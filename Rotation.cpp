#include "Rotation.h"

Rotation::Rotation()
{
}

Rotation::Rotation(GYRO gyroue, ACG acg, GPS gps)
{
	GYRO gyro("");
	double y_correction = acg.correctMountingresult(100);
	gyro.correctMounting(0, 0, y_correction);
	gyro.rotationTrue();
	size_t size = gyro.entries;
	time.resize(size);
	pitch.resize(size);
	roll.resize(size);
	yaw.resize(size);
	double ratio = 0.02;

	double corrector = 0;

	FastAverageDouble av(500);
	std::vector<double>rollCorrector;
	rollCorrector.resize(size);
	double averageResult;
	FastAverageDouble total;
	for (size_t i = 1; i < size; i++)
	{
		averageResult = total.addValueNoFilter(gyro.roll[i].x);
	}
	std::vector<double>yawCahce;
	yawCahce.resize(size);
	FastAverageDouble yawAv(500);
	FastAverageDouble yawAv2(50);
	for (size_t i = 1; i < size; i++)
	{
		yawCahce[i] = yawAv2.additionalValue(yawAv.additionalValue(gyro.yaw[i].x))* (180 / pi);

		while (yawCahce[i]<0)
		{
			yawCahce[i] += 360;
		}
		while (yawCahce[i]>360)
		{
			yawCahce[i] =- 360;
		}
	}

	for (size_t i = 1; i < size; i++)
	{
		//pitch=Steepness{Distance&Height&time}

		//double steepness = (gps.altitude[i].x - gps.altitude[i - 1].x) / gps.distance[i];

		//Do weight by Distance and not time
		size_t gpsI = gps.findClosestElement(gyro.time[i].x);

		//double steepness = atan2(gps.altitude[gpsI - 1].x - gps.altitude[gpsI].x, gps.myDist[gpsI - 1].x - gps.myDist[gpsI].x);

		//pitch[i].x = gyro.pitch[i].x * ratio + steepness * (1 - ratio);

		//Pitch usable to calculate acceleration and NOT correct it
		corrector = av.additionalValue(gyro.pitch[i].x);
		pitch[i].x = gyro.pitch[i].x - corrector;

		//std::cout << pitch[i].x << std::endl;

		//rollCorrector = RollAv.additionalValue(gyro.roll[i].x);
		roll[i].x = gyro.roll[i].x - averageResult;
		//std::cout << rollCorrector << std::endl;

		if (!isnan(gps.bearing[gpsI].x))
		{
			yaw[i].x = gps.bearing[gpsI].x;
			yaw[i].x = gps.bearing[gpsI].x * ratio + yawCahce[i] * (1 - ratio);
			if (yaw[i].x > 360)
			{
				yaw[i].x -= 360;
			}
			if (yaw[i].x < 0)
			{
				yaw[i].x += 360;
			}
		}

	}
	tiltX.resize(acg.entries);
	tiltY.resize(acg.entries);
	for (size_t i = 0; i < acg.entries; i++)
	{
		// Tilt angle in the X direction
		tiltX[i].x = std::atan(acg.ytrue[i].x / std::sqrt(acg.xtrue[i].x * acg.xtrue[i].x + acg.ztrue[i].x * acg.ztrue[i].x));

		// Tilt angle in the Y direction
		tiltY[i].x = std::atan(acg.xtrue[i].x / std::sqrt(acg.ytrue[i].x * acg.ytrue[i].x + acg.ztrue[i].x * acg.ztrue[i].x));

	}
}

Rotation::~Rotation()
{
}
