#pragma once

#include "ArchiveConsoleListing.h"
#include "ConsoleSettings.h"
#include <string>

using namespace std;

class ConsoleList
{
public:
	void listCommand(const ConsoleArgs& consoleArgs);

private:
	ArchiveConsoleListing archiveConsoleListing;

	void listArchiveContents(const string& filename);
	void listAllArchivesInDirectory(const string& directory);
};