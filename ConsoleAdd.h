#pragma once

#include "ConsoleModifyBase.h"
#include "ConsoleSettings.h"

class ConsoleAdd : ConsoleModifyBase
{
public:
	void addCommand(const ConsoleArgs& consoleArgs);

private:
	void extractCurrentArchiveContents(const string& archiveFilename, const string& tempDirectory);
	void checkFilesExist(const vector<string>& filenames);
	void cleanup(const string& tempDirectory, vector<string>* filesToAdd);
};