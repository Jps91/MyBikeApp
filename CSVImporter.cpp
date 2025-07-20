#include "CSVImporter.h"

CSVImporter::CSVImporter()
{
	;
}

CSVData CSVImporter::importData(std::string fullFileName, std::string delimiterList)
{
	CSVData m_csvData{};
	m_fullFileName = fullFileName;
	m_inputFile.open(fullFileName, std::ios::in | std::ios::binary);
	if (!m_inputFile)
	{
		std::cerr << "ERROR: Could not open File: " << fullFileName << "\n";
		return m_csvData;
	}
	isHeadlinePresent();
	countLines();
	
	m_inputFile.clear();
	
	if (!delimiterList.empty())
	{
		m_ListOfDelimiter = delimiterList;
	}

	m_inputFile.seekg(0);
	std::string cachedLine;
		size_t headlineLineCount = 0;
	if (m_hasHeadLine)
	{
		std::getline(m_inputFile, cachedLine);
		headlineLineCount++;
		m_csvData.setRowLabels(parseLineToSegments(cachedLine));
	}

	for (size_t i = 0; i < m_csvData.rows.size(); i++)
	{
		m_csvData.rows[i].resize(m_lineCount - headlineLineCount);
	}

	size_t currentLineCount = 0;
	while (std::getline(m_inputFile, cachedLine))
	{
		std::vector<std::string>cachedSegments;
		cachedSegments = parseLineToSegments(cachedLine);
		if (cachedSegments.size() == m_csvData.rows.size())
		{
			for (size_t i = 0; i < cachedSegments.size()-headlineLineCount; i++)
			{
				m_csvData.rows[i][currentLineCount] = cachedSegments[i];
			}
		}
		currentLineCount++;
	}
	m_inputFile.seekg(0);
	m_inputFile.close();
	return m_csvData;
}


void CSVImporter::isHeadlinePresent()	//Side effect: sets the curso to the beginning
{
	m_inputFile.clear();
	if (!m_inputFile.is_open())
	{
		return;
	}
	m_inputFile.seekg(0);
	std::string cachedLine;
	std::getline(m_inputFile, cachedLine);

	if (cachedLine.empty())
	{
		std::cerr << "Error: The File is Empty: " << m_fullFileName << "\n";
		return;
	}

	if (std::isalpha(cachedLine[0]))
	{
		m_hasHeadLine=false;
	}
	else
	{
		m_hasHeadLine = true;
	}
	m_inputFile.seekg(0);
}

void CSVImporter::countLines()
{
	m_inputFile.clear();
	if (!m_inputFile.is_open())
	{
		return;
	}
	m_inputFile.seekg(0);
	std::string cacheLine;
	m_lineCount = 0;
	while (std::getline(m_inputFile, cacheLine))
	{
		m_lineCount++;
	}

	m_inputFile.seekg(0);
}

std::vector<std::string> CSVImporter::parseLineToSegments(std::string line)	//TODO: Faster by setting resizing first.
{
	std::vector<std::string>segments;
	std::string tempSegment{};
	size_t lineSize = line.size();
	for (size_t i = 0; i < lineSize; i++)
	{
		for (size_t d = 0; d < m_ListOfDelimiter.size(); d++)
		{
			if (line[i] == m_ListOfDelimiter[d])
			{
				segments.push_back(tempSegment);
				tempSegment.resize(0);
				break;
			}
			else
			{
				tempSegment += line[i];
				break;
			}
		}

	}
	if (tempSegment.size() != 0)
	{
		segments.push_back(tempSegment);
		tempSegment.resize(0);
	}
	return segments;
}

CSVImporter::~CSVImporter()
{

}

void CSVData::setRowLabels(std::vector<std::string> labelList)
{
	size_t rowCount = labelList.size();
	rowLabel.resize(rowCount);
	for (size_t i = 0; i < rowCount; i++)
	{
		rowLabel[i] = labelList[i];
	}
	rows.resize(rowCount);
}

void CSVData::print()
{
	if (rows[0].empty())
	{
		std::cerr << "Warining:	Could not Print DataRow is Empty";
		return;
	}
	for (size_t rowNumber = 0; rowNumber < rowLabel.size(); rowNumber++)
	{
		std::cout << rowLabel[rowNumber] << "	";
	}
	std::cout << "\n";
	for (size_t i = 0; i < rows[0].size(); i++)
	{
		for (size_t rowNumber = 0; rowNumber < rowLabel.size(); rowNumber++)
		{
			std::cout << rows[rowNumber][i] << "	";
		}
		std::cout << "\n";
	}
}