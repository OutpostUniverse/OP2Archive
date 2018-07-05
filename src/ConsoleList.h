#pragma once

#include "ArchiveConsoleListing.h"
#include "ConsoleSettings.h"
#include <string>

class ConsoleList
{
public:
	void ListCommand(const ConsoleArgs& consoleArgs);

private:
	ArchiveConsoleListing archiveConsoleListing;

	void ListArchiveContents(const std::string& filename);
	void ListAllArchivesInDirectory(const std::string& directory);
};
