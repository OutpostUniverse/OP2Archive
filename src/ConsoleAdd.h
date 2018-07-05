#pragma once

#include "ConsoleModifyBase.h"
#include "ConsoleSettings.h"
#include <string>
#include <vector>

class ConsoleAdd : ConsoleModifyBase
{
public:
	ConsoleAdd() : ConsoleModifyBase("File(s) successfully added to archive") {};
	void AddCommand(const ConsoleArgs& consoleArgs);

private:
	void OutputInitialAddMessage(const std::string& archiveFilename, int fileCountToAdd);
	bool ArchivedFileTaggedForOverwrite(const std::string& internalFilename, const std::vector<std::string>& filesToAdd);
	std::vector<std::string> ExtractFilesFromOriginalArchive(const std::string& archiveFilename, const std::vector<std::string>& internalFilenames, bool overwrite);
	void CheckFilesExist(const std::vector<std::string>& filenames);
};
