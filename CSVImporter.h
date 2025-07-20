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
};

class CSVImporter
{
public:
	CSVImporter(std::string fullFileName);
	void importData(std::string listOfDelimiter);
	~CSVImporter();
private:
	std::string m_fullFileName{};
	std::ifstream m_inputFile;

	void isHeadlinePresent();
	bool m_hasHeadLine=true;

	void countLines();
	size_t m_lineCount=0;

	std::string m_ListOfDelimiter = "	;\n";
	std::vector<std::string> parseLineToSegments(std::string line);
	


	CSVData m_csvData{};
	//TODO: Remove / deal with Comments
	//TODO: Input Validation
};

