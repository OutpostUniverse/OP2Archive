#pragma once

#include "ConsoleSettings.h"
#include "OP2Utility.h"
#include <string>
#include <vector>
#include <memory>

class ConsoleCreate
{
public:
	void CreateCommand(const ConsoleArgs& consoleArgs);
	void CreateArchiveFile(const std::string& archiveFilename, const std::vector<std::string>& filenames, bool quiet);

private:
	std::unique_ptr<Archives::ArchiveFile> CreateArchiveTemplate(const std::string& archiveFilename);
	void CreateUsingDefaultDirectory(const std::string& archiveFilename, const ConsoleSettings& consoleSettings);
	std::vector<std::string> GatherFilesForArchive(const std::vector<std::string>& paths);
	void CheckCreateOverwrite(const std::string& archiveFilename, bool overwrite, bool quiet);
	void OutputInitialCreateMessage(const std::string& archiveFilename, int packedFileCount);
	void OutputCreateResults(int packedFileCount, const std::string& archiveFilename);

	//std::vector<std::string> SortPathsByFileName(std::vector<std::string> paths);
	
	// Must be static for passing into the sort alogrithm.
	static bool ComparePathFilenames(const std::string path1, const std::string path2);
	void CheckForIllegalFilenames(std::vector<std::string> paths);
};
