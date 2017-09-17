#include "ConsoleHelper.h"

string ConsoleHelper::dashedLine = "--------------------------------------------------";

bool ConsoleHelper::isArchiveExtension(const string& filename)
{
	return XFile::extensionMatches(filename, "VOL") || XFile::extensionMatches(filename, "CLM");
}

void ConsoleHelper::checkIfPathsEmpty(const vector<string>& paths)
{
	if (paths.size() == 0)
		throw exception("You must provide at least one file or directory. To provide the current directory, enter './'.");
}

unique_ptr<ArchiveFile> ConsoleHelper::openArchive(const string& archivePath)
{
	if (XFile::extensionMatches(archivePath, "VOL"))
		return make_unique<VolFile>(archivePath.c_str());
	
	if (XFile::extensionMatches(archivePath, "CLM"))
		return make_unique<ClmFile>(archivePath.c_str());

	throw invalid_argument("Provided filename is not an archive file (.VOL/.CLM)");
}

string ConsoleHelper::createTempDirectory()
{
	srand(static_cast<int>(time(NULL))); // For creating a unique directory.
	int number = rand();

	string directory("./OP2ArchiveTemp-" + to_string(number));

	XFile::createDirectory(directory);

	return directory;
}