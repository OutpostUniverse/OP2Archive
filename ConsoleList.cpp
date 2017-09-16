#include "ConsoleList.h"
#include "ConsoleHelper.h"
#include <iostream>

void ConsoleList::listCommand(const ConsoleArgs& consoleArgs)
{
	ConsoleHelper::checkIfPathsEmpty(consoleArgs);

	for (string path : consoleArgs.paths)
	{
		if (XFile::isDirectory(path))
			listAllArchivesInDirectory(path);
		else if (ConsoleHelper::isArchiveExtension(path))
			listArchiveContent(path);
		else
			throw exception("You must provide either a directory or a file of type (.vol|.clm).");
	}
}

void ConsoleList::listArchiveContent(const string& filename)
{
	unique_ptr<ArchiveFile> archive = ConsoleHelper::openArchive(filename);
	archiveConsoleListing.listContents(*archive);
}

void ConsoleList::listAllArchivesInDirectory(const string& directory)
{
	ResourceManager resourceManager(directory);
	vector<string> volFilenames = resourceManager.getAllFilenamesOfType(directory, ".vol", false);
	vector<string> clmFilenames = resourceManager.getAllFilenames(directory, ".clm", false);

	cout << volFilenames.size() << " vol archive file(s) located." << endl;
	cout << clmFilenames.size() << " clm archive file(s) located." << endl;
	cout << ConsoleHelper::dashedLine << endl << endl;

	for (const string& filename : volFilenames)
		listArchiveContent(filename);

	for (const string& filename : clmFilenames)
		listArchiveContent(filename);
}