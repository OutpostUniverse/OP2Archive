#pragma once

#include "ConsoleModifyBase.h"
#include "ConsoleSettings.h"

class ConsoleAdd : ConsoleModifyBase
{
public:
	void addCommand(const ConsoleArgs& consoleArgs);
};