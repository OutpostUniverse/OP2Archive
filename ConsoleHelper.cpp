#include "ConsoleHelper.h"

bool isArchiveExtension(const string& filename)
{
	return XFile::extensionMatches(filename, "VOL") || XFile::extensionMatches(filename, "CLM");
}

void checkIfPathsEmpty(const ConsoleArgs& consoleArgs)
{
	if (consoleArgs.paths.size() == 0)
		throw exception("You must provide at least one file or directory. To provide the current directory, enter './'.");
}

ArchiveFile* openArchive(const string& archivePath)
{
	Archives::ArchiveFile* archiveFile;

	if (XFile::extensionMatches(archivePath, "VOL"))
		archiveFile = new Archives::VolFile(archivePath.c_str());
	else
		archiveFile = new Archives::ClmFile(archivePath.c_str());

	return archiveFile;
}

string createTempDirectory()
{
	srand((int)time(NULL)); // For creating a unique directory.
	int number = rand();

	string directory("./OP2ArchiveTemp-" + to_string(number));

	XFile::createDirectory(directory);

	return directory;
}