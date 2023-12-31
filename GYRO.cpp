#include "GYRO.h"
#include <filesystem>

GYRO::GYRO(std::string filePath)
{
	std::filesystem::path relativePath = "Data\\Gyroscope.csv";
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
	char value_x[64];
	char value_y[64];
	char value_z[64];
	char nothing[512];
	size_t i = 0;
	int retValue = 0;
	inputFile.getline(nothing, sizeof(nothing));
	while (inputFile.getline(value_time, sizeof(value_time), '	')
		&& inputFile.getline(value_x, sizeof(value_x), '	')
		&& inputFile.getline(value_y, sizeof(value_y), '	')
		&& inputFile.getline(value_z, sizeof(value_z), '\n'))
	{
		if (i < entries)
		{
			retValue = _atodbl(&time[i], value_time);
			retValue = _atodbl(&x[i], value_x);
			retValue = _atodbl(&y[i], value_y);
			retValue = _atodbl(&z[i], value_z);
		}
		i++;
	}
	inputFile.close();
}

void GYRO::correctMounting(double yaw, double roll, double pitch)
{
	x_true.resize(entries + 1);
	y_true.resize(entries + 1);
	z_true.resize(entries + 1);

	for (size_t i = 0; i < entries; i++)
	{
		x_true[i].x = x[i].x * cos(pitch) * cos(yaw) - y[i].x * cos(roll) * sin(yaw) + z[i].x * sin(roll) * cos(yaw);
		y_true[i].x = x[i].x * cos(pitch) * sin(yaw) + y[i].x * cos(roll) * cos(yaw) - z[i].x * sin(roll) * sin(yaw);
		z_true[i].x = -x[i].x * sin(pitch) * cos(yaw) + y[i].x * sin(roll) * sin(yaw) + z[i].x * cos(roll) * cos(pitch);
	}
}

void GYRO::rotationTrue()
{
	yaw.resize(entries + 1);
	pitch.resize(entries + 1);
	roll.resize(entries + 1);

	for (size_t i = 1; i < entries; i++)
	{
		pitch[i].x = x_true[i].x * (time[i].x - time[i - 1].x) + pitch[i - 1].x;

		roll[i].x = y_true[i].x * (time[i].x - time[i - 1].x) + roll[i - 1].x;
		
		yaw[i].x = z_true[i].x * (time[i].x - time[i - 1].x) + yaw[i - 1].x;
	}
}

void GYRO::rotation()
{
	yaw.resize(entries + 1);
	pitch.resize(entries + 1);
	roll.resize(entries + 1);

	for (size_t i = 1; i < entries; i++)
	{
		pitch[i].x = x[i].x * (time[i].x - time[i - 1].x) + pitch[i - 1].x;

		roll[i].x = y[i].x * (time[i].x - time[i - 1].x) + roll[i - 1].x;

		yaw[i].x = z[i].x * (time[i].x - time[i - 1].x) + yaw[i - 1].x;
	}

}

size_t GYRO::findClosestElement(double target)
{
	size_t begin = 0;
	size_t end = time.size() - 1;
	size_t i = 0;
	double value = target;
	i = (end - begin) / 2;
	while (end - begin != 1)
	{
		if (time[i].x > value)
		{
			end = i;
			i = i - (end - begin) / 2;
		}
		else if (time[i].x < value)
		{
			begin = i;
			i = i + (end - begin) / 2;
		}
	}
	if (i<0||i>time.size())
	{
		return 0;
	}
	return i;
}

GYRO::~GYRO()
{
}
