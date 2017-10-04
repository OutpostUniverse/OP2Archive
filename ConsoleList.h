#pragma once

#include "ArchiveConsoleListing.h"
#include "ConsoleSettings.h"
#include <string>

class ConsoleList
{
public:
	void listCommand(const ConsoleArgs& consoleArgs);

private:
	ArchiveConsoleListing archiveConsoleListing;

	void listArchiveContents(const std::string& filename);
	void listAllArchivesInDirectory(const std::string& directory);
};