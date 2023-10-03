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

double ACG::correctMountingresult(double steps)
{

	std::vector<double>result;
	result.resize(steps + 1);
	for (size_t i = 0; i < steps; i++)
	{
		correctMounting(static_cast<double>(40) / i);			//40=Max Angle
		FastAverageDouble av;
		for (size_t j = 0; j < entries; j++)
		{
			result[i] = av.addValueNoFilter(xtrue[j].x + ytrue[j].x + ztrue[j].x);
		}
		std::cout << result[i] << std::endl;
	}
	double res = DBL_MAX;
	double gravity = 9.806;

	int n = steps-1;
	double konstante = 9.806;  // Die Konstante, zu der Sie die nächstgelegene Zahl finden möchten
	double naechste_zahl = result[0];  // Annahme: Das erste Array-Element ist vorläufig das Nächste.

	double kleinste_differenz = std::abs(konstante - result[0]); // Die vorläufige Differenz
	size_t pointer = 0;
	for (int i = 1; i < n; i++) {
		double differenz = std::abs(konstante - result[i]);
		if (differenz < kleinste_differenz) {
			kleinste_differenz = differenz;
			naechste_zahl = result[i];
			pointer = i;
		}
	}
	//std::cout << "Nächste Zahl:	" << naechste_zahl << std::endl;
	return pointer;
}
ACG::~ACG()
{
	;
}
