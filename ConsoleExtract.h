#pragma once

#include "ConsoleSettings.h"
#include <string>
#include <vector>

using namespace std;

void consoleExtractFiles(const ConsoleArgs& consoleArgs);
void extractSpecificArchive(const ConsoleArgs& consoleArgs);

void extractAllFilesSpecificArchive(ArchiveFile* archive, const ConsoleSettings& consoleSettings);
void extractFileSpecificArchive(ArchiveFile* archive, const string& filename, const ConsoleSettings& consoleSettings);


void consoleExtractFile(const string& internalFilename, const ConsoleSettings& consoleSettings);

void consoleExtractDirectory(const string& directory, const ConsoleSettings& consoleSettings);

vector<ArchiveFile*> openArchivesInDirectory(const string& directory);
ArchiveFile* openArchive(const string& archivePath);