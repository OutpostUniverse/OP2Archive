#pragma once

#include "ConsoleModifyBase.h"
#include "ConsoleSettings.h"
#include <string>
#include <vector>

class ConsoleAdd : ConsoleModifyBase
{
public:
	ConsoleAdd() : ConsoleModifyBase("File(s) successfully added to archive") {};
	void addCommand(const ConsoleArgs& consoleArgs);

private:
	void outputInitialAddMessage(const string& archiveFilename, int fileCountToAdd);
	bool archivedFileTaggedForOverwrite(const string& internalFilename, const std::vector<std::string>& filesToAdd);
	std::vector<std::string> extractFiles(const string& archiveFilename, const std::vector<std::string>& internalFilenames, bool overwrite);
	void checkFilesExist(const std::vector<std::string>& filenames);
};