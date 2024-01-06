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
			std::cout << "---" << '\n';
			std::cout << x[i].x - x[i - 1].x << '\n';
			std::cout << y[i].x - y[i - 1].x << '\n';
			std::cout << '\n';
			//z[i].x = gps.altitude[gpsI].x;
		}

		/*
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

		//x[i].x = x[i - 1].x + speed.filtertspeed[i].x * delta_t * cos(direction_rad);
		//y[i].x = y[i - 1].x + speed.filtertspeed[i].x * delta_t * sin(direction_rad);


		if (gpsI != oldgps)
		{
			oldgps = gpsI;
			x[i].x = xd;
			y[i].x = yd;
		}
		x[i].x = xd;
		y[i].x = yd;

		z[i].x = zd;
		*/
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



/*
	// Example usage
	Coordinates currentLocation = { 37.7749, -122.4194, 0 }; // San Francisco coordinates
	double bearing = 45.0; // Example bearing in degrees
	double acceleration = 9.8; // Example acceleration in m/s^2 (approximate gravity on Earth)
	double deltaTime = 10.0; // Example time interval in seconds
	double inclinationAngle = atan(0.2); // Example inclination angle in radians (adjust as needed)

	Coordinates nextLocation = calculateNextCoordinates(currentLocation, bearing, acceleration, deltaTime, inclinationAngle);

	std::cout << "Next GPS coordinates after " << deltaTime << " seconds:\n";
	std::cout << "Latitude: " << nextLocation.latitude << "\n";
	std::cout << "Longitude: " << nextLocation.longitude << "\n";
	std::cout << "Altitude: " << nextLocation.altitude << "\n";

	return 0;
	*/
