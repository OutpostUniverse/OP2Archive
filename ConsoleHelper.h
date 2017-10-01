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
	static unique_ptr<ArchiveFile> openArchive(const string& archivePath);
	static vector<string> getArchiveFilenames(const string& directory);
	static void ConsoleHelper::checkIfPathsEmpty(const vector<string>& paths);
	static string createTempDirectory();
	static void listContentsOfArchive(const string& archiveFilename);
};