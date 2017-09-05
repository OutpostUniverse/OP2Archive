#include "ConsoleModifyBase.h"

vector<string>* ConsoleModifyBase::getFilesToModify(const ConsoleArgs& consoleArgs)
{
	vector<string>* filesToRemove = new vector<string>(consoleArgs.paths.begin() + 1, consoleArgs.paths.end());

	if (filesToRemove->size() == 0)
		throw exception("No file(s) provided to remove from the archive.");

	return filesToRemove;
}

string ConsoleModifyBase::getArchiveName(const ConsoleArgs& consoleArgs)
{
	if (consoleArgs.paths.size() == 0)
		throw exception("No archive filename provided.");

	return consoleArgs.paths[0];
}