#pragma once

#include "OP2Utility.h"
#include <vector>
#include <string>
#include <memory>

using namespace std;
using namespace Archives;

class ArchiveConsoleListing
{
public:
	void listContents(ArchiveFile& archiveFile);

private:
	const size_t maxFilenameSize = 40;
	
	int findMaxFilenameSize(ArchiveFile& archiveFile);
	unique_ptr<vector<string>> formatFileSizes(ArchiveFile& archiveFile, size_t& maxCharsInSize);
	string CreateBlankChars(size_t stringSize, int columnSize);
};