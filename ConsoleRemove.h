#pragma once

#include "ConsoleModifyBase.h"
#include "ConsoleSettings.h"
#include "OP2Utility.h"
#include <vector>
#include <string>

class ConsoleRemove : ConsoleModifyBase
{
public:
	ConsoleRemove() : ConsoleModifyBase("File(s) successfully removed from archive") {};
	void removeCommand(const ConsoleArgs& consoleArgs);

private:
	void outputInitialAddMessage(const std::string& archiveFilename, int fileCountToRemove);
	std::vector<std::string> removeMatchingFilenames(Archives::ArchiveFile& archive, const std::vector<std::string>& filesToRemove);
	void throwUnfoundFileDuringRemoveException(std::vector<std::string> unfoundFilenames);
	void checkFilesAvailableToRemove(Archives::ArchiveFile& archive, const std::vector<std::string>& filesToRemove, bool quiet);
	void extractFiles(Archives::ArchiveFile& archive, const std::vector<std::string> internalFilenames);
};