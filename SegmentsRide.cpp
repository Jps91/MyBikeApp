#include "SegmentsRide.h"

SegmentsRide::SegmentsRide(GPS_old gps)
{
	size_t entries = gps.entries;

	x.resize(entries);
	y.resize(entries);
	z.resize(entries);

	const double EARTH_RADIUS = 6371000.0;
	const double DEG_TO_RAD = 0.017453292519943295;

	for (size_t i = 0; i < entries; i++)
	{
		x[i].x = EARTH_RADIUS * gps.longitude[i].x * DEG_TO_RAD;
		y[i].x = EARTH_RADIUS * log(tan((90.0 + gps.latitude[i].x) * DEG_TO_RAD / 2.0));
		z[i].x = gps.altitude[i].x;
	}
	const int range = 3;
	abweichungen.resize(entries);
	for (size_t i = range; i < entries - range; i++)
	{
		std::vector<double>pb;

		pb.resize(static_cast<std::vector<double, std::allocator<double>>::size_type>(range) + 1);
		for (size_t j = 0; j < static_cast<unsigned long long>(range) - 1; j++)
		{
			pb[j] = gps.bearing[j+i].x;
		}
		abweichungen[i] = calculateStandardDeviation(pb);
	}
}



double SegmentsRide::calculateStandardDeviation(const std::vector<double>& data)
{
	double sum = 0.0;
	double mean = 0.0;
	double standardDeviation = 0.0;

	for (size_t i = 0; i < data.size(); ++i)
	{
		sum += data[i];
	}

	mean = sum / data.size();

	for (size_t i = 0; i < data.size(); ++i)
	{
		standardDeviation += pow(data[i] - mean, 2);
	}

	return sqrt((standardDeviation) / data.size()) - mean;
}
