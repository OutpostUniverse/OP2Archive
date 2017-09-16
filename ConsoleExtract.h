#pragma once

#include "ConsoleSettings.h"
#include <string>
#include <vector>
#include <memory>

using namespace std;

class ConsoleExtract
{
public:
	void extractCommand(const ConsoleArgs& consoleArgs);

private:
	void consoleExtractFiles(const ConsoleArgs& consoleArgs);
	void extractSpecificArchive(const ConsoleArgs& consoleArgs);

	void extractAllFilesSpecificArchive(ArchiveFile& archive, const ConsoleSettings& consoleSettings);
	void extractFileSpecificArchive(ArchiveFile& archive, const string& filename, const ConsoleSettings& consoleSettings);

	void consoleExtractFile(const string& internalFilename, const ConsoleSettings& consoleSettings);

	void consoleExtractDirectory(const string& directory, const ConsoleSettings& consoleSettings);

	vector<unique_ptr<ArchiveFile>> openArchivesInDirectory(const string& directory);
};