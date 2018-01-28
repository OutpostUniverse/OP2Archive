#pragma once

#include "ConsoleSettings.h"
#include <string>

class ConsoleFind
{
public:
	void FindCommand(const ConsoleArgs& consoleArgs);

private:
	void FindFileInArchives(const std::string& path);
};

