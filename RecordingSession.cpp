#include "RecordingSession.h"

//TODO: Input Validation and Improved Input adaptation


RecordingSession::RecordingSession()
{
}

void RecordingSession::initialize(std::string inputFolderPath, std::string OutputfolderPath)
{
	setCSVInputFolder(inputFolderPath);
	setCSVOutputFolder(OutputfolderPath);

	GPS tempGPSSensor(inputFolderPath);
	gpsSensor = tempGPSSensor;

	ACG tempACGSensor(inputFolderPath);
	acgSensor = tempACGSensor;

	GYRO tempGyroSensor(inputFolderPath);
	gyroSensor = tempGyroSensor;
}

bool RecordingSession::setCSVInputFolder(std::string folderPath)
{
	m_CSVInputFolderName = folderPath;

	return true;
}

std::string RecordingSession::getCSVInputFolder()
{
	return m_CSVInputFolderName;
}

bool RecordingSession::setCSVOutputFolder(std::string folderPath)
{
	//TODO: Input Validation and Improved Input adaptation
	m_CSVOutputFolderName = folderPath;
	return true;
}

std::string RecordingSession::getCSVOutputFolder()
{
	return m_CSVOutputFolderName;
}


RecordingSession::~RecordingSession()
{

}

Sensor::Sensor()
{
	;
}

bool Sensor::setSensorFolder(std::string folderPath)
{
	if (false)
	{
		return false;
	}
	m_sensorFolder = folderPath;
	return true;
}

std::string Sensor::getSensorFolder()
{
	return m_sensorFolder;
}

Sensor::~Sensor()
{
	;
}

GPS::GPS()
{
	;
}

GPS::GPS(std::string folderPath)
{
	CSVImporter importGPS;
	CSVData gpsData = importGPS.importDataV3(folderPath + m_fileName, "");

	position_Anlge.resize(gpsData.valueLineCount);
	bearing_degree.resize(gpsData.valueLineCount);
	verticalAccuracy_meter.resize(gpsData.valueLineCount);
	horizontalAccuracy_meter.resize(gpsData.valueLineCount);
	speed_meterPerSecond.resize(gpsData.valueLineCount);

	for (size_t i = 0; i < gpsData.valueLineCount; i++)
	{
		const double cacheTime = std::strtod(gpsData.value[0][i].c_str(), nullptr);

		position_Anlge[i].time = cacheTime;
		position_Anlge[i].latitude = std::strtod(gpsData.value[1][i].c_str(), nullptr);
		position_Anlge[i].longitude = std::strtod(gpsData.value[2][i].c_str(), nullptr);
		position_Anlge[i].hight = std::strtod(gpsData.value[3][i].c_str(), nullptr);

		speed_meterPerSecond[i].time = cacheTime;
		speed_meterPerSecond[i].time = std::strtod(gpsData.value[4][i].c_str(), nullptr);

		bearing_degree[i].time = cacheTime;
		bearing_degree[i].time = std::strtod(gpsData.value[5][i].c_str(), nullptr);

		horizontalAccuracy_meter[i].time = cacheTime;
		horizontalAccuracy_meter[i].time = std::strtod(gpsData.value[6][i].c_str(), nullptr);

		verticalAccuracy_meter[i].time = cacheTime;
		verticalAccuracy_meter[i].accuracy = std::strtod(gpsData.value[7][i].c_str(), nullptr);
	}
}


GPS::~GPS()
{
	;
}

ACG::ACG()
{
	;
}

ACG::ACG(std::string folderPath)
{
	CSVImporter importACG;
	CSVData acgData = importACG.importDataV3(folderPath + m_fileName, "");

	accelerationMeterPerSecond.resize(acgData.valueLineCount);
#pragma omp parallel for schedule(static)
	for (size_t i = 0; i < acgData.valueLineCount; i++)
	{
		accelerationMeterPerSecond[i].time = std::strtod(acgData.value[0][i].c_str(), nullptr);
		accelerationMeterPerSecond[i].x = std::strtod(acgData.value[1][i].c_str(), nullptr);
		accelerationMeterPerSecond[i].y = std::strtod(acgData.value[2][i].c_str(), nullptr);
		accelerationMeterPerSecond[i].z = std::strtod(acgData.value[3][i].c_str(), nullptr);
	}
}

ACG::~ACG()
{
	;
}

GYRO::GYRO()
{
	;
}

GYRO::GYRO(std::string folderPath)
{
	CSVImporter importGYRO;
	CSVData gyroData = importGYRO.importDataV3(folderPath + m_fileName, "");

	rotation_RadiansPerSecond.resize(gyroData.valueLineCount);
#pragma omp parallel for schedule(static)
	for (size_t lineNumber = 0; lineNumber < gyroData.valueLineCount; lineNumber++)
	{
		rotation_RadiansPerSecond[lineNumber].time = std::strtod(gyroData.value[0][lineNumber].c_str(), nullptr);
		rotation_RadiansPerSecond[lineNumber].roll = std::strtod(gyroData.value[1][lineNumber].c_str(), nullptr);
		rotation_RadiansPerSecond[lineNumber].pitch = std::strtod(gyroData.value[2][lineNumber].c_str(), nullptr);
		rotation_RadiansPerSecond[lineNumber].yaw = std::strtod(gyroData.value[3][lineNumber].c_str(), nullptr);
	}
}

GYRO::~GYRO()
{
	;
}
