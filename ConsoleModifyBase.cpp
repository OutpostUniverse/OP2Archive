#include "ConsoleModifyBase.h"
#include "ConsoleCreate.h"
#include "ConsoleHelper.h"
#include <iostream>
#include <memory>
#include <stdexcept>

using namespace std;

ConsoleModifyBase::ConsoleModifyBase(const string& successMessage)
{
	this->successMessage = successMessage;

	tempDirectory = ConsoleHelper::createTempDirectory();
}

ConsoleModifyBase::~ConsoleModifyBase()
{
	if (!tempDirectory.empty())
		XFile::deletePath(tempDirectory);
}

vector<string> ConsoleModifyBase::getFilesToModify(const ConsoleArgs& consoleArgs)
{
	vector<string> filesToRemove(consoleArgs.paths.begin() + 1, consoleArgs.paths.end());

	if (filesToRemove.size() == 0)
		throw runtime_error("No filename(s) provided to use in modifying the archive.");

	return filesToRemove;
}

string ConsoleModifyBase::getArchiveName(const ConsoleArgs& consoleArgs)
{
	if (consoleArgs.paths.size() == 0)
		throw runtime_error("No archive filename provided.");

	return consoleArgs.paths[0];
}

void ConsoleModifyBase::createModifiedArchive(const string& archiveFilename, const vector<string>& filenamesToAdd, bool quiet)
{
	ConsoleCreate consoleCreate;
	consoleCreate.createArchiveFile(archiveFilename, filenamesToAdd, true);

	if (!quiet)
	{
		cout << successMessage + " " + archiveFilename << endl << endl;
		ConsoleHelper::listContentsOfArchive(archiveFilename);
	}
}