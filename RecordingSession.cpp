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
	CSVData gpsData;
	gpsData = importGPS.importData(folderPath + m_fileName, "");

	size_t dataSize = gpsData.rows[0].size();

	gPos.resizeAll(dataSize);
	gHight.resizeAll(dataSize);
	gSpeed.resizeAll(dataSize);
	gRot.resizeAll(dataSize);

	for (size_t i = 0; i < gpsData.rows[0].size(); i++)
	{
		double cacheTime = std::stod(gpsData.rows[0][i]);

		gPos.time[i] = cacheTime;
		gPos.latitude[i] = std::stod(gpsData.rows[1][i]);
		gPos.longitude[i] = std::stod(gpsData.rows[2][i]);

		gHight.time[i] = cacheTime;
		gHight.hight[i] = std::stod(gpsData.rows[3][i]);
		gHight.gpsAccuracyVertical[i] = std::stod(gpsData.rows[7][i]);

		gSpeed.time[i] = cacheTime;
		gSpeed.speed[i] = std::stod(gpsData.rows[4][i]);

		gRot.time[i] = cacheTime;
		gRot.global_yaw[i] = std::stod(gpsData.rows[5][i]);
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
	CSVData acgData;
	acgData = importACG.importData(folderPath + m_fileName, "");

	size_t dataSize = acgData.rows[0].size();

	acg.resizeAll(dataSize);

	for (size_t i = 0; i < acgData.rows[0].size(); i++)
	{
		acg.time[i] = std::stod(acgData.rows[0][i]);
		acg.x[i] = std::stod(acgData.rows[1][i]);
		acg.y[i] = std::stod(acgData.rows[2][i]);
		acg.z[i] = std::stod(acgData.rows[3][i]);
	}
}

ACG::~ACG()
{
	;
}
