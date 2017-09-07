#pragma once

#include "ConsoleModifyBase.h"
#include "ConsoleSettings.h"

class ConsoleAdd : ConsoleModifyBase
{
public:
	void addCommand(const ConsoleArgs& consoleArgs);

private:
	void extractCurrentArchiveContents(const string& archiveFilename, const string& tempDirectory);
};