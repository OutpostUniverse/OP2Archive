#pragma once

#include "ConsoleSettings.h"
#include "OP2Utility.h"
#include <string>
#include <memory>

using namespace std;
using namespace Archives;

class ConsoleHelper
{
public: 
	static string dashedLine;

	static bool isArchiveExtension(const string& filename);
	static void checkIfPathsEmpty(const ConsoleArgs& consoleArgs);
	static unique_ptr<ArchiveFile> openArchive(const string& archivePath);
	static string createTempDirectory();
};