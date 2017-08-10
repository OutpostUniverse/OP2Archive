#pragma once

#include "OP2Utility.h"

using namespace std;
using namespace Archives;

class ArchiveConsoleListing
{
public:
	void listContents(ArchiveFile* archiveFile);

private:
	int findMaxFilenameSize(ArchiveFile* archiveFile);
};