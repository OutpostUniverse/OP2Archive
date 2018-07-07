#pragma once

#include "OP2Utility.h"
#include <string>
#include <vector>
#include <memory>

class ConsoleHelper
{
public: 
	static std::string dashedLine;

	static bool IsArchiveExtension(const std::string& filename);
	static std::unique_ptr<Archives::ArchiveFile> OpenArchive(const std::string& archivePath);
	static std::vector<std::string> GetArchiveFilenames(const std::string& directory);
	static void CheckIfPathsEmpty(const std::vector<std::string>& paths);
	static std::string CreateTempDirectory();
	static void ListContentsOfArchive(const std::string& archiveFilename);
};
