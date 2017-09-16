#pragma once

#include "ConsoleSettings.h"
#include <string>

class ConsoleLocate
{
public:
	void locateCommand(const ConsoleArgs& consoleArgs);

private:
	void locateFileInArchives(const string& path);
};

