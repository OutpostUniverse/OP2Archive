#pragma once

#include <vector>
#include <string>
#include "ConsoleArgumentParser.h"
#include "OP2Utility.h"

using namespace std;
using namespace ConsoleArgumentParser;
using namespace Archives;

vector<string>* removeMatchingStrings(const vector<string>& strings, const vector<string>& stringsToRemove);
vector<string>* removeFilenames(ArchiveFile* archive, const vector<string>& filesToRemove);
void throwUnfoundFileDuringRemoveException(vector<string>* unfoundFilenames);
void checkFilesAvailableToRemove(ArchiveFile* archive, const vector<string>& filesToRemove, bool quiet);
vector<string>* getFilesToRemove(const ConsoleArgs& consoleArgs);
ArchiveFile* checkAndOpenArchive(const ConsoleArgs& consoleArgs);
void removeCommand(const ConsoleArgs& consoleArgs);

string createTempDirectory();