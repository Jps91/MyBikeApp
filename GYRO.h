#pragma once
#include <vector>
#include <fstream>
#include <istream>
#include <fstream>
#include <cmath>  
#include <iostream>


const double pi = 3.1415926535897932385;
class GYRO
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



	std::vector<_CRT_DOUBLE>rpm;

	size_t entries = 0;
	GYRO(std::string filename, double yaw, double roll, double pitch);
	void correctMounting(double yaw, double roll, double pitch);
	void rotation();
	void rotationTrue();
	size_t getIndexByTime(double timestamp);
	size_t findClosestElement(double target);
	//void rpm();
	void store();
	~GYRO();

private:
	std::string fileName_m = "Gyroscope.csv";
	bool status = true;

};

