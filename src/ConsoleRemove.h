#pragma once

#include "ConsoleModifyBase.h"
#include "ConsoleSettings.h"
#include "OP2Utility.h"
#include <vector>
#include <string>
#include <cstddef>

class ConsoleRemove : ConsoleModifyBase
{
public:
	ConsoleRemove() : ConsoleModifyBase("File(s) successfully removed from archive") {};
	void RemoveCommand(const ConsoleArgs& consoleArgs);

private:
	void OutputInitialAddMessage(const std::string& archiveFilename, std::size_t fileCountToRemove);
	std::vector<std::string> RemoveMatchingFilenames(OP2Utility::Archive::ArchiveFile& archive, const std::vector<std::string>& filesToRemove);
	void ThrowUnfoundFileDuringRemoveException(const std::vector<std::string>& unfoundFilenames);
	void CheckFilesAvailableToRemove(OP2Utility::Archive::ArchiveFile& archive, const std::vector<std::string>& filesToRemove, bool quiet);
	void ExtractFilesFromOriginalArchive(OP2Utility::Archive::ArchiveFile& archive, const std::vector<std::string> internalFilenames);
};
