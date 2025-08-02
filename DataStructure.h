#pragma once
#include <vector>
#include <stdio.h>
#include <iostream>


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


struct Rotation_1D_Degree	//as well as gps bearing_degree
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
	std::vector<double>global_yaw;	//bearing_degree

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



struct RotationRadians {
	double time;
	double roll;
	double pitch;
	double yaw;

	// Element-wise addition
	RotationRadians operator+(const RotationRadians& other) const {
		return {
			time,
			roll + other.roll,
			pitch + other.pitch,
			yaw + other.yaw
		};
	}

	// Element-wise subtraction
	RotationRadians operator-(const RotationRadians& other) const {
		return {
			time,
			roll - other.roll,
			pitch - other.pitch,
			yaw - other.yaw
		};
	}

	// Element-wise multiplication
	RotationRadians operator*(const RotationRadians& other) const {
		return {
			time,
			roll * other.roll,
			pitch * other.pitch,
			yaw * other.yaw
		};
	}

	// Scalar multiplication
	RotationRadians operator*(double scalar) const {
		return {
			time,
			roll * scalar,
			pitch * scalar,
			yaw * scalar
		};
	}

	friend RotationRadians operator*(double scalar, const RotationRadians& r) {
		return r * scalar;
	}

	// Element-wise division
	RotationRadians operator/(const RotationRadians& other) const {
		return {
			time,
			roll / other.roll,
			pitch / other.pitch,
			yaw / other.yaw
		};
	}

	// Scalar division
	RotationRadians operator/(double scalar) const {
		return {
			time,
			roll / scalar,
			pitch / scalar,
			yaw / scalar
		};
	}
};
struct RotationQuaternion {
	double time;
	double w, x, y, z;

	// Addition (component-wise)
	RotationQuaternion operator+(const RotationQuaternion& other) const {
		return { time, w + other.w, x + other.x, y + other.y, z + other.z };
	}

	// Subtraction (component-wise)
	RotationQuaternion operator-(const RotationQuaternion& other) const {
		return { time, w - other.w, x - other.x, y - other.y, z - other.z };
	}

	// Quaternion multiplication (Hamilton product)
	RotationQuaternion operator*(const RotationQuaternion& q) const {
		return {
			time,
			w * q.w - x * q.x - y * q.y - z * q.z,
			w * q.x + x * q.w + y * q.z - z * q.y,
			w * q.y - x * q.z + y * q.w + z * q.x,
			w * q.z + x * q.y - y * q.x + z * q.w
		};
	}

	// Scalar multiplication
	RotationQuaternion operator*(double scalar) const {
		return { time, w * scalar, x * scalar, y * scalar, z * scalar };
	}

	friend RotationQuaternion operator*(double scalar, const RotationQuaternion& q) {
		return q * scalar;
	}

	// Scalar division
	RotationQuaternion operator/(double scalar) const {
		return { time, w / scalar, x / scalar, y / scalar, z / scalar };
	}

	// Norm squared
	double normSquared() const {
		return w * w + x * x + y * y + z * z;
	}

	// Inverse quaternion (assuming non-zero norm)
	RotationQuaternion inverse() const {
		double n = normSquared();
		if (n == 0) throw std::runtime_error("Cannot invert zero quaternion");
		return { time, w / n, -x / n, -y / n, -z / n };
	}

	// Quaternion division: q1 / q2 = q1 * inverse(q2)
	RotationQuaternion operator/(const RotationQuaternion& q) const {
		return *this * q.inverse();
	}
};

struct Axis  // Calculations are based per index
{
	std::vector<double> data;

	//Utility stuff:
	static constexpr double PI = 3.14159265358979323846;

	Axis getSegment(size_t startIndex, const size_t endIndex) const
	{
		Axis segment;

		if (startIndex >= endIndex || endIndex > data.size())
		{
			std::cerr << "ERROR: Invalid range in getSegment(" << startIndex << ", " << endIndex << ".	The size is: )" << data.size() << "\n";
			return segment;
		}
		segment.data = std::vector<double>(data.begin() + startIndex, data.begin() + endIndex);
		return segment;
	}
	bool overwriteBySegment(const Axis& segment, const size_t startIndex)
	{
		//Idk what I want
		//1. do Nothing when it does not match the size
		//2. do it until the we run into the limits (short4en then vector)
		//3. throw an erro or warn or both
		//4. Extend the base vector

		size_t tempIndex = startIndex;
		for (size_t segmentIndex = 0; segmentIndex < segment.data.size() && tempIndex < data.size(); segmentIndex++, tempIndex++)
		{
			data[tempIndex] = segment.data[segmentIndex];
		}
		if (segment.data.size() + startIndex > data.size())
		{
			return false;
		}
	}

	//Linear math operators:
	Axis operator+(double val) const
	{
		return elementwise_op(val, [](double a, double b) { return a + b; });
	}
	Axis operator+(const Axis& other) const
	{
		return elementwise_op(other, [](double a, double b) { return a + b; });
	}

	Axis operator-(double val) const
	{
		return elementwise_op(val, [](double a, double b) { return a - b; });
	}
	Axis operator-(const Axis& other) const
	{
		return elementwise_op(other, [](double a, double b) { return a - b; });
	}

	Axis operator*(double val) const
	{
		return elementwise_op(val, [](double a, double b) { return a * b; });
	}
	Axis operator*(const Axis& other) const
	{
		return elementwise_op(other, [](double a, double b) { return a * b; });
	}

	Axis operator/(double val) const
	{
		if (val == 0.0)
		{
			std::cerr << "WARNING: Division by zero in Axis::operator/ with scalar using DBL_MIN instead\n";
			val = DBL_MIN;
		}
		return elementwise_op(val, [](double a, double b) { return a / b; });
	}
	Axis operator/(const Axis& other) const
	{
		return elementwise_op(other, [](double a, double b)
			{
				if (b == 0.0)
				{
					std::cerr << "WARNING:	Division by zero in Axis::operator/	using dblMIN instead\n";
					b = DBL_MIN;
				}
				return a / b;
			});
	}


	//Linear math functions:
	Axis mod(double val) const
	{
		return elementwise_op(val, [](double a, double b) { return std::fmod(a, b); });
	}
	Axis mod(const Axis& other) const
	{
		return elementwise_op(other, [](double a, double b) {return std::fmod(a, b); });
	}

	Axis pow(double val) const
	{
		return elementwise_op(val, [](double a, double b) { return std::pow(a, b); });
	}
	Axis pow(const Axis& other)
	{
		return elementwise_op(other, [](double a, double b) {return std::pow(a, b); });
	}

	Axis sqrt() const
	{
		Axis result;
		result.data.resize(data.size());
		for (size_t i = 0; i < data.size(); ++i)
		{
			if (data[i] >= 0.0)
				result.data[i] = std::sqrt(data[i]);
			else
			{
				std::cerr << "WARNING: sqrt of negative value\n";
				result.data[i] = 0.0; // or NAN
			}
		}
		return result;
	}
	Axis sqrt(const Axis& other)
	{
		return elementwise_op(other, [](double a, double /*unused*/)
			{
				if (a >= 0.0)
				{
					return std::sqrt(a);
				}
				else
				{
					return 0.0;
				}
			});
	}

	//Trigonomie:
	Axis sin() const
	{
		Axis result;
		result.data.resize(data.size());
		for (size_t i = 0; i < data.size(); ++i)
			result.data[i] = std::sin(data[i]);
		return result;
	}
	Axis sin(const Axis& other)
	{
		return elementwise_op(other, [](double a, double /*unused*/) {return std::sin(a); });
	}

	Axis asin() const
	{
		Axis result;
		result.data.resize(data.size());
		for (size_t i = 0; i < data.size(); ++i)
		{
			if (data[i] < -1.0 || data[i] > 1.0)
				std::cerr << "ERROR: asin domain error: input not in [-1, 1]\n";
			result.data[i] = std::asin(data[i]);
		}
		return result;
	}
	Axis asin(const Axis& other)
	{
		return elementwise_op(other, [](double a, double /*unused*/)
			{
				if (a < -1.0 || a > 1.0)
					std::cerr << "ERROR:	asin domain error: input not in the range of [-1, 1]\n";
				return std::asin(a);
			});
	}


	Axis cos() const
	{
		Axis result;
		result.data.resize(data.size());
		for (size_t i = 0; i < data.size(); ++i)
			result.data[i] = std::cos(data[i]);
		return result;
	}
	Axis cos(const Axis& other)
	{
		return elementwise_op(other, [](double a, double /*unused*/) {return std::cos(a); });
	}

	Axis acos() const
	{
		Axis result;
		result.data.resize(data.size());
		for (size_t i = 0; i < data.size(); ++i)
		{
			if (data[i] < -1.0 || data[i] > 1.0)
				std::cerr << "ERROR: acos domain error: input not in [-1, 1]\n";
			result.data[i] = std::acos(data[i]);
		}
		return result;
	}
	Axis acos(const Axis& other)
	{
		return elementwise_op(other, [](double a, double /*unused*/) {
			if (a < -1.0 || a > 1.0)
				std::cerr << "ERROR:	acos domain error: input not in range of[-1, 1]\n";
			return std::acos(a);
			});
	}


	Axis tan() const
	{
		Axis result;
		result.data.resize(data.size());
		for (size_t i = 0; i < data.size(); ++i)
			result.data[i] = std::tan(data[i]);
		return result;
	}
	Axis tan(const Axis& other)
	{
		return elementwise_op(other, [](double a, double /*unused*/) {return std::tan(a); });
	}

	Axis atan() const
	{
		Axis result;
		result.data.resize(data.size());
		for (size_t i = 0; i < data.size(); ++i)
			result.data[i] = std::atan(data[i]);
		return result;
	}
	Axis atan(const Axis& other)
	{
		return elementwise_op(other, [](double a, double /*unused*/) {return std::atan(a); });
	}

	Axis atan2(const Axis& other)
	{
		return elementwise_op(other, [](double a, double b) {return std::atan2(a, b); });
	}

	//Unit convert and special functions:
	Axis degreeToRad(const Axis& other)
	{
		return elementwise_op(other, [](double a, double /*unused*/) {return (a * PI / 180); });
	}
	Axis radToDegree(const Axis& other)
	{
		return elementwise_op(other, [](double a, double /*unused*/) {return (a * 180 / PI); });
	}

private:
	// Generic element-wise operation
	template<typename Func>
	Axis elementwise_op(const Axis& other, Func op) const
	{
		size_t size1 = data.size();
		size_t size2 = other.data.size();
		size_t minSize = std::min(size1, size2);
		size_t maxSize = std::max(size1, size2);

		Axis result;
		result.data.resize(maxSize);

		// Compute overlapping part
		for (size_t i = 0; i < minSize; ++i)
		{
			result.data[i] = op(data[i], other.data[i]);
		}

		// Copy remainder from longer vector
		if (size1 > size2)
		{
			std::copy(data.begin() + size2, data.end(), result.data.begin() + size2);
		}
		else if (size2 > size1)
		{
			std::copy(other.data.begin() + size1, other.data.end(), result.data.begin() + size1);
		}

		return result;
	}

	// Generic element-wise operation for LIST vs. VALUE
	template<typename Func>
	Axis elementwise_op(double val, Func op) const
	{
		Axis result;
		result.data.resize(data.size());

		for (size_t i = 0; i < data.size(); ++i)
		{
			result.data[i] = op(data[i], val);
		}

		return result;
	}
};

struct KinematicListV3
{
	Axis time;
	Axis x;
	Axis y;
	Axis z;

	//Utility:
	void resizeAllAxis(size_t newSize)
	{
		p_size = newSize;
		time.data.resize(newSize);
		x.data.resize(newSize);
		y.data.resize(newSize);
		z.data.resize(newSize);
	}

	size_t getSize() const
	{
		if (checkSize())
		{
			return 0;
		}
		//p_size = time.data.size();
		return p_size;
	}

	bool checkSize() const
	{
		if (p_size == time.data.size() && p_size == x.data.size() && p_size == y.data.size() && p_size == z.data.size())
		{
			return true;
		}
		return false;
	}

	bool checkTime() const
	{
		if (time.data.size() < 1 || time.data[0] < 0.0)
		{
			return false;
		}

		for (size_t i = 1; i < getSize(); i++)
		{
			if (time.data[i - 1] >= time.data[i])
			{
				return false;
			}
		}
		return true;
	}

	bool addEntrySingle(const double newTime, const double newX, const double newY, const double newZ)	//just like pushback
	{
		if (time.data.back() >= newTime)
		{
			return false;	//the Time is not increasing
		}

		time.data.push_back(newTime);
		x.data.push_back(newX);
		y.data.push_back(newY);
		z.data.push_back(newZ);

		return true;
		//Maybe we want to implement any sizecheck, otherwise no need to be a bool.
	}
	bool addEntryList(const std::vector<double>& newTime, const std::vector<double>& newX, const std::vector<double>& newY, const std::vector<double>& newZ)
	{
		if (newTime.size() != newX.size() && newTime.size() != newY.size() && newTime.size() != newZ.size())
		{
			return false;
		}

		resizeAllAxis(newTime.size());
		time.data = newTime;
		x.data = newX;
		y.data = newY;
		z.data = newZ;

		return checkTime();
	}

	KinematicListV3 getSegment(size_t startIndex, size_t endIndex) const
	{
		KinematicListV3 segment;
		segment.time = time.getSegment(startIndex, endIndex);
		segment.x = x.getSegment(startIndex, endIndex);
		segment.y = y.getSegment(startIndex, endIndex);
		segment.z = z.getSegment(startIndex, endIndex);

		return segment;
	}
	bool overwriteBySegement(const KinematicListV3& segment, size_t startIndex)
	{
		//Idk what I want
		//1. do Nothing when it does not match the size
		//2. do it until the we run into the limits
		//3. throw an erro or warn or both

		/*
		if (segment.getSize() + startIndex > getSize())
		{
			return false;
		}*/
		size_t tempIndex = startIndex;
		for (size_t segmentIndex = 0; segmentIndex < segment.getSize() && tempIndex < getSize(); segmentIndex++, tempIndex++)
		{
			time.data[tempIndex] = segment.time.data[segmentIndex];
		}
		return true;
	}

	//TODO:
	//Basic math for multi Axis calculations like rotation.
	void scale(double factor)
	{
		x = x * factor;
		y = y * factor;
		z = z * factor;
	}

private:
	size_t p_size = 0;
};

struct RoationListV3
{
	Axis time;
	Axis roll;
	Axis pitch;
	Axis yaw;
	void resize(size_t newSize)
	{
		time.data.resize(newSize);
		roll.data.resize(newSize);
		pitch.data.resize(newSize);
		yaw.data.resize(newSize);
	}

	//ToDo: Basic Math look at the struct above (Kinematic)

};








