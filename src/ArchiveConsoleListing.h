#pragma once

#include "OP2Utility.h"
#include <vector>
#include <string>
#include <memory>
#include <cstddef>

class ArchiveConsoleListing
{
public:
	void ListContents(Archives::ArchiveFile& archiveFile);

private:
	const std::size_t maxFilenameSize = 40;
	
	int FindMaxFilenameSize(Archives::ArchiveFile& archiveFile);
	std::unique_ptr<std::vector<std::string>> FormatFileSizes(Archives::ArchiveFile& archiveFile, std::size_t& maxCharsInSize);
	std::string CreateBlankChars(std::size_t stringSize, int columnSize);
};
