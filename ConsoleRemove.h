#pragma once

#include "ConsoleModifyBase.h"
#include "ConsoleSettings.h"
#include "OP2Utility.h"
#include <vector>
#include <string>

using namespace std;
using namespace Archives;

class ConsoleRemove : ConsoleModifyBase
{
public:
	ConsoleRemove() : ConsoleModifyBase("") {};
	void removeCommand(const ConsoleArgs& consoleArgs);

private:
	vector<string> removeMatchingFilenames(ArchiveFile* archive, const vector<string>& filesToRemove);
	void throwUnfoundFileDuringRemoveException(vector<string> unfoundFilenames);
	void checkFilesAvailableToRemove(ArchiveFile* archive, const vector<string>& filesToRemove, bool quiet);
};