#pragma once

#include "ConsoleSettings.h"
#include "OP2Utility.h"
#include <string>

using namespace std;
using namespace Archives;

class ConsoleCreate
{
public:
	void createCommand(const ConsoleArgs& consoleArgs);
	void createArchiveFile(const string& archiveFilename, const vector<string>& filenames, const ConsoleSettings& consoleSettings);

private:
	ArchiveFile* createArchiveTemplate(const string& archiveFilename);
	void outputCreateBegan(const string& archiveFilename, int packedFileCount);
	void outputCreateResults(bool success, int packedFileCount);
	void createUsingDefaultDirectory(const string& archiveFilename, const ConsoleSettings& consoleSettings);
	vector<string> gatherFilesForArchive(const vector<string>& paths);
	void checkCreateOverwrite(const string& archiveFilename, bool overwrite, bool quiet);
};