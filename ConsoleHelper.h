#pragma once

#include "ConsoleSettings.h"
#include <string>

using namespace std;

class ConsoleHelper
{
public: 
	static string dashedLine;

	static bool isArchiveExtension(const string& filename);
	static void checkIfPathsEmpty(const ConsoleArgs& consoleArgs);
	static ArchiveFile* openArchive(const string& archivePath);
	static string createTempDirectory();
};