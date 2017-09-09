#include "ConsoleHelper.h"

string ConsoleHelper::dashedLine = "--------------------------------------------------";

bool ConsoleHelper::isArchiveExtension(const string& filename)
{
	return XFile::extensionMatches(filename, "VOL") || XFile::extensionMatches(filename, "CLM");
}

void ConsoleHelper::checkIfPathsEmpty(const ConsoleArgs& consoleArgs)
{
	if (consoleArgs.paths.size() == 0)
		throw exception("You must provide at least one file or directory. To provide the current directory, enter './'.");
}

ArchiveFile* ConsoleHelper::openArchive(const string& archivePath)
{
	if (XFile::extensionMatches(archivePath, "VOL"))
		return new Archives::VolFile(archivePath.c_str());
	else
		return new Archives::ClmFile(archivePath.c_str());
}

string ConsoleHelper::createTempDirectory()
{
	srand((int)time(NULL)); // For creating a unique directory.
	int number = rand();

	string directory("./OP2ArchiveTemp-" + to_string(number));

	XFile::createDirectory(directory);

	return directory;
}