// MyBikeApp.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <filesystem>
#include <iomanip>

#include "DataStructure.h"
#include "RecordingSession.h"
#include "CSVImporter.h"


int main()
{
	std::string folder = "C:\\temp\\MyBikeApp\\DemoData\\";
	std::string filenameGPS = "Location.csv";
	std::string filenameACG = "Accelerometer.csv";
	std::string filenameGYRO = "Gyroscope.csv";

	RecordingSession track;
	track.initialize(folder, folder + "Output\\");


	

	CSVImporter csvFiles;
	
	CSVData csvDataACG;
	CSVData csvDataGPS;
	csvDataACG = csvFiles.importData(folder + filenameACG,"");
	csvDataGPS = csvFiles.importData(folder + filenameGPS,"");
	
	//csvDataACG.print();
	csvDataGPS.print();


	return 0;
}
