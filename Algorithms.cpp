#include "Algorithms.h"



size_t findNextLowerElementIndex(const std::vector<double>& sortedList, const size_t listSize, const double target)
{
	size_t l = 0, r = listSize;
	while (l < r) {
		size_t m = (l + r) >> 1;
		(sortedList[m] <= target ? l : r) = m + (sortedList[m] <= target);
	}
	return l ? l - 1 : 0;
}

std::vector<double> computeTiltAngleToEarth(const std::vector<double>& accX, const std::vector<double>& accY)
{
	size_t size = accX.size();
	std::vector<double> angles(size);

#pragma omp parallel for schedule(static)
	for (size_t i = 0; i < size; ++i)
	{
		// Angle in radians between the vector (accX, accY) and the vertical axis (0, -1)
		angles[i] = std::atan2(accX[i], -accY[i]); // atan2(y, x): handles all quadrants
		//angles[i] *= RAD_TO_DEG; // uncomment if you want degrees    use instead: const double pi = 3.1415926535897932385;
	}

	return angles;
}

size_t findValueEstimatedOneLower(std::vector<double>& time, double targetTime)	//not finished
{
	if (time.size() < 1)
	{
		return 0;
	}

	size_t index = time.size();

	double firstTime = time.front();
	double lastTime = time[index];

	double indexCountPerSecond = (lastTime - firstTime) / index;

	size_t estimatedTargetIndex = targetTime * indexCountPerSecond;

	size_t targetIndex = 0;
	/*
	Code not yet implemented.
	Code has to find the index of the targettime that is equal or the previous time.

	*/
	return targetIndex;
}

Acceleration_1D_MeterPerSecondSquared acceleration_sum(
	const Acceleration_3D_MeterPerSecondSquared& acceleration3D)
{
	return Acceleration_1D_MeterPerSecondSquared{ acceleration3D.time,
		sqrt(acceleration3D.x * acceleration3D.x +
			acceleration3D.y * acceleration3D.y +
			acceleration3D.z * acceleration3D.z)
	};
}

Speed_1D_MeterPerSecond speed_sum(
	const Speed_3D_MeterPerSecond& speed3D)
{
	return Speed_1D_MeterPerSecond{ speed3D.time,
		sqrt(speed3D.x * speed3D.x +
		speed3D.y * speed3D.y +
		speed3D.z * speed3D.z)
	};
}

Rotation_3D_Euler_Radians euler_Radians_from_Quaternion(const Rotation_3D_Quaternion& q)
{
	Rotation_3D_Euler_Radians euler;
	euler.time = q.time;

	// Roll (x-axis rotation)
	double sinr_cosp = 2 * (q.w * q.x + q.y * q.z);
	double cosr_cosp = 1 - 2 * (q.x * q.x + q.y * q.y);
	euler.roll = std::atan2(sinr_cosp, cosr_cosp);

	// Pitch (y-axis rotation)
	double sinp = 2 * (q.w * q.y - q.z * q.x);
	if (std::abs(sinp) >= 1)
		euler.pitch = std::copysign(3.14159265358979323846 / 2, sinp); // use 90 degrees if out of range
	else
		euler.pitch = std::asin(sinp);

	// Yaw (z-axis rotation)
	double siny_cosp = 2 * (q.w * q.z + q.x * q.y);
	double cosy_cosp = 1 - 2 * (q.y * q.y + q.z * q.z);
	euler.yaw = std::atan2(siny_cosp, cosy_cosp);

	return euler;
}

Rotation_3D_Quaternion quaternion_Multiply(
	const Rotation_3D_Quaternion& quaternion1,
	const Rotation_3D_Quaternion& quaternion2)
{
	return Rotation_3D_Quaternion{
		quaternion2.time,
		quaternion1.w * quaternion2.w - quaternion1.x * quaternion2.x - quaternion1.y * quaternion2.y - quaternion1.z * quaternion2.z,
		quaternion1.w * quaternion2.x + quaternion1.x * quaternion2.w + quaternion1.y * quaternion2.z - quaternion1.z * quaternion2.y,
		quaternion1.w * quaternion2.y - quaternion1.x * quaternion2.z + quaternion1.y * quaternion2.w + quaternion1.z * quaternion2.x,
		quaternion1.w * quaternion2.z + quaternion1.x * quaternion2.y - quaternion1.y * quaternion2.x + quaternion1.z * quaternion2.w
	};
}

Rotation_3D_Quaternion quaternion_applyDelta(const Rotation_3D_Quaternion& current, const Rotation_3D_Quaternion& delta)
{
	return quaternion_Multiply(delta, current);
}


Rotation_3D_Quaternion quaternion_delta_from_Rotation_3D_RadiansPerSecond(
	const Rotation_3D_RadiansPerSecond& rotation_3D,
	const double deltaTime)
{
	double angle = std::sqrt(
		rotation_3D.roll * rotation_3D.roll +
		rotation_3D.pitch * rotation_3D.pitch +
		rotation_3D.yaw * rotation_3D.yaw
	) * deltaTime;

	// Avoid division by zero
	if (angle < 1e-8)
	{
		return Rotation_3D_Quaternion{ rotation_3D.time,1.0, 0.0, 0.0, 0.0 };  // No rotation
	}

	// Normalize the axis (use the same values as above)
	double axisX = rotation_3D.roll;
	double axisY = rotation_3D.pitch;
	double axisZ = rotation_3D.yaw;
	double axisLength = std::sqrt(axisX * axisX + axisY * axisY + axisZ * axisZ);

	double halfAngle = angle / 2.0;
	double sinHalfAngle = std::sin(halfAngle);
	double cosHalfAngle = std::cos(halfAngle);

	return Rotation_3D_Quaternion{ rotation_3D.time,
		cosHalfAngle,
		(axisX / axisLength) * sinHalfAngle,
		(axisY / axisLength) * sinHalfAngle,
		(axisZ / axisLength) * sinHalfAngle
	};
}

