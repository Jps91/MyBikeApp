#pragma once
#include <vector>
#include <stdio.h>
#include <iostream>

/*
struct LocalCoordinateSystem
{
};

struct GlobalCoordianteSystem
{
};

template<typename Coordsystem>
struct Rotation;

template<>
struct Rotation<LocalCoordinateSystem>
{
	double roll, pitch, yaw;
	Rotation(double roll = 0, double pitch = 0, double yaw = 0) :roll(roll), pitch(pitch), yaw(yaw) {};
};



Rotation<LocalCoordinateSystem> test{ 0,0,0 };
*/

struct BodyRotation_SpeedRadiansPerSecond
{
	double time;

	double roll;
	double pitch;
	double yaw;
};

struct BodyRotation_SpeedDegreesPerSecond
{
	double time;

	double roll;
	double pitch;
	double yaw;
};

struct BodyRotation_Degrees
{
	double time;

	double roll;
	double pitch;
	double yaw;
};

struct BodyRotation_Radians
{
	double time;

	double roll;
	double pitch;
	double yaw;
};

struct GlobalPosition_2D_GPS
{
	double time;

	double latitude;
	double longitude;
};

struct GlobalPosition_3D_GPS
{
	double time;

	double latitude;
	double longitude;
	double hight;
};

struct Distance_1D_Meter
{
	double time;

	double distance;
};

struct Acceleration_1D_MeterPerSecondSquared
{
	double time;

	double acc;
};

struct Acceleration_3D_MeterPerSecondSquared
{
	double time;

	double x;
	double y;
	double z;
};

struct Speed_1D_MeterPerSecond
{
	double time;
	double v;
};

struct Speed_3D_MeterPerSecond
{
	double time;

	double x;
	double y;
	double z;
};

struct Position_3D_Meter
{
	double time;

	double x;
	double y;
	double z;
};

struct Accuracy_Meter
{
	double	time;
	double accuracy;
};


struct Rotation_1D_Radians
{
	double time;

	double rad;
};

struct Rotation_2D_Radians
{
	double time;

	double x;
	double y;
};

struct Rotation_3D_Radians
{
	double time;

	double roll;
	double pitch;
	double yaw;
};


struct Rotation_1D_RadiansPerSecond
{
	double time;

	double rad;
};

struct Rotation_2D_RadiansPerSecond
{
	double time;

	double x;
	double y;
};

struct Rotation_3D_RadiansPerSecond
{
	double time;

	double roll;
	double pitch;
	double yaw;
};


struct Bearing_Degree
{
	double time;

	double deg;
};


struct Rotation_3D_Quaternion
{
	double time;

	double w = 1;
	double x = 0;
	double y = 0;
	double z = 0;
};

struct Rotation_3D_Euler_Radians
{
	double time;

	double roll;
	double pitch;
	double yaw;
};

struct QuanternionList
{
	std::vector<double>time;

	std::vector<double>w;
	std::vector<double>x;
	std::vector<double>y;
	std::vector<double>z;

	size_t size;
	void resizeAll(size_t newSize)
	{
		size = newSize;
		time.resize(size);
		w.resize(size);
		x.resize(size);
		y.resize(size);
		z.resize(size);
	}

	void normalize()
	{
		size = w.size();
		for (size_t i = 0; i < size; i++)
		{
			double n = std::sqrt(
				w[i] * w[i] +
				x[i] * x[i] +
				y[i] * y[i] +
				z[i] * z[i]);

			if (n == 0)
			{
				n = DBL_MIN;
			}
			w[i] /= n;
			x[i] /= n;
			y[i] /= n;
			z[i] /= n;
		}
	}

	void gyroMeasurementToQuanternions(
		const std::vector<double>& gyroTime,
		const std::vector<double>& gyroX,
		const std::vector<double>& gyroY,
		const std::vector<double>& gyroZ)
	{
		size = gyroX.size();
		time = gyroTime;

		// Resize quaternion vectors to for delta quaternions between intervals NOTE: to Avoid div by 0 we fill the vectors with the angle 0 and dont calculate if we dont need to
		w.resize(size, 1.0);
		x.resize(size, 0.0);
		y.resize(size, 0.0);
		z.resize(size, 0.0);

		// Compute delta quaternion for each interval
		for (size_t i = 0; i < size - 1; ++i)
		{
			// Angular velocity vector magnitude
			double omega_mag = std::sqrt(
				gyroX[i] * gyroX[i] +
				gyroY[i] * gyroY[i] +
				gyroZ[i] * gyroZ[i]);

			// If rotation, delta quaternion is identity  
			if (omega_mag != 0.0)
			{
				// Angle rotated during dt
				double half_theta = omega_mag * (gyroTime[i + 1] - gyroTime[i]) / 2.0;
				double sin_half = std::sin(half_theta);

				// Normalize axis and build quaternion
				w[i] = std::cos(half_theta);
				x[i] = (gyroX[i] / omega_mag) * sin_half;
				y[i] = (gyroY[i] / omega_mag) * sin_half;
				z[i] = (gyroZ[i] / omega_mag) * sin_half;
			}
		}
	}
	void print() const
	{
		for (size_t i = 0; i < w.size(); ++i)
		{
			std::cout << "delta_t [" << time[i] << " -> " << time[i + 1] << "]  "
				<< "q = ["
				<< w[i] << ", "
				<< x[i] << ", "
				<< y[i] << ", "
				<< z[i] << "]\n";
		}
	}
	void printEulerDegrees() const
	{
		for (size_t i = 0; i + 1 < w.size(); ++i)  // i+1 for time range printing
		{
			// Quaternion components
			double qw = w[i];
			double qx = x[i];
			double qy = y[i];
			double qz = z[i];

			// Roll (x-axis rotation)
			double sinr_cosp = 2.0 * (qw * qx + qy * qz);
			double cosr_cosp = 1.0 - 2.0 * (qx * qx + qy * qy);
			double roll = std::atan2(sinr_cosp, cosr_cosp);

			// Pitch (y-axis rotation)
			double sinp = 2.0 * (qw * qy - qz * qx);
			double pitch;
			if (std::abs(sinp) >= 1)
				pitch = std::copysign(3.141592653589793 / 2, sinp);  // use 90 degrees if out of range
			else
				pitch = std::asin(sinp);

			// Yaw (z-axis rotation)
			double siny_cosp = 2.0 * (qw * qz + qx * qy);
			double cosy_cosp = 1.0 - 2.0 * (qy * qy + qz * qz);
			double yaw = std::atan2(siny_cosp, cosy_cosp);

			// Convert radians to degrees
			roll *= 180 / 3.141592653589793;
			pitch *= 180 / 3.141592653589793;
			yaw *= 180 / 3.141592653589793;

			std::cout << "delta_t [" << time[i] << " -> " << time[i + 1] << "]  "
				<< "Euler angles (deg): roll=" << roll
				<< ", pitch=" << pitch
				<< ", yaw=" << yaw << '\n';
		}
	}
};


struct GlobalPosition
{
	std::vector<double>time;
	std::vector<double>latitude;
	std::vector<double>longitude;
	std::vector<double>gpsAccuracyHorizontal;

	size_t size{};

	void resizeAll(size_t newSize)
	{
		size = newSize;
		time.resize(newSize, 0);
		latitude.resize(newSize, 0);
		longitude.resize(newSize, 0);
		gpsAccuracyHorizontal.resize(newSize, 0);
	};
};




struct GlobalSpeed
{
	std::vector<double>time;
	std::vector<double>speed;

	size_t size;

	void resizeAll(size_t newSize)
	{
		size = newSize;
		time.resize(newSize, 0);
		speed.resize(newSize, 0);
	};
};

struct GlobalHight		//The Hight above NN (Normal Null / Sealevel)
{
	std::vector<double>time;
	std::vector<double>hight;
	std::vector<double>gpsAccuracyVertical;

	size_t size;

	void resizeAll(size_t newSize)
	{
		size = newSize;
		time.resize(newSize, 0);
		hight.resize(newSize, 0);
		gpsAccuracyVertical.resize(newSize, 0);
	};

};
struct GlobalRotaion
{
	std::vector<double>time;
	std::vector<double>global_roll;
	std::vector<double>global_pitch;
	std::vector<double>global_yaw;	//bearing

	//NOTE: Bearing and Yaw are not the same.
	//Yaw does not depend on the postion change while driving. BUT the effect might be less noticeable then the yaw drift.
	size_t size;

	void resizeAll(size_t newSize)
	{
		size = newSize;
		time.resize(newSize, 0);
		global_roll.resize(newSize, 0);
		global_pitch.resize(newSize, 0);
		global_yaw.resize(newSize, 0);
	};
};

struct LocalDistance
{
	std::vector<double>time;
	std::vector<double>x;
	std::vector<double>y;
	std::vector<double>z;

	size_t size;

	void resizeAll(size_t newSize)
	{
		size = newSize;
		time.resize(newSize, 0);
		x.resize(newSize, 0);
		y.resize(newSize, 0);
		z.resize(newSize, 0);
	};
};

struct LocalSpeed
{
	std::vector<double>time;
	std::vector<double>x;
	std::vector<double>y;
	std::vector<double>z;

	size_t size;

	void resizeAll(size_t newSize)
	{
		size = newSize;
		time.resize(newSize, 0);
		x.resize(newSize, 0);
		y.resize(newSize, 0);
		z.resize(newSize, 0);
	};
};

struct LocalAcceleration
{
	std::vector<double>time;
	std::vector<double>x;
	std::vector<double>y;
	std::vector<double>z;

	size_t size;

	void resizeAll(const size_t newSize)
	{
		size = newSize;
		time.resize(newSize, 0);
		x.resize(newSize, 0);
		y.resize(newSize, 0);
		z.resize(newSize, 0);
	};
};

struct LocalRoation
{
	std::vector<double>time;
	std::vector<double>roll;
	std::vector<double>pitch;
	std::vector<double>yaw;

	size_t size;

	void resizeAll(size_t newSize)
	{
		size = newSize;
		time.resize(newSize, 0);
		roll.resize(newSize, 0);
		yaw.resize(newSize, 0);
		pitch.resize(newSize, 0);
	};
};

struct LocalRoationSpeed
{
	std::vector<double>time;
	std::vector<double>rollPerSecond;
	std::vector<double>pitchPerSecond;
	std::vector<double>yawPerSecond;

	size_t size;

	void resizeAll(size_t newSize)
	{
		size = newSize;
		time.resize(newSize, 0);
		rollPerSecond.resize(newSize, 0);
		pitchPerSecond.resize(newSize, 0);
		yawPerSecond.resize(newSize, 0);
	};
};




struct DataPoint	//time cant be zero, to prevent divided by 0
{
	double val = 0;	//Measuring Value
	double t = DBL_MIN;	//time
	bool set(double measurementValue, double timeOfMeasurement)		//only nessesary when importing / sanatizing
	{
		val = measurementValue;
		if (timeOfMeasurement < 0)
		{
			return false;
		}
		if (timeOfMeasurement > 0)
		{
			t = timeOfMeasurement;
		}
		return true;
	};

	//NOTICE: 
	//Some calculations could calculate stuff before the start at time 0. Therefore a time beginning offset might be handy.
};

