#include "CSVImporter.h"
#include <omp.h>

CSVImporter::CSVImporter()
{
	;
}

CSVData CSVImporter::importData(std::string fullFileName, std::string delimiterList)	//empty delimiter =>tab,;,\n
{
	CSVData csvData{};
	m_fullFileName = fullFileName;
	m_inputFile.open(fullFileName, std::ios::in | std::ios::binary);
	if (!m_inputFile)
	{
		std::cerr << "ERROR: Could not open File: " << fullFileName << "\n";
		return csvData;
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
	size_t newRowSize = m_lineCount;
	if (m_hasHeadLine)
	{
		std::getline(m_inputFile, cachedLine);
		newRowSize -= 1;			//because we skipp the first line for the Data
		csvData.setRowLabels(parseLineToSegments(cachedLine));
	}

	for (size_t i = 0; i < csvData.rows.size(); i++)
	{
		csvData.rows[i].resize(newRowSize);
	}

	size_t currentLineCount = 0;
	while (std::getline(m_inputFile, cachedLine))
	{
		std::vector<std::string>cachedSegments;
		cachedSegments = parseLineToSegments(cachedLine);
		if (cachedSegments.size() == csvData.rows.size())
		{
			for (size_t i = 0; i < cachedSegments.size(); i++)
			{
				csvData.rows[i][currentLineCount] = cachedSegments[i];
			}
		}
		currentLineCount++;
	}
	m_inputFile.seekg(0, std::ios::beg);
	m_inputFile.close();
	return csvData;
}

CSVData CSVImporter::importDataV2(const std::string& fullFileName, const std::string& listOfDelimiter)
{
	CSVData csvData;

	//Open File:
	std::fstream inputFile(fullFileName, std::ios::in | std::ios::binary);
	if (!inputFile)
	{
		std::cerr << "ERROR: Could not open File: " << fullFileName << "\n";
		return csvData;
	}


	//Count the Lines:
	constexpr size_t BUF_SIZE = 1 << 20; // 1MB buffer
	std::unique_ptr<char[]> buffer = std::make_unique<char[]>(BUF_SIZE);
	size_t lineCount = 0;

	while (inputFile.read(buffer.get(), BUF_SIZE) || inputFile.gcount()) {
		size_t bytesRead = inputFile.gcount();
		for (size_t i = 0; i < bytesRead; ++i)
			lineCount += (buffer[i] == '\n');
	}


	//Check if CSV has Headline
	inputFile.clear();
	inputFile.seekg(0, std::ios::beg);

	char firstChar = '\0';
	inputFile.get(firstChar);

	if (inputFile.fail()) {
		std::cerr << "Error: The file is empty or unreadable: " << fullFileName << "\n";
		return csvData;
	}

	bool hasHeadLine = !(firstChar >= '0' && firstChar <= '9');	// Check if firstChar is digit (indicating no header)


	//Import file to vector<string>
	inputFile.clear();                  // Clear EOF/fail flags
	inputFile.seekg(0, std::ios::beg); // Rewind to beginning
	std::string cachedLine;
	size_t dataSize = lineCount;

	if (hasHeadLine)
	{
		std::getline(inputFile, cachedLine);
		dataSize -= 1;
		csvData.setRowLabels(parseLineToSegmentsV2(cachedLine));
	}

	csvData.rows.resize(csvData.rowLabel.size());
	for (size_t i = 0; i < csvData.rowLabel.size(); i++)
	{
		csvData.rows[i].resize(dataSize);
	}

	size_t currentLineCount = hasHeadLine ? 0 : 1; // already inserted one preview row if no header

	while (std::getline(inputFile, cachedLine))
	{
		std::vector<std::string> cachedSegments = parseLineToSegmentsV2(cachedLine);
		if (cachedSegments.size() == csvData.rows.size())
		{
			for (size_t i = 0; i < cachedSegments.size(); ++i)
			{
				if (currentLineCount < dataSize)
					csvData.rows[i][currentLineCount] = cachedSegments[i];
			}
			currentLineCount++;
		}
	}

	inputFile.close();
	return csvData;
}

CSVData CSVImporter::importDataV3(const std::string& fullFileName, const std::string& listOfDelimiter)
{


	CSVData csvData;

	//Open File:
	std::fstream inputFile(fullFileName, std::ios::in | std::ios::binary);
	if (!inputFile)
	{
		std::cerr << "ERROR: Could not open File: " << fullFileName << "\n";
		return csvData;
	}

	inputFile.seekg(0, std::ios::end);
	size_t fileSize = inputFile.tellg();
	inputFile.seekg(0, std::ios::beg);

	std::vector<char> fileBuffer(fileSize);
	inputFile.read(fileBuffer.data(), fileSize);
	inputFile.close();

	std::vector<std::pair<const char*, size_t>> lineViews; // pointer + length
	const char* start = fileBuffer.data();
	const char* end = start + fileSize;
	const char* lineStart = start;

	for (const char* p = start; p < end; ++p) {
		if (*p == '\n') {
			lineViews.emplace_back(lineStart, p - lineStart);
			lineStart = p + 1;
		}
	}
	if (lineStart < end)
		lineViews.emplace_back(lineStart, end - lineStart); // handle final line without newline

	const char* firstLine = lineViews[0].first;
	bool hasHeadLine = !(firstLine[0] >= '0' && firstLine[0] <= '9');



	size_t numColumns = csvData.rowLabel.size();
	csvData.rows.resize(numColumns);



	for (auto& col : csvData.rows)
		col.resize(lineViews.size() - (hasHeadLine ? 1 : 0));

	size_t startLine = hasHeadLine ? 1 : 0;
	size_t totalLines = lineViews.size() - startLine;

	std::vector<std::vector<std::string>> parsedLines(totalLines);

#pragma omp parallel for schedule(dynamic, 128)
	for (size_t i = 0; i < totalLines; ++i) {
		const auto& [linePtr, length] = lineViews[startLine + i];
		std::string_view line(linePtr, length);
		parsedLines[i] = parseLineToSegmentsV3(line); // must accept string_view
	}
	for (size_t row = 0; row < totalLines; ++row) {
		const auto& segments = parsedLines[row];
		if (segments.size() != numColumns)
			continue;
		for (size_t col = 0; col < numColumns; ++col) {
			csvData.rows[col][row] = segments[col];
		}
	}


	return CSVData();
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
std::vector<std::string> CSVImporter::parseLineToSegmentsV2(const std::string& line)
{
	const char* data = line.data();
	size_t len = line.size();
	std::vector<std::string> segments;
	segments.reserve(4); // adjust based on expected average column count

	// Fast delimiter lookup table (O(1) char match)
	bool isDelim[256] = {};
	for (char d : m_ListOfDelimiter)
		isDelim[static_cast<unsigned char>(d)] = true;

	size_t start = 0;
	for (size_t i = 0; i < len; ++i)
	{
		if (isDelim[static_cast<unsigned char>(data[i])])
		{
			segments.emplace_back(data + start, i - start);
			start = i + 1;
		}
	}

	// Add last segment
	if (start <= len)
		segments.emplace_back(data + start, len - start);

	return segments;
}

std::vector<std::string> CSVImporter::parseLineToSegmentsV3(const std::string_view& line)
{

	const char* data = line.data();
	size_t len = line.size();
	std::vector<std::string> segments;
	segments.reserve(4); // tune to expected column count

	// O(1) delimiter lookup
	bool isDelim[256] = {};
	for (char d : m_ListOfDelimiter)
		isDelim[static_cast<unsigned char>(d)] = true;

	size_t start = 0;
	for (size_t i = 0; i < len; ++i)
	{
		unsigned char c = static_cast<unsigned char>(data[i]);
		if (isDelim[c])
		{
			size_t segLen = i - start;
			segments.emplace_back(data + start, segLen);
			start = i + 1;
		}
	}

	// Handle last segment
	if (start <= len)
	{
		segments.emplace_back(data + start, len - start);
	}
	return segments;

}


std::vector<std::string> CSVImporter::parseLineToSegments(const std::string line)	//TODO: Faster by setting resizing first.
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