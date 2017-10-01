#include "ConsoleLocate.h"
#include "ConsoleHelper.h"
#include <iostream>

void ConsoleLocate::locateCommand(const ConsoleArgs& consoleArgs)
{
	ConsoleHelper::checkIfPathsEmpty(consoleArgs.paths);

	for (string path : consoleArgs.paths)
		locateFileInArchives(path);
}

void ConsoleLocate::locateFileInArchives(const string& path)
{
	string directory = XFile::getDirectory(path);
	ResourceManager resourceManager(directory);

	string filename = XFile::getFilename(path);

	string archiveFilename = resourceManager.findContainingArchiveFile(filename);

	if (archiveFilename == "")
		cout << "The file " << filename << " is not located in an archive at the supplied directory." << endl << endl;
	else
		cout << "The file " << filename << " is located in the archive " << archiveFilename << "." << endl << endl;
}