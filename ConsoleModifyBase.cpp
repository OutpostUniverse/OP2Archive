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

	tempDirectory = ConsoleHelper::CreateTempDirectory();
}

ConsoleModifyBase::~ConsoleModifyBase()
{
	if (!tempDirectory.empty())
		XFile::deletePath(tempDirectory);
}

vector<string> ConsoleModifyBase::GetFilesToModify(const ConsoleArgs& consoleArgs)
{
	vector<string> filesToRemove(consoleArgs.paths.begin() + 1, consoleArgs.paths.end());

	if (filesToRemove.size() == 0)
		throw runtime_error("No filename(s) provided to use in modifying the archive.");

	return filesToRemove;
}

string ConsoleModifyBase::GetArchiveName(const ConsoleArgs& consoleArgs)
{
	if (consoleArgs.paths.size() == 0)
		throw runtime_error("No archive filename provided.");

	return consoleArgs.paths[0];
}

void ConsoleModifyBase::CreateModifiedArchive(const string& archiveFilename, const vector<string>& filenamesToAdd, bool quiet)
{
	ConsoleCreate consoleCreate;
	consoleCreate.CreateArchiveFile(archiveFilename, filenamesToAdd, true);

	if (!quiet)
	{
		cout << successMessage + " " + archiveFilename << endl << endl;
		ConsoleHelper::ListContentsOfArchive(archiveFilename);
	}
}
