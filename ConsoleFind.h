#pragma once

#include "ConsoleSettings.h"
#include <string>

class ConsoleFind
{
public:
	void findCommand(const ConsoleArgs& consoleArgs);

private:
	void findFileInArchives(const std::string& path);
};

