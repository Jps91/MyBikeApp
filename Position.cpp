#include "Position.h"

Position::Position()
{
}

Position::Position(GPS gps)
{
	size_t entries = gps.entries;
	time = gps.time;
	x.resize(entries);
	y.resize(entries);
	z.resize(entries);

	for (size_t i = 0; i < entries; i++)
	{
		auto result = gps_to_cartesian(gps.latitude[i].x, gps.longitude[i].x, gps.altitude[i].x);
		x[i].x = std::get<0>(result);
		y[i].x = std::get<1>(result);
		z[i].x = std::get<2>(result);
	}
}

std::tuple<double, double, double> Position::gps_to_cartesian(double latitude, double longitude, double altitude)
{
	double lat_rad = M_PI / 180.0 * latitude;
	double lon_rad = M_PI / 180.0 * longitude;

	// Calculate Cartesian coordinates
	double x = EarthRadius * cos(lat_rad) * cos(lon_rad);
	double y = EarthRadius * cos(lat_rad) * sin(lon_rad);
	double z = EarthRadius * sin(lat_rad) + altitude;

	return std::make_tuple(x, y, z);
}
