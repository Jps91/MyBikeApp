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