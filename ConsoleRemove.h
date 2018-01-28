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
	void RemoveCommand(const ConsoleArgs& consoleArgs);

private:
	void OutputInitialAddMessage(const std::string& archiveFilename, int fileCountToRemove);
	std::vector<std::string> RemoveMatchingFilenames(Archives::ArchiveFile& archive, const std::vector<std::string>& filesToRemove);
	void ThrowUnfoundFileDuringRemoveException(std::vector<std::string> unfoundFilenames);
	void CheckFilesAvailableToRemove(Archives::ArchiveFile& archive, const std::vector<std::string>& filesToRemove, bool quiet);
	void ExtractFiles(Archives::ArchiveFile& archive, const std::vector<std::string> internalFilenames);
};
