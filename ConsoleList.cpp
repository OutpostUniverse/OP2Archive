#include "ConsoleList.h"
#include "ConsoleHelper.h"
#include <iostream>
#include <vector>
#include <memory>
#include <stdexcept>

using namespace std;
using namespace Archives;

void ConsoleList::ListCommand(const ConsoleArgs& consoleArgs)
{
	ConsoleHelper::CheckIfPathsEmpty(consoleArgs.paths);

	for (string path : consoleArgs.paths)
	{
		if (XFile::isDirectory(path))
			ListAllArchivesInDirectory(path);
		else if (ConsoleHelper::IsArchiveExtension(path))
			ListArchiveContents(path);
		else
			throw invalid_argument("You must provide either a directory or a file of type (.vol|.clm).");
	}
}

void ConsoleList::ListArchiveContents(const string& filename)
{
	unique_ptr<ArchiveFile> archive = ConsoleHelper::OpenArchive(filename);
	archiveConsoleListing.ListContents(*archive);
}

void ConsoleList::ListAllArchivesInDirectory(const string& directory)
{
	vector<string> archiveFilenames = ConsoleHelper::GetArchiveFilenames(directory);

	cout << archiveFilenames.size() << " archive file(s) located." << endl;
	cout << ConsoleHelper::dashedLine << endl << endl;

	for (const string& filename : archiveFilenames)
		ListArchiveContents(filename);
}