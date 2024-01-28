#include "Position.h"





struct Coordinates {
	long double latitude;
	long double longitude;
	long double altitude;
};

long double calculateInclinationAngle(long double deltaAltitude, long double deltaDistance) {
	return atan(deltaAltitude / deltaDistance);
}

Coordinates calculateNextCoordinates(const Coordinates& current, long double bearing, long double speed, long double deltaTime, long double deltaAltitude)
{
	// Runge-Kutta method
	long double k1_lat = speed * deltaTime * cos(bearing);
	long double k1_lon = speed * deltaTime * sin(bearing);

	long double k2_lat = speed * deltaTime * cos(bearing + 0.5 * k1_lon);
	long double k2_lon = speed * deltaTime * sin(bearing + 0.5 * k1_lon);

	long double k3_lat = speed * deltaTime * cos(bearing + 0.5 * k2_lon);
	long double k3_lon = speed * deltaTime * sin(bearing + 0.5 * k2_lon);

	long double k4_lat = speed * deltaTime * cos(bearing + k3_lon);
	long double k4_lon = speed * deltaTime * sin(bearing + k3_lon);

	long double deltaLat = (k1_lat + 2 * k2_lat + 2 * k3_lat + k4_lat) / 6;
	long double deltaLon = (k1_lon + 2 * k2_lon + 2 * k3_lon + k4_lon) / 6;

	long double inclinationAngle = calculateInclinationAngle(deltaAltitude, 1.0);

	long double newLatitude = current.latitude + deltaLat + deltaAltitude * tan(inclinationAngle);
	long double newLongitude = current.longitude + deltaLon;
	long double newAltitude = current.altitude + deltaAltitude * cos(inclinationAngle);

	return { newLatitude, newLongitude, newAltitude };
}

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

		Coordinates currentLocation{ x[i - 1].x ,y[i - 1].x ,z[i - 1].x };
		Coordinates newLocation = calculateNextCoordinates(currentLocation, rot.yaw[gyroI].x, speed.speed[i].x / 111319.9, acg.time[i].x - acg.time[i - 1].x, 0);
		//IDK why, but when we use 1° ~= 111000m then it looks like it works

		x[i].x = newLocation.latitude;
		y[i].x = newLocation.longitude;
		z[i].x = newLocation.altitude;

		if (gpsI != oldgps)
		{
			oldgps = gpsI;
			x[i].x = gps.latitude[gpsI].x;
			y[i].x = gps.longitude[gpsI].x;
			/*
			std::cout << "---" << '\n';
			std::cout << x.at(i).x - x.at(i - 1).x << '\n';
			std::cout << y.at(i).x - y.at(i - 1).x << '\n';
			std::cout << '\n';
			//z[i].x = gps.altitude[gpsI].x;
			*/
		}
	}

	for (size_t gpsI = 1; gpsI < gps.entries - 2; gpsI++)
	{
		//A to B
		std::vector<_CRT_DOUBLE> aToBx;
		std::vector<_CRT_DOUBLE> aToBy;
		std::vector<_CRT_DOUBLE> aToBz;

		aToBx.resize(1);
		aToBy.resize(1);
		aToBz.resize(1);
		aToBx[0].x = gps.latitude[gpsI].x;
		aToBy[0].x = gps.longitude[gpsI].x;
		aToBz[0].x = gps.altitude[gpsI].x;

		size_t acgIAtoB = acg.findClosestElemen(gps.time[gpsI].x);
		while (acg.time[acgIAtoB].x < gps.time[gpsI + 1].x)
		{
			acgIAtoB++;
			size_t gyroI = gyro.findClosestElement(acg.time[acgIAtoB].x);
			Coordinates currentLocation{ aToBx.back().x ,aToBy.back().x ,aToBz.back().x };
			Coordinates newLocation = calculateNextCoordinates(currentLocation, rot.yaw[gyroI].x, speed.speed[acgIAtoB].x / 111319.9, acg.time[acgIAtoB].x - acg.time[acgIAtoB - 1].x, 0);

			_CRT_DOUBLE lat{};
			_CRT_DOUBLE lon{};
			_CRT_DOUBLE alt{};

			lat.x = newLocation.latitude;
			lon.x = newLocation.longitude;
			alt.x = newLocation.altitude;

			aToBx.push_back(lat);
			aToBy.push_back(lon);
			aToBz.push_back(alt);
		}

		//B to A
		std::vector<_CRT_DOUBLE> bToAx;
		std::vector<_CRT_DOUBLE> bToAy;
		std::vector<_CRT_DOUBLE> bToAz;
		
		bToAx.resize(1);
		bToAy.resize(1);
		bToAz.resize(1);
		bToAx[0].x = gps.latitude[gpsI+1].x;
		bToAy[0].x = gps.longitude[gpsI+1].x;
		bToAz[0].x = gps.altitude[gpsI+1].x;

		size_t acgIBtoA = acg.findClosestElemen(gps.time[gpsI].x + 1);

		while (acg.time[acgIBtoA].x > gps.time[gpsI].x)
		{
			acgIBtoA++;
			size_t gyroI = gyro.findClosestElement(acg.time[acgIBtoA].x);
			Coordinates currentLocation{ bToAx.back().x ,bToAy.back().x ,bToAz.back().x };
			Coordinates newLocation = calculateNextCoordinates(currentLocation, rot.yaw[gyroI].x, -speed.speed[acgIBtoA].x / 111319.9, acg.time[acgIBtoA].x - acg.time[acgIBtoA - 1].x, 0);

			_CRT_DOUBLE lat{};
			_CRT_DOUBLE lon{};
			_CRT_DOUBLE alt{};

			lat.x = newLocation.latitude;
			lon.x = newLocation.longitude;
			alt.x = newLocation.altitude;

			bToAx.push_back(lat);
			bToAy.push_back(lon);
			bToAz.push_back(alt);
		}

		std::cout << (bToAx.back().x - aToBx.at(0).x) * 111319.9 << std::endl;
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
