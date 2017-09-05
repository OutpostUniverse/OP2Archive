#pragma once

#include "ConsoleSettings.h"
#include <string>
#include <vector>

using namespace std;

// Base class for ConsoleRemove and ConsoleAdd. Contains code common to a console 
// application providing ArchiveFile manipulation (not simple create or extract).
class ConsoleModifyBase
{
protected:
	string getArchiveName(const ConsoleArgs& consoleArgs);
	vector<string>* getFilesToModify(const ConsoleArgs& consoleArgs);
};