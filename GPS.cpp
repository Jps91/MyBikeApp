#include "GPS.h"

GPS::GPS()
{
	;
}
#include <filesystem>
GPS::GPS(std::string filePath)
{
	std::filesystem::path relativePath = "Data\\Location.csv";
	std::filesystem::path absolutePath = std::filesystem::absolute(relativePath);
	std::fstream inputFile{ absolutePath.string(), std::ios::in};
	if (!inputFile)
	{
		std::cerr << "Could not open: " << absolutePath.string() << std::endl;
	}

	char someData[256]{};
	while (inputFile.getline(someData, sizeof(someData)))
	{
		entries++;
	}
	inputFile.close();

	time.resize(entries + 1);
	latitude.resize(entries + 1);
	longitude.resize(entries + 1);
	altitude.resize(entries + 1);
	altitude_WGS84.resize(entries + 1);
	speed.resize(entries + 1);
	bearing.resize(entries + 1);
	distance.resize(entries + 1);
	horizontalAccuracy.resize(entries + 1);
	verticalAccuracy.resize(entries + 1);
	satellites.resize(entries + 1);

	inputFile.open(absolutePath.string(), std::ios::in);
	if (!inputFile)
	{
		std::cerr << "Could not open: " << absolutePath.string() << std::endl;
	}
	char value_time[64];
	char lat[64];
	char lon[64];
	char alt[64];
	char alt_WGS84[64];
	char spee[64];
	char bear[64];
	char dist[64];
	char hacc[64];
	char vacc[64];
	char sat[64];
	char nothing[256];
	size_t i = 0;
	int retValue = 0;
	inputFile.getline(nothing, sizeof(nothing));
	while (inputFile.getline(value_time, sizeof(value_time), '	')
		&& inputFile.getline(lat, sizeof(lat), '	')
		&& inputFile.getline(lon, sizeof(lon), '	')
		&& inputFile.getline(alt, sizeof(alt), '	')
		//&& inputFile.getline(alt_WGS84, sizeof(alt_WGS84), '	')
		&& inputFile.getline(spee, sizeof(spee), '	')
		&& inputFile.getline(bear, sizeof(bear), '	')
		//&& inputFile.getline(dist, sizeof(dist), '	')
		&& inputFile.getline(hacc, sizeof(hacc), '	')
		&& inputFile.getline(vacc, sizeof(vacc), '\n')
		//&& inputFile.getline(sat, sizeof(sat), '\n')
		)
	{
		if (i < entries)
		{
			retValue = _atodbl(&time[i], value_time);
			retValue = _atodbl(&latitude[i], lat);
			retValue = _atodbl(&longitude[i], lon);
			retValue = _atodbl(&altitude[i], alt);
			//retValue = _atodbl(&altitude_WGS84[i], alt_WGS84);
			retValue = _atodbl(&speed[i], spee);
			retValue = _atodbl(&bearing[i], bear);
			//retValue = _atodbl(&distance[i], dist);
			retValue = _atodbl(&horizontalAccuracy[i], hacc);
			retValue = _atodbl(&verticalAccuracy[i], vacc);
			//retValue = _atodbl(&satellites[i], sat);
		}
		i++;
	}
	inputFile.close();

	myDist.resize(entries + 1);
	const double R = 6378.137; // Radius of the Earth in kilometers

	for (size_t i = 1; i < entries; i++)
	{

		double dLat = (latitude[i].x * M_PI / 180) - (latitude[i - 1].x * M_PI / 180);
		double dLon = (longitude[i].x * M_PI / 180) - (longitude[i - 1].x * M_PI / 180);

		double a = sin(dLat / 2) * sin(dLat / 2) +
			cos(latitude[i - 1].x * M_PI / 180) * cos(latitude[i].x * M_PI / 180) *
			sin(dLon / 2) * sin(dLon / 2);

		double c = 2 * atan2(sqrt(a), sqrt(1 - a));
		double d = R * c;

		myDist[i].x = d * 1000 + myDist[i - 1].x; // meters
	}


}

size_t GPS::getIndexByTime(double timestamp)
{
	if (timestamp > time[entries - 2].x)
	{
		return entries;
	}
	for (size_t i = 0; i < entries; i++)
	{
		if (time[i].x > timestamp)
		{
			return i;
		}
	}

	return 1;
}

void GPS::terrain()
{
	std::fstream file{"C:\\1_Jan\\DataServerClient\\Projekte\\BikeApp\\SensorBox\\ENDLESS_23_06_2023_16_27_03\\GPS_Log.csv", std::ios::trunc | std::ios::out};
	int loadingbar = round(static_cast<float>(entries) / 100);
	for (size_t i = 0; i < entries - 1; i++)
	{
		file << latitude[i].x << " " << longitude[i].x << " " << altitude[i].x << std::endl;


		if (i % loadingbar == 0)
		{
			system("CLS");
			std::cout << "Saving GPS_log.csv File: " << 100 * static_cast<float>(i) / entries << "%";
		}
	}
	file.close();
}

double GPS::fitLineToPointsAngle(const std::vector<double>& xValues, const std::vector<double>& yValues)
{
	if (xValues.size() != yValues.size())
	{
		return 0.0;
	}

	double xMean = 0.0;
	double yMean = 0.0;

	for (size_t i = 0; i < xValues.size(); ++i)
	{
		xMean += xValues[i];
		yMean += yValues[i];
	}

	xMean /= xValues.size();
	yMean /= yValues.size();

	double numerator = 0.0;
	double denominator = 0.0;

	for (size_t i = 0; i < xValues.size(); ++i)
	{
		numerator += (xValues[i] - xMean) * (yValues[i] - yMean);
		denominator += (xValues[i] - xMean) * (xValues[i] - xMean);
	}

	if (denominator != 0.0)
	{
		double slope = numerator / denominator;
		double intercept = yMean - slope * xMean;
		double angleRadians = std::atan(slope);
		double angleDegrees = angleRadians * (180.0 / 3.1415);
		return angleDegrees;
	}
	else
	{
		return 0.0;
	}


}

GPS::~GPS()
{
}
