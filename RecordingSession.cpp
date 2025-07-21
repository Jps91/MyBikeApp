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

	const size_t dataSize = gpsData.rows[0].size();

	gPos.resizeAll(dataSize);
	gHight.resizeAll(dataSize);
	gSpeed.resizeAll(dataSize);
	gRot.resizeAll(dataSize);

	// Access columns once
	const auto& colTime = gpsData.rows[0];
	const auto& colLat = gpsData.rows[1];
	const auto& colLong = gpsData.rows[2];
	const auto& colHight = gpsData.rows[3];
	const auto& colSpeed = gpsData.rows[4];
	const auto& colYaw = gpsData.rows[5];
	const auto& colAccuracy = gpsData.rows[7];

	// Get raw data pointers for all vectors
	double* timePosPtr = gPos.time.data();
	double* latPtr = gPos.latitude.data();
	double* longPtr = gPos.longitude.data();

	double* timeHightPtr = gHight.time.data();
	double* hightPtr = gHight.hight.data();
	double* accVertPtr = gHight.gpsAccuracyVertical.data();

	double* timeSpeedPtr = gSpeed.time.data();
	double* speedPtr = gSpeed.speed.data();

	double* timeRotPtr = gRot.time.data();
	double* yawPtr = gRot.global_yaw.data();

#pragma omp parallel for schedule(static)
	for (size_t i = 0; i < dataSize; ++i)
	{
		// Use strtod for performance, avoid string_view or exceptions
		const double cacheTime = std::strtod(colTime[i].c_str(), nullptr);

		timePosPtr[i] = cacheTime;
		latPtr[i] = std::strtod(colLat[i].c_str(), nullptr);
		longPtr[i] = std::strtod(colLong[i].c_str(), nullptr);

		timeHightPtr[i] = cacheTime;
		hightPtr[i] = std::strtod(colHight[i].c_str(), nullptr);
		accVertPtr[i] = std::strtod(colAccuracy[i].c_str(), nullptr);

		timeSpeedPtr[i] = cacheTime;
		speedPtr[i] = std::strtod(colSpeed[i].c_str(), nullptr);

		timeRotPtr[i] = cacheTime;
		yawPtr[i] = std::strtod(colYaw[i].c_str(), nullptr);
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

	const size_t dataSize = acgData.rows[0].size(); // assumes all rows same size
	acg.resizeAll(dataSize);

	// Access columns directly
	const std::vector<std::string>& col0 = acgData.rows[0];
	const std::vector<std::string>& col1 = acgData.rows[1];
	const std::vector<std::string>& col2 = acgData.rows[2];
	const std::vector<std::string>& col3 = acgData.rows[3];

	double* timePtr = acg.time.data();
	double* xPtr = acg.x.data();
	double* yPtr = acg.y.data();
	double* zPtr = acg.z.data();

#pragma omp parallel for schedule(static)
	for (size_t i = 0; i < dataSize; ++i)
	{
		timePtr[i] = std::strtod(col0[i].c_str(), nullptr);
		xPtr[i] = std::strtod(col1[i].c_str(), nullptr);
		yPtr[i] = std::strtod(col2[i].c_str(), nullptr);
		zPtr[i] = std::strtod(col3[i].c_str(), nullptr);
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

	const size_t dataSize = gyroData.rows[0].size();

	gyro.resizeAll(dataSize);

	// Access columns once
	const auto& colTime = gyroData.rows[0];
	const auto& colRoll = gyroData.rows[1];
	const auto& colPitch = gyroData.rows[2];
	const auto& colYaw = gyroData.rows[3];

	// Get raw pointers to destination buffers
	double* timePtr = gyro.time.data();
	double* rollPtr = gyro.rollPerSecond.data();
	double* pitchPtr = gyro.pitchPerSecond.data();
	double* yawPtr = gyro.yawPerSecond.data();

#pragma omp parallel for schedule(static)
	for (size_t i = 0; i < dataSize; ++i)
	{
		timePtr[i] = std::strtod(colTime[i].c_str(), nullptr);
		rollPtr[i] = std::strtod(colRoll[i].c_str(), nullptr);
		pitchPtr[i] = std::strtod(colPitch[i].c_str(), nullptr);
		yawPtr[i] = std::strtod(colYaw[i].c_str(), nullptr);
	}
}

GYRO::~GYRO()
{
	;
}
