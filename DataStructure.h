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


struct GlobalPosition
{
	std::vector<double>time;
	std::vector<double>latitude;
	std::vector<double>longitude;
	std::vector<double>gpsAccuracyHorizontal;

	size_t size;

	void resizeAll(size_t& newSize)
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

	void resizeAll(size_t& newSize)
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

	void resizeAll(size_t& newSize)
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

	void resizeAll(size_t& newSize)
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

	void resizeAll(size_t& newSize)
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

	void resizeAll(size_t& newSize)
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

	void resizeAll(size_t& newSize)
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

	void resizeAll(size_t& newSize)
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

	void resizeAll(size_t& newSize)
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