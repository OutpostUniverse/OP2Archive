#pragma once

#include "ConsoleSettings.h"
#include <string>
#include <vector>

using namespace std;

// Base class for ConsoleRemove and ConsoleAdd. Contains code common to a console 
// application providing ArchiveFile manipulation (not simple create or extract).
class ConsoleModifyBase
{
public:
	ConsoleModifyBase(const string& successMessage);
	virtual ~ConsoleModifyBase();

protected:
	string successMessage;
	string tempDirectory;

	string getArchiveName(const ConsoleArgs& consoleArgs);
	vector<string> getFilesToModify(const ConsoleArgs& consoleArgs);
	void createModifiedArchive(const string& archiveFilename, const vector<string>& filenamesToAdd, bool quiet);
};