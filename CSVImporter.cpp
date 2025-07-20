#include "CSVImporter.h"

CSVImporter::CSVImporter(std::string fullFileName)
{
	m_fullFileName = fullFileName;
	m_inputFile.open(fullFileName, std::ios::in | std::ios::binary);
	if (!m_inputFile)
	{
		std::cerr << "ERROR: Could not open File: " << fullFileName << "\n";
		return;
	}
	isHeadlinePresent();
	countLines();
}

void CSVImporter::importData(std::string delimiterList)
{
	if (!m_inputFile.is_open())
	{
		return;
	}
	if (!delimiterList.empty())
	{
		m_ListOfDelimiter = delimiterList;
	}

	m_inputFile.seekg(0);
	std::string cachedLine;
	size_t currentLineCount = 0;
	if (m_hasHeadLine)
	{
		std::getline(m_inputFile, cachedLine);
		currentLineCount++;
		m_csvData.setRowLabels(parseLineToSegments(cachedLine));
	}

	for (size_t i = 0; i < m_csvData.rows.size(); i++)
	{
		m_csvData.rows[i].resize(m_lineCount - currentLineCount);
	}


	while (std::getline(m_inputFile, cachedLine))
	{
		std::vector<std::string>cachedSegments;
		cachedSegments = parseLineToSegments(cachedLine);
		if (cachedSegments.size() == m_csvData.rows.size())
		{
			for (size_t i = 0; i < cachedSegments.size(); i++)
			{
				m_csvData.rows[i][currentLineCount] = cachedSegments[i];
			}
		}
		currentLineCount++;
	}
	m_inputFile.seekg(0);
	m_inputFile.close();
}

void CSVImporter::isHeadlinePresent()	//Side effect: sets the curso to the beginning
{
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
		m_hasHeadLine;
	}
	m_inputFile.seekg(0);
}

void CSVImporter::countLines()
{
	if (!m_inputFile.is_open())
	{
		return;
	}
	m_inputFile.seekg(0);
	std::string cacheLine;

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
