#pragma once
#pragma once
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <cctype>

struct CSVData
{
	std::vector<std::string>rowLabel{};
	std::vector<std::vector<std::string>>rows;

	void setRowLabels(std::vector<std::string>labelList);
	void print();
	size_t dataLines{};
	size_t rowCount{};
	void setAllSize(const size_t rows, const size_t lines);
};

class CSVImporter
{
public:
	CSVImporter();
	CSVData importData(std::string fullFileName, std::string listOfDelimiter);
	CSVData importDataV2(const std::string& fullFileName, const std::string& listOfDelimiter);
	CSVData importDataV3(const std::string& fullFileName, const std::string& listOfDelimiter);
	~CSVImporter();


private:
	std::string m_fullFileName{};
	std::ifstream m_inputFile;

	void isHeadlinePresent();
	bool m_hasHeadLine = true;

	void countLines();
	size_t m_lineCount = 0;

	std::string m_ListOfDelimiter = "	;\n";
	std::vector<std::string> parseLineToSegments(const std::string line);
	std::vector<std::string> parseLineToSegmentsV2(const std::string& line);
	std::vector<std::string> parseLineToSegmentsV3(const std::string_view& line);

	//TODO: Remove / deal with Comments
	//TODO: Input Validation
};

