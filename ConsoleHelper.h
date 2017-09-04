#pragma once

#include "ConsoleSettings.h"
#include <string>

using namespace std;

const string dashedLine = "--------------------------------------------------";

bool isArchiveExtension(const string& filename);
void checkIfPathsEmpty(const ConsoleArgs& consoleArgs);