#pragma once

#include "OP2Utility.h"
#include <string>
#include <vector>
#include <memory>

using namespace std;
using namespace Archives;

class ConsoleHelper
{
public: 
	static string dashedLine;

	static bool isArchiveExtension(const string& filename);
	static void ConsoleHelper::checkIfPathsEmpty(const vector<string>& paths);
	static unique_ptr<ArchiveFile> openArchive(const string& archivePath);
	static string createTempDirectory();
};