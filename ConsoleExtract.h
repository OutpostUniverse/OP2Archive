#pragma once

#include "ConsoleSettings.h"
#include "OP2Utility.h"
#include <string>
#include <vector>

class ConsoleExtract
{
public:
	void ExtractCommand(const ConsoleArgs& consoleArgs);

private:
	void ExtractFromArchive(const std::string& archiveFilename, const std::vector<std::string>& filesToExtract, const ConsoleSettings& consoleSettings);
	void ExtractFromDirectory(const std::string& directory, const ConsoleSettings& consoleSettings);

	void ExtractAllFiles(Archives::ArchiveFile& archive, const ConsoleSettings& consoleSettings);
	void ExtractSpecificFile(Archives::ArchiveFile& archive, const std::string& filename, const ConsoleSettings& consoleSettings);

	bool CheckIfFileExists(std::string path, bool quiet);
	void OutputExtractionMessage(bool success, std::string filename);
};