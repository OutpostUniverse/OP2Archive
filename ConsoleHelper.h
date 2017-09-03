#pragma once

#include "ConsoleArgumentParser.h"
#include <string>

using namespace std;
using namespace ConsoleArgumentParser;

const string dashedLine = "--------------------------------------------------";

bool isArchiveExtension(const string& filename);
void checkIfPathsEmpty(const ConsoleArgs& consoleArgs);