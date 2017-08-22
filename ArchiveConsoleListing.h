#pragma once

#include <vector>
#include <string>
#include "OP2Utility.h"

using namespace std;
using namespace Archives;

class ArchiveConsoleListing
{
public:
	void listContents(ArchiveFile* archiveFile);

private:
	const size_t maxFilenameSize = 40;
	
	int findMaxFilenameSize(ArchiveFile* archiveFile);
	vector<string>* formatFileSizes(ArchiveFile* archiveFile, size_t& maxCharsInSize);
	string CreateBlankChars(size_t stringSize, int columnSize);
};