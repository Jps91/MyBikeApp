#pragma once
#include <vector>
#include <cmath>
#include "DataStructure.h"


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

Rotation_3D_Quaternion quaternion_normalize(
	const Rotation_3D_Quaternion& rotation);

/*Conversion Function single row*/



/*Complex Conversion (Requeries Delta)*/
Rotation_3D_Quaternion quaternion_delta_from_Rotation_3D_RadiansPerSecond(
	const Rotation_3D_RadiansPerSecond& rotation_3D,
	const double deltaTime);


/*List Operrations:*/
std::vector<Speed_3D_MeterPerSecond>speed_3D_from_Acceleration_3D(
	const std::vector<Acceleration_3D_MeterPerSecondSquared>& acceleration);

std::vector<Acceleration_3D_MeterPerSecondSquared>acceleration_3D_toLocalCoordSystem(
	const std::vector<Acceleration_3D_MeterPerSecondSquared>& acc,
	const std::vector<Rotation_3D_Quaternion>& rotation);

std::vector<Rotation_3D_Quaternion>quaternion_delta_from_Rotation_3D_RadiansPerSecond(
	const std::vector<Rotation_3D_RadiansPerSecond>& rotation);

std::vector<Rotation_3D_Quaternion>quaternion_Multiply(
	const std::vector<Rotation_3D_Quaternion>& base,
	const std::vector<Rotation_3D_Quaternion>& delta);

std::vector<Rotation_3D_Quaternion>quaternion_applyDelta(
	const Rotation_3D_Quaternion& rotationInitial,
	const std::vector<Rotation_3D_Quaternion>& rotation);


