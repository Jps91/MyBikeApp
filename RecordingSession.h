#pragma once
#include <string>
#include <stdio.h>
#include <vector>
#include <fstream>
#include <iostream>

#include "DataStructure.h"

//TODO: Maybe Add Config File for (defalut) settings importing

class Sensor
{
public:
	Sensor();
	bool setSensorFolder(std::string folderPath);
	std::string getSensorFolder();
	~Sensor();

private:
	std::string m_sensorFolder = { "" };

};

class GPS :public Sensor
{
public:
	GPS();
	GPS(std::string fullFilePath);
	~GPS();
private:
	std::string m_sensorName = "GPS";
	std::string m_fileName = "Location.csv";
	const size_t m_numberOfDataRows = 8;				//counted from CSV File
	const std::string m_delimiter = "	";
	const std::string m_newlineCharacter = "\n";
	const size_t m_DataStartOffset = 1;

	Measurements2D m_worldPosition;
	Measurements1D m_worldPositionAccuracy;
	Measurements1D m_hight;
	Measurements1D m_hightAccuracy;
	Measurements1D m_velocity;
	Measurements1D m_bearing;
};


class RecordingSession
{
public:
	RecordingSession();
	void initialize(std::string inputFolderPath, std::string OutputfolderPath);

	bool setCSVInputFolder(std::string folderPath);		//is kind of useless, when the ,path is set, but you can first set it here and reference it later.
	std::string getCSVInputFolder();

	bool setCSVOutputFolder(std::string folderPath);	//E.g.: C:\\temp\ //LOL if you end a comment with a \ it breaks the code.
	std::string getCSVOutputFolder();

	~RecordingSession();

private:
	std::string m_CSVOutputFolderName{ "" };
	std::string m_CSVInputFolderName{ "" };

};

