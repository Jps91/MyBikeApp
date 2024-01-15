#include "ACG.h"

ACG::ACG()
{
	;
}

ACG::ACG(std::string filepath)
{
	std::filesystem::path relativePath = "Data\\Accelerometer.csv";
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
	x.resize(entries + 1);
	y.resize(entries + 1);
	z.resize(entries + 1);

	inputFile.open(absolutePath.string(), std::ios::in);
	if (!inputFile)
	{
		std::cerr << "Could not open: " << absolutePath.string() << std::endl;
	}
	char value_time[64];
	char xa[64];
	char ya[64];
	char za[64];
	char nothing[256];
	size_t i = 0;
	int retValue = 0;
	inputFile.getline(nothing, sizeof(nothing));
	while (inputFile.getline(value_time, sizeof(value_time), '	')
		&& inputFile.getline(xa, sizeof(xa), '	')
		&& inputFile.getline(ya, sizeof(ya), '	')
		&& inputFile.getline(za, sizeof(za), '\n'))
	{
		if (i < entries)
		{
			retValue = _atodbl(&time[i], value_time);
			retValue = _atodbl(&x[i], xa);
			retValue = _atodbl(&y[i], ya);
			retValue = _atodbl(&z[i], za);
		}
		i++;
	}
	inputFile.close();

}

void ACG::smoothIt()
{
	std::filesystem::path relativePath = "Data\\Accelerometer_out.csv";
	std::filesystem::path absolutePath = std::filesystem::absolute(relativePath);
	std::fstream outputFile{ absolutePath.string(), std::ios::out | std::ios::trunc};
	if (!outputFile)
	{
		std::cerr << "Could not open: " << absolutePath.string() << std::endl;
	}
	else
	{
		int smoothness = 75;
		FastAverageDouble xAverage(smoothness);
		FastAverageDouble yAverage(smoothness);
		FastAverageDouble zAverage(smoothness);

		xm.resize(entries + 1);
		ym.resize(entries + 1);
		zm.resize(entries + 1);

		for (size_t i = 0; i < entries; i++)
		{

			double xn = xAverage.additionalValue(xtrue[i].x);
			double yn = yAverage.additionalValue(ytrue[i].x);
			double zn = zAverage.additionalValue(ztrue[i].x);
			outputFile << time[i].x << "	" << xn << "	" << yn << "	" << zn << std::endl;
			xm[i].x = xn;
			ym[i].x = yn;
			zm[i].x = zn;
		}
		outputFile.close();
	}


}

void ACG::correctMounting()
{
	if (mountigAngleDegree==0)
	{
		findRoationAngle();
	}
	xtrue.resize(entries + 1);
	ytrue.resize(entries + 1);
	ztrue.resize(entries + 1);
	double angle = mountigAngleDegree * (M_PI / 180.0);
	for (size_t i = 0; i < entries; i++)
	{
		// Speichern Sie die ursprünglichen Koordinaten
		double originalX = x[i].x;
		double originalY = y[i].x;
		double originalZ = z[i].x;

		// Rotation durchführen
		xtrue[i].x = originalX;
		ytrue[i].x = originalY * cos(angle) - originalZ * sin(angle);
		ztrue[i].x = originalY * sin(angle) + originalZ * cos(angle);
	}
}

size_t ACG::findClosestElemen(double target)
{

	int left = 0;
	int right = entries - 1;
	double closestElement = 0.0;
	double closestDifference = std::abs(target - time[0].x);

	while (left <= right)
	{
		int mid = left + (right - left) / 2;
		double currentElement = time[mid].x;
		double currentDifference = std::abs(target - currentElement);

		if (currentDifference < closestDifference) {
			closestDifference = currentDifference;
			closestElement = currentElement;
		}

		if (currentElement < target) {
			left = mid + 1;
		}
		else if (currentElement > target) {
			right = mid - 1;
		}
		else {
			return currentElement; // Das Ziel wurde gefunden
		}
	}

	return closestElement;

}

void ACG::filter()
{
	xfilter.resize(entries);
	FrequnceTimeFilter tFilter(0.5, 0.8);
	FrequenceAmplitudeFilter fFilter(0.0, 0.1);
	averageFilter avFilter{};
	avFilter.filter(x);
	for (size_t i = 1; i < entries; i++)
	{
		//xfilter[i].x = tFilter.filter(time[i].x - time[i - 1].x, x[i].x) + fFilter.filter(time[i].x - time[i - 1].x, x[i].x);
		xfilter[i].x = x[i].x;
	}
}

double ACG::findRoationAngle()
{
	mountigAngleDegree = bisearch(-30, 30, 0.0005, 0);
	return 0.0;
}

double ACG::getMountigAngleDegree()
{
	if (mountigAngleDegree == 0)
	{
		findRoationAngle();
	}
	return mountigAngleDegree;
}

ACG::~ACG()
{
	;
}

double ACG::createRotationTestResult(double angleDegrees)
{
	std::vector<_CRT_DOUBLE> xCache;
	std::vector<_CRT_DOUBLE> yCache;
	std::vector<_CRT_DOUBLE> zCache;

	xCache.resize(entries);
	yCache.resize(entries);
	zCache.resize(entries);

	double angle = angleDegrees * (M_PI / 180.0);
	for (size_t i = 0; i < entries; i++)
	{
		xCache[i].x = x[i].x;
		yCache[i].x = y[i].x * cos(angle) - z[i].x * sin(angle);
		zCache[i].x = y[i].x * sin(angle) + z[i].x * cos(angle);
	}

	FastAverageDouble av{};
	double result = 0;
	for (size_t j = 0; j < entries; j++)
	{
		result = av.addValueNoFilter(yCache[j].x);
	}
	av.~FastAverageDouble();
	return result;
}

double ACG::bisearch(double lower_bound, double upper_bound, double tolerance, double target_value)	//if it is stuck in the while Loop, then try increasing return vlaue of createRotationTestResult or check if it is continuesly rising or falling, and if the target Value is in range of the tolerance 
{
	double result_middle = 0;
	double middle = 0;
	while ((upper_bound - lower_bound) > tolerance)
	{
		middle = (lower_bound + upper_bound) / 2;

		result_middle = createRotationTestResult(middle);

		if (result_middle == target_value)
		{
			return middle;
		}
		else if (result_middle * createRotationTestResult(lower_bound) < target_value)
		{
			upper_bound = middle;
		}
		else
		{
			lower_bound = middle;
		}
	}

	return middle;
}