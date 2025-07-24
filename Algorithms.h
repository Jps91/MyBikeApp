#pragma once
#include <vector>
#include <cmath>
#include "DataStructure.h"

QuanternionList importFromRadPerSecond();	//this is the Quanterniondelta
size_t findNextLowerElementIndex(const std::vector<double>& sortedList, const size_t listSize, const double target);
std::vector<double> computeTiltAngleToEarth(const std::vector<double>& accX, const std::vector<double>& accY);

size_t findValueEstimatedOneLower(std::vector<double>& time, double targetTime);


/*Single data row Operations*/
Acceleration_1D_MeterPerSecondSquared acceleration_sum(
	const Acceleration_3D_MeterPerSecondSquared& acceleration3D);

Speed_1D_MeterPerSecond speed_sum(
	const Speed_3D_MeterPerSecond& speed3D);


Rotation_3D_Euler_Radians euler_Radians_from_Quaternion(
	const Rotation_3D_Quaternion& q);


Rotation_3D_Quaternion quaternion_Multiply(
	const Rotation_3D_Quaternion& quaternion1,
	const Rotation_3D_Quaternion& quaternion2);

Rotation_3D_Quaternion quaternion_applyDelta(
	const Rotation_3D_Quaternion& current,
	const Rotation_3D_Quaternion& delta);



/*Conversion Function single row*/



/*Complex Conversion (Requeries Delta)*/
Rotation_3D_Quaternion quaternion_delta_from_Rotation_3D_RadiansPerSecond(
	const Rotation_3D_RadiansPerSecond& rotation_3D,
	const double deltaTime);

