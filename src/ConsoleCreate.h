#pragma once

#include "ConsoleSettings.h"
#include "OP2Utility.h"
#include <string>
#include <vector>
#include <memory>
#include <cstddef>

class ConsoleCreate
{
public:
	void CreateCommand(const ConsoleArgs& consoleArgs);
	void CreateArchiveFile(const std::string& archiveFilename, const std::vector<std::string>& filenames, bool quiet);

private:
	std::unique_ptr<Archive::ArchiveFile> CreateArchiveTemplate(const std::string& archiveFilename);
	std::vector<std::string> GatherFilesForArchive(const std::vector<std::string>& paths);
	void CheckCreateOverwrite(const std::string& archiveFilename, bool overwrite, bool quiet);
	void OutputInitialCreateMessage(const std::string& archiveFilename, std::size_t packedFileCount);
	void OutputCreateResults(std::size_t packedFileCount, const std::string& archiveFilename);

	void CheckForIllegalFilenames(const std::vector<std::string>& paths);
};
