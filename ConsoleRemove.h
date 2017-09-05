#pragma once

#include "ConsoleSettings.h"
#include "OP2Utility.h"
#include <vector>
#include <string>

using namespace std;
using namespace Archives;

class ConsoleRemove
{
public:
	void removeCommand(const ConsoleArgs& consoleArgs);

private:
	vector<string>* removeMatchingStrings(const vector<string>& strings, const vector<string>& stringsToRemove);
	vector<string>* removeFilenames(ArchiveFile* archive, const vector<string>& filesToRemove);
	void throwUnfoundFileDuringRemoveException(vector<string>* unfoundFilenames);
	void checkFilesAvailableToRemove(ArchiveFile* archive, const vector<string>& filesToRemove, bool quiet);
	vector<string>* getFilesToRemove(const ConsoleArgs& consoleArgs);
	ArchiveFile* checkAndOpenArchive(const ConsoleArgs& consoleArgs);
};