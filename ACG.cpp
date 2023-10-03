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

void ACG::correctMounting(double angleDegrees)
{
	xtrue.resize(entries + 1);
	ytrue.resize(entries + 1);
	ztrue.resize(entries + 1);
	for (size_t i = 0; i < entries; i++)
	{
		double angle = angleDegrees * (M_PI / 180.0);
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


ACG::~ACG()
{
	;
}
