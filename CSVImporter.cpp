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

	m_inputFile.seekg(0, std::ios::beg);
	std::string cachedLine;
	size_t newRowSize= m_lineCount;
	if (m_hasHeadLine)
	{
		std::getline(m_inputFile, cachedLine);
		newRowSize -=1;			//because we skipp the first line for the Data
		m_csvData.setRowLabels(parseLineToSegments(cachedLine));
	}

	
	for (size_t i = 0; i < m_csvData.rows.size(); i++)
	{
		m_csvData.rows[i].resize(newRowSize);
	}

	size_t currentLineCount = 0;
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
	m_inputFile.seekg(0, std::ios::beg);
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
	m_inputFile.seekg(0, std::ios::beg);
	std::string cachedLine;
	std::getline(m_inputFile, cachedLine);

	if (cachedLine.empty())
	{
		std::cerr << "Error: The File is Empty: " << m_fullFileName << "\n";
		return;
	}

	m_hasHeadLine = !std::isalpha(cachedLine[0]);
	m_inputFile.seekg(0, std::ios::beg);
}

void CSVImporter::countLines()
{
	m_inputFile.clear();
	if (!m_inputFile.is_open())
	{
		return;
	}
	m_inputFile.seekg(0, std::ios::beg);
	m_lineCount = 0;
	std::string cacheLine;
	while (std::getline(m_inputFile, cacheLine))
	{
		++m_lineCount;
	}

	m_inputFile.seekg(0, std::ios::beg);

	
}

std::vector<std::string> CSVImporter::parseLineToSegments(std::string line)	//TODO: Faster by setting resizing first.
{
	std::vector<std::string> segments;
	std::string tempSegment;

	// Optional: reserve estimated capacity to reduce reallocations
	size_t estimatedSegments = 1;
	for (char delim : m_ListOfDelimiter)
		estimatedSegments += std::count(line.begin(), line.end(), delim);
	segments.reserve(estimatedSegments);

	for (char ch : line)
	{
		// Check if current char is any delimiter
		bool isDelimiter = false;
		for (char delim : m_ListOfDelimiter)
		{
			if (ch == delim)
			{
				isDelimiter = true;
				break;
			}
		}

		if (isDelimiter)
		{
			segments.push_back(std::move(tempSegment)); // move for efficiency
			tempSegment.clear();
		}
		else
		{
			tempSegment.push_back(ch);
		}
	}

	if (!tempSegment.empty())
	{
		segments.push_back(std::move(tempSegment));
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