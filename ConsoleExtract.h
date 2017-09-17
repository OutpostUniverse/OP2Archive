#pragma once

#include "ConsoleSettings.h"
#include "OP2Utility.h"
#include <string>
#include <vector>
#include <memory>

using namespace std;
using namespace Archives;

class ConsoleExtract
{
public:
	void extractCommand(const ConsoleArgs& consoleArgs);

private:
	void extractFromArchive(const string& archiveFilename, const vector<string>& filesToExtract, const ConsoleSettings& consoleSettings);
	void extractFromDirectory(const string& directory, const ConsoleSettings& consoleSettings);

	void extractAllFiles(ArchiveFile& archive, const ConsoleSettings& consoleSettings);
	void extractSpecificFile(ArchiveFile& archive, const string& filename, const ConsoleSettings& consoleSettings);
	vector<string> getArchiveFilenames(const string& directory);
};