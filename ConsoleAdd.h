#pragma once

#include "ConsoleModifyBase.h"
#include "ConsoleSettings.h"

class ConsoleAdd : ConsoleModifyBase
{
public:
	~ConsoleAdd();
	void addCommand(const ConsoleArgs& consoleArgs);

private:
	string tempDirectory;

	void outputInitialAddMessage(const string& archiveFilename, int fileCountToAdd);
	void createModifiedArchive(const string& archiveFilename, const vector<string>& filesToAdd, bool quiet);
	bool archivedFileTaggedForOverwrite(const string& internalFilename, const vector<string>& filesToAdd);
	vector<string> extractFiles(const string& archiveFilename, const string& tempDirectory, const vector<string>& filesToAdd, bool overwrite);
	void checkFilesExist(const vector<string>& filenames);
};