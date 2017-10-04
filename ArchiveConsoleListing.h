#pragma once

#include "OP2Utility.h"
#include <vector>
#include <string>
#include <memory>

class ArchiveConsoleListing
{
public:
	void listContents(Archives::ArchiveFile& archiveFile);

private:
	const size_t maxFilenameSize = 40;
	
	int findMaxFilenameSize(Archives::ArchiveFile& archiveFile);
	std::unique_ptr<std::vector<std::string>> formatFileSizes(Archives::ArchiveFile& archiveFile, size_t& maxCharsInSize);
	std::string CreateBlankChars(size_t stringSize, int columnSize);
};