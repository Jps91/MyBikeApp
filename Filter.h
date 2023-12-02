#pragma once
#include <cmath>
#include <stdio.h>
#include "FastAverageDouble.h"
class BandPassFilter
{
private:
	double alphaLow{};
	double alphaHigh{};
	double filteredValue{};
	double prevFilteredValueLow{};
public:
	BandPassFilter(double initialAlphaLow, double initialAlphaHigh);
	double filter(double rawValue);

};

class FrequnceTimeFilter
{
public:
	FrequnceTimeFilter(double initialAlphaLow, double initialAlphaHigh);
	~FrequnceTimeFilter();
	double filter(double deltaTime, double rawValue);

private:
	double alphaLow{};
	double alphaHigh{};
	double filteredValue{};
	double prevFilteredValueLow{};
};

class FrequenceAmplitudeFilter
{
public:
	FrequenceAmplitudeFilter(double initialAlphaLow, double initialAlphaHigh);
	~FrequenceAmplitudeFilter();
	double filter(double deltaTime, double rawValue);
private:
	double alphaLow{};
	double alphaHigh{};
	double filteredValue{};
	double prevFilteredValueLow{};

};

class averageFilter
{
public:
	averageFilter();
	void filter(std::vector<_CRT_DOUBLE>& valueList);
	~averageFilter();

private:

};
