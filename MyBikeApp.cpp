// MyBikeApp.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <filesystem>
#include <iomanip>


#include "GPS.h"
#include "ACG.h"
#include "GYRO.h"
#include "Speed.h"
#include "Rotation.h"
#include "Position.h"
#include "SegmentsRide.h"



int main()
{
	std::filesystem::path relativePath = "Data\\OUTPUT.csv";
	std::filesystem::path absolutePath = std::filesystem::absolute(relativePath);
	std::fstream outputFile{ absolutePath.string(), std::ios::in | std::ios::out | std::ios::trunc};
	if (!outputFile)
	{
		std::cerr << "Could not open: " << absolutePath.string() << std::endl;
	}
	outputFile << std::fixed << std::setprecision(5);

	ACG acg("");
	GPS gps("");
	GYRO gyro("");
	Rotation rot(acg);
	Position pos(gps);
	size_t limit = acg.entries-1;
	for (size_t i = 0; i < limit; i++)
	{
		size_t gpsI = gps.findClosestElement(acg.time[i].x);
		size_t gyroI = gyro.findClosestElement(acg.time[i].x);
		if (pos.x.at(i).x!=0&& pos.y.at(i).x!=0)
		{
			
			outputFile << pos.x[i].x << "	" << pos.y[i].x << "	" <<   (gps.bearing[gpsI].x*pi/180) - rot.yaw[gyroI].x << '\n';
		}
	}
	outputFile.close();
	std::cout << "Hello World!\n";
}
