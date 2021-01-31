#pragma once

#include "OP2Utility.h"
#include <string>
#include <vector>
#include <memory>

class ConsoleHelper
{
public: 
	static const std::string dashedLine;

	static bool IsArchiveExtension(const std::string& filename);
	static std::unique_ptr<OP2Utility::Archive::ArchiveFile> OpenArchive(const std::string& archivePath);
	static std::vector<std::string> GetArchiveFilenames(const std::string& directory);
	static void CheckIfPathsEmpty(const std::vector<std::string>& paths);
	static std::string CreateTempDirectory();
	static void ListContentsOfArchive(const std::string& archiveFilename);
};
