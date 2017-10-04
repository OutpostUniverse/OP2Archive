#pragma once

#include "ConsoleSettings.h"
#include "OP2Utility.h"
#include <string>
#include <vector>

class ConsoleExtract
{
public:
	void extractCommand(const ConsoleArgs& consoleArgs);

private:
	void extractFromArchive(const std::string& archiveFilename, const std::vector<std::string>& filesToExtract, const ConsoleSettings& consoleSettings);
	void extractFromDirectory(const std::string& directory, const ConsoleSettings& consoleSettings);

	void extractAllFiles(Archives::ArchiveFile& archive, const ConsoleSettings& consoleSettings);
	void extractSpecificFile(Archives::ArchiveFile& archive, const std::string& filename, const ConsoleSettings& consoleSettings);
};