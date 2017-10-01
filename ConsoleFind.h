#pragma once

#include "ConsoleSettings.h"
#include <string>

using namespace std;

class ConsoleFind
{
public:
	void findCommand(const ConsoleArgs& consoleArgs);

private:
	void findFileInArchives(const string& path);
};

