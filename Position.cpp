#include "Position.h"

Position::Position()
{
}

Position::Position(GPS gps, ACG acg, Rotation rot)
{
	size_t entries = acg.entries;
	time = acg.time;
	x.resize(entries);
	y.resize(entries);
	z.resize(entries);

	const double EARTH_RADIUS = 6371000.0;
	const double DEG_TO_RAD = 0.017453292519943295;

	for (size_t i = 0; i < entries; i++)
	{
		size_t gpsI = gps.getIndexByTime(acg.time[i].x);
		x[i].x = EARTH_RADIUS * gps.longitude[gpsI].x * DEG_TO_RAD;
		y[i].x = EARTH_RADIUS * log(tan((90.0 + gps.latitude[gpsI].x) * DEG_TO_RAD / 2.0));
		z[i].x = gps.altitude[gpsI].x;
	}
}
