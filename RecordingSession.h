#pragma once
#include <string>
#include <stdio.h>
#include <vector>
#include <fstream>
#include <iostream>

#include "DataStructure.h"
#include "CSVImporter.h"

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
	GlobalPosition gPos;
	GlobalHight gHight;
	GlobalSpeed gSpeed;
	GlobalRotaion gRot;

public:
	GPS();
	GPS(std::string folderPath);
	~GPS();
private:
	std::string m_fileName = "Location.csv";	//TODO: Load Value form Settings
};

class ACG :public Sensor
{
public:
	LocalAcceleration acg;
public:
	ACG();
	ACG(std::string folderPath);
	~ACG();
private:
	std::string m_fileName = "Accelerometer.csv";	//Wrong Name leads to Errors!!
};

class GYRO :public Sensor
{
public:
	LocalRoationSpeed gyro;
public:
	GYRO();
	GYRO(std::string folderPath);
	~GYRO();
private:
	std::string m_fileName = "Gyroscope.csv";
};


class RecordingSession
{
public:
	GPS gpsSensor;
	ACG acgSensor;
	GYRO gyroSensor;
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

