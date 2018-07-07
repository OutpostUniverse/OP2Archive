#pragma once

#include "ConsoleSettings.h"
#include <string>
#include <vector>

// Base class for ConsoleRemove and ConsoleAdd. Contains code common to a console 
// application providing ArchiveFile manipulation (not simple create or extract).
class ConsoleModifyBase
{
public:
	ConsoleModifyBase(const std::string& successMessage);
	virtual ~ConsoleModifyBase();

protected:
	std::string successMessage;
	std::string tempDirectory;

	std::string GetArchiveName(const ConsoleArgs& consoleArgs);
	std::vector<std::string> GetFilesToModify(const ConsoleArgs& consoleArgs);
	void CreateModifiedArchive(const std::string& archiveFilename, const std::vector<std::string>& filenamesToAdd, bool quiet);
};
