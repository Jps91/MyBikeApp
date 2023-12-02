#include "Filter.h"

BandPassFilter::BandPassFilter(double initialAlphaLow, double initialAlphaHigh)
	: alphaLow(initialAlphaLow),
	alphaHigh(initialAlphaHigh)
{
	;
}

double BandPassFilter::filter(double rawValue)
{
	// Low-pass component
	double lowPass = alphaLow * rawValue + (1.0 - alphaLow) * prevFilteredValueLow;

	// High-pass component
	double highPass = rawValue - lowPass;

	// Combine low-pass and high-pass components
	filteredValue = alphaHigh * (filteredValue + highPass) + (1.0 - alphaHigh) * lowPass;

	prevFilteredValueLow = lowPass;


	return filteredValue;
}

FrequnceTimeFilter::FrequnceTimeFilter(double initialAlphaLow, double initialAlphaHigh)
{
	;
}

FrequnceTimeFilter::~FrequnceTimeFilter()
{
}

double FrequnceTimeFilter::filter(double deltaTime, double rawValue)
{
	// Low-pass component
	double lowPass = alphaLow * rawValue + (1.0 - alphaLow) * prevFilteredValueLow;

	// High-pass component
	double highPass = rawValue - lowPass;

	// Combine low-pass and high-pass components
	filteredValue = alphaHigh * (filteredValue + (rawValue - prevFilteredValueLow) / deltaTime) + (1.0 - alphaHigh) * prevFilteredValueLow;

	prevFilteredValueLow = lowPass;

	return filteredValue;
}

FrequenceAmplitudeFilter::FrequenceAmplitudeFilter(double initialAlphaLow, double initialAlphaHigh)
{
	;
}

FrequenceAmplitudeFilter::~FrequenceAmplitudeFilter()
{
}

double FrequenceAmplitudeFilter::filter(double deltaTime, double rawValue)
{
	// Berechne die Amplitude des aktuellen Werts
	double amplitude = abs(rawValue);

	// Niedrigpasskomponente auf die Amplitude anwenden
	double lowPassAmplitude = alphaLow * amplitude + (1.0 - alphaLow) * prevFilteredValueLow;

	// Hochpasskomponente auf die Amplitude anwenden
	double highPassAmplitude = amplitude - lowPassAmplitude;

	// Kombiniere Niedrigpass- und Hochpasskomponenten
	filteredValue = alphaHigh * (filteredValue + (amplitude - prevFilteredValueLow) / deltaTime) + (1.0 - alphaHigh) * prevFilteredValueLow;

	prevFilteredValueLow = lowPassAmplitude;

	return filteredValue;
}

averageFilter::averageFilter()
{
	;
}

void averageFilter::filter(std::vector<_CRT_DOUBLE>& valueList)
{
	FastAverageDouble av(120);		//=n*seconds
	FastAverageDouble av2(50);
	FastAverageDouble av3(180);
	for (size_t i = 0; i < valueList.size(); i++)
	{
		valueList[i].x=av3.additionalValue(av2.additionalValue((av.additionalValue(valueList[i].x))));
	}
}

averageFilter::~averageFilter()
{
}
