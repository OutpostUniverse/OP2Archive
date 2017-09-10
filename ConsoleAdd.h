#pragma once

#include "ConsoleModifyBase.h"
#include "ConsoleSettings.h"

class ConsoleAdd : ConsoleModifyBase
{
public:
	ConsoleAdd() : ConsoleModifyBase("File(s) successfully added to archive") {};
	void addCommand(const ConsoleArgs& consoleArgs);

private:
	void outputInitialAddMessage(const string& archiveFilename, int fileCountToAdd);
	bool archivedFileTaggedForOverwrite(const string& internalFilename, const vector<string>& filesToAdd);
	vector<string> extractFiles(const string& archiveFilename, const vector<string>& internalFilenames, bool overwrite);
	void checkFilesExist(const vector<string>& filenames);
};