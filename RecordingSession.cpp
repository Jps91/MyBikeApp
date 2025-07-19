#include "RecordingSession.h"

//TODO: Input Validation and Improved Input adaptation


RecordingSession::RecordingSession()
{
}

void RecordingSession::initialize(std::string inputFolderPath, std::string OutputfolderPath)
{
	setCSVInputFolder(inputFolderPath);
	setCSVOutputFolder(OutputfolderPath);
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

GPS::GPS(std::string fullFilePath)
{
	std::fstream inputFile(fullFilePath, std::ios::in | std::ios::binary);
	if (!inputFile)
	{
		std::cout << "ERROR: Could not open File: " << fullFilePath << "\n";
	}
	std::string tempLine;
	tempLine.reserve(128);
	size_t lineCount{};
	while (std::getline(inputFile, tempLine))
	{
		lineCount++;
	}
	inputFile.close();

	m_worldPosition.resize(lineCount);
	m_worldPosition.unit = DegreesLatitude;
	m_worldPositionAccuracy.resize(lineCount);
	m_worldPositionAccuracy.unit = DegreesLongitude;
	m_hight.resize(lineCount);
	m_hight.unit = Meters;
	m_hightAccuracy.resize(lineCount);
	m_hightAccuracy.unit = Meters;
	m_velocity.resize(lineCount);
	m_velocity.unit = MetersPerSecond;
	m_bearing.resize(lineCount);
	m_bearing.unit = Degrees360;

	inputFile.open(fullFilePath, std::ios::in | std::ios::binary);


	if (lineCount<=m_DataStartOffset)
	{
		std::cout << "Error: DataStartOffset ist larger then the File\n";
	}

	for (size_t i = 0; i < m_DataStartOffset; i++)
	{
		std::getline(inputFile, tempLine);
	}
	std::vector<std::string>segments{};
	size_t measuringIndex = 0;

	while (std::getline(inputFile, tempLine))
	{
		std::string tempSegment{};
		size_t lineSize = tempLine.size();
		for (size_t i = 0; i < lineSize; i++)
		{
			
			if (tempLine[i] == m_delimiter[0] || tempLine[i] == m_newlineCharacter[0])
			{
				segments.push_back(tempSegment);
				tempSegment.resize(0);
			}
			else
			{
				tempSegment += tempLine[i];
			}
		}
		if (tempSegment.size() != 0)
		{
			segments.push_back(tempSegment);
			tempSegment.resize(0);
		}
		if (segments.size() < 7)
		{
			std::cerr << "ERROR: In the File is missing a DataRow, or Value: " << fullFilePath <<"	"<<segments.size() << "\n";
			return;
		}
		double tempTime = std::stod(segments[0]);
		m_worldPosition.v[measuringIndex].set(std::stod(segments[1]), tempTime);
		m_worldPosition.w[measuringIndex].set(std::stod(segments[2]), tempTime);
		m_hight.u[measuringIndex].set(std::stod(segments[3]), tempTime);
		m_velocity.u[measuringIndex].set(std::stod(segments[4]), tempTime);
		m_bearing.u[measuringIndex].set(std::stod(segments[5]), tempTime);
		m_worldPositionAccuracy.u[measuringIndex].set(std::stod(segments[6]), tempTime);
		m_hightAccuracy.u[measuringIndex].set(std::stod(segments[7]), tempTime);
		segments.resize(0);

		measuringIndex++;
	}
	inputFile.close();
}

GPS::~GPS()
{
	;
}
