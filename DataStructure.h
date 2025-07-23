#pragma once
#include <vector>
#include <stdio.h>


enum Units
{
	Meters,
	MetersPerSecond,
	MetersPerSecondSquared,

	Degrees,
	Degrees360,
	DegreesPerSecond,
	DegreesPerSecondSquared,

	DegreesLatitude,
	DegreesLongitude,
	DegreesLatitudePerSecond,
	DegreesLongitudePerSecond,
	DegreesLatitudePerSecondSquared,
	DegreesLongitudePerSecondSquared,

	Radians,
	Radians2Pi,
	RadiansPerSecond,
	RadiansPerSecondSquared,

	Kilograms,
	Seconds,
	Amperes,
	Kelvins,
	Moles,
	Candelas,

	Hertz,        // Frequency
	Newtons,      // Force
	Pascals,      // Pressure
	Joules,       // Energy
	Watts,        // Power
	Coulombs,     // Electric charge
	Volts,        // Electric potential
	Farads,       // Capacitance
	Ohms,         // Resistance
	Siemens,      // Conductance
	Webers,       // Magnetic flux
	Teslas,       // Magnetic flux density
	Henrys,       // Inductance
	Lumens,       // Luminous flux
	Lux,          // Illuminance
	Becquerels,   // Radioactivity
	Grays,        // Absorbed dose
	Sieverts,     // Dose equivalent
	Katals        // Catalytic activity
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
		size = gyroTime.size();
		if (size < 2)
		{
			throw std::runtime_error("At least two gyro samples required");
		}

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
		//std::cout << std::fixed << std::setprecision(6);
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


struct Measurements1D
{
	std::vector<DataPoint>u;
	Units unit;

	void resize(size_t size)
	{
		u.resize(size, DataPoint{});
	}
	void newUnit(Units newUnit)
	{
		unit = newUnit;
	}
};

struct Measurements2D
{
	std::vector<DataPoint>v;
	std::vector<DataPoint>w;
	Units unit;

	void resize(size_t size)
	{
		v.resize(size, DataPoint{});
		w.resize(size, DataPoint{});
	}
	void newUnit(Units newUnit)
	{
		unit = newUnit;
	}
};

struct Measurements3D
{
	std::vector<DataPoint>x;
	std::vector<DataPoint>y;
	std::vector<DataPoint>z;
	Units unit;

	void resize(size_t size)
	{
		x.resize(size, DataPoint{});
		y.resize(size, DataPoint{});
		z.resize(size, DataPoint{});
	}
	void newUnit(Units newUnit)
	{
		unit = newUnit;
	}
};


struct VehicleOrientation
{
	std::vector<Measurements3D>position;
	std::vector<Measurements3D>direction;
	std::vector<Measurements3D>speed;
};