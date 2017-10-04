#pragma once

#include "OP2Utility.h"
#include <string>
#include <vector>
#include <memory>

class ConsoleHelper
{
public: 
	static string dashedLine;

	static bool isArchiveExtension(const std::string& filename);
	static std::unique_ptr<Archives::ArchiveFile> openArchive(const std::string& archivePath);
	static std::vector<std::string> getArchiveFilenames(const std::string& directory);
	static void ConsoleHelper::checkIfPathsEmpty(const std::vector<std::string>& paths);
	static std::string createTempDirectory();
	static void listContentsOfArchive(const std::string& archiveFilename);
};