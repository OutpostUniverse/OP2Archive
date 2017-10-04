#pragma once

#include "ConsoleSettings.h"
#include "OP2Utility.h"
#include <string>
#include <vector>
#include <memory>

class ConsoleCreate
{
public:
	void createCommand(const ConsoleArgs& consoleArgs);
	void createArchiveFile(const std::string& archiveFilename, const std::vector<std::string>& filenames, bool quiet);

private:
	std::unique_ptr<Archives::ArchiveFile> createArchiveTemplate(const std::string& archiveFilename);
	void createUsingDefaultDirectory(const std::string& archiveFilename, const ConsoleSettings& consoleSettings);
	std::vector<std::string> gatherFilesForArchive(const std::vector<std::string>& paths);
	void checkCreateOverwrite(const std::string& archiveFilename, bool overwrite, bool quiet);
	void outputInitialCreateMessage(const std::string& archiveFilename, int packedFileCount);
	void outputCreateResults(int packedFileCount, const std::string& archiveFilename);
};