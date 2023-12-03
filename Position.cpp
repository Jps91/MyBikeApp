#include "Position.h"

Position::Position()
{
}

Position::Position(GPS gps)
{
	GYRO gyro("");
	ACG acg("");
	Speed speed(acg);
	speed.filter();
	Rotation rot(acg);

	size_t entries = acg.entries - 1;
	x.resize(entries + 1);
	y.resize(entries + 1);
	z.resize(entries + 1);
	size_t oldgps = 0;
	double xcorr = 0;
	double ycorr = 0;
	for (size_t i = 1; i < entries - 1; i++)
	{
		size_t gpsI = gps.findClosestElement(acg.time[i].x);
		size_t gyroI = gyro.findClosestElement(acg.time[i].x);
		if (gpsI < 0 || gpsI > gps.time.size() - 1 || gyroI < 0 || gyroI > gyro.time.size() - 1)
		{
			break;
		}
		auto result = gps_to_cartesian(gps.latitude[gpsI].x, gps.longitude[gpsI].x, gps.altitude[gpsI].x);
		double xd = std::get<0>(result);
		double yd = std::get<1>(result);
		double zd = std::get<2>(result);
		double delta_t = speed.time[i].x - speed.time[i - 1].x;
		
		double direction_rad = (rot.yaw[gyroI].x);	
		x[i].x = x[i - 1].x + speed.filtertspeed[i].x * delta_t * cos(direction_rad);
		y[i].x = y[i - 1].x + speed.filtertspeed[i].x * delta_t * sin(direction_rad);

		if (gpsI != oldgps)
		{
			oldgps = gpsI;
			x[i].x = xd;
			y[i].x = yd;
		}


		z[i].x = zd;
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
