#pragma once
#include <vector>
#include <fstream>
#include <istream>
#include <fstream>
#include <cmath>  
#include <iostream>


const double pi = 3.1415926535897932385;
class GYRO_old
{
public:
	std::vector<_CRT_DOUBLE> time;

	std::vector<_CRT_DOUBLE>x;
	std::vector<_CRT_DOUBLE>y;
	std::vector<_CRT_DOUBLE>z;
	
	std::vector<_CRT_DOUBLE>x_true;
	std::vector<_CRT_DOUBLE>y_true;
	std::vector<_CRT_DOUBLE>z_true;

	std::vector<_CRT_DOUBLE>pitch;
	std::vector<_CRT_DOUBLE>roll;
	std::vector<_CRT_DOUBLE>yaw;

	size_t entries = 0;
	GYRO_old(std::string filename);
	void correctMounting(double yaw, double roll, double pitch);
	void rotation();
	void rotationTrue();
	size_t findClosestElement(double target);
	~GYRO_old();

private:
	std::string fileName_m = "Gyroscope.csv";
};

