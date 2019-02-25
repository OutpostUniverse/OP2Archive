#include "ConsoleFind.h"
#include "ConsoleHelper.h"
#include <iostream>

using namespace std;

void ConsoleFind::FindCommand(const ConsoleArgs& consoleArgs)
{
	ConsoleHelper::CheckIfPathsEmpty(consoleArgs.paths);

	for (const auto& path : consoleArgs.paths) {
		FindFileInArchives(path);
	}
}

void ConsoleFind::FindFileInArchives(const string& path)
{
	ResourceManager resourceManager(XFile::GetDirectory(path));

	const string filename = XFile::GetFilename(path);

	const string archiveFilename = resourceManager.FindContainingArchivePath(filename);

	if (archiveFilename == "") {
		cout << "The file " << filename << " is not located in an archive at the supplied directory." << endl << endl;
	}
	else {
		cout << "The file " << filename << " is located in the archive " << archiveFilename << "." << endl << endl;
	}
}
