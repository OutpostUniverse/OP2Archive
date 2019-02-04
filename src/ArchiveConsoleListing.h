#pragma once

#include "OP2Utility.h"
#include <vector>
#include <string>
#include <memory>
#include <cstddef>

class ArchiveConsoleListing
{
public:
	void ListContents(Archive::ArchiveFile& archiveFile);

private:
	const std::size_t maxFilenameSize = 40;
	
	std::size_t FindMaxFilenameSize(Archive::ArchiveFile& archiveFile);
	std::unique_ptr<std::vector<std::string>> FormatFileSizes(Archive::ArchiveFile& archiveFile, std::size_t& maxCharsInSize);
	std::string CreateBlankChars(std::size_t stringSize, std::size_t columnSize);
};
