// MyBikeApp.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <filesystem>
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


	ACG acg("");
	GPS gps("");
	Position pos(gps);

	size_t limit = gps.entries-1;
	for (size_t i = 0; i < limit; i++)
	{
		outputFile << pos.x[i].x << "	" << pos.y[i].x << "	" << pos.z[i].x << '\n';
	}
	outputFile.close();
	std::cout << "Hello World!\n";
}
