#include "ConsoleList.h"
#include "ConsoleHelper.h"
#include <iostream>
#include <vector>
#include <memory>
#include <stdexcept>

using namespace std;
using namespace Archives;

void ConsoleList::listCommand(const ConsoleArgs& consoleArgs)
{
	ConsoleHelper::checkIfPathsEmpty(consoleArgs.paths);

	for (string path : consoleArgs.paths)
	{
		if (XFile::isDirectory(path))
			listAllArchivesInDirectory(path);
		else if (ConsoleHelper::isArchiveExtension(path))
			listArchiveContents(path);
		else
			throw invalid_argument("You must provide either a directory or a file of type (.vol|.clm).");
	}
}

void ConsoleList::listArchiveContents(const string& filename)
{
	unique_ptr<ArchiveFile> archive = ConsoleHelper::openArchive(filename);
	archiveConsoleListing.listContents(*archive);
}

void ConsoleList::listAllArchivesInDirectory(const string& directory)
{
	vector<string> archiveFilenames = ConsoleHelper::getArchiveFilenames(directory);

	cout << archiveFilenames.size() << " archive file(s) located." << endl;
	cout << ConsoleHelper::dashedLine << endl << endl;

	for (const string& filename : archiveFilenames)
		listArchiveContents(filename);
}