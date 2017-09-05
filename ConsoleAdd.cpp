#include "ConsoleAdd.h"
#include "ConsoleHelper.h"
#include "OP2Utility.h"

using namespace Archives;

void ConsoleAdd::addCommand(const ConsoleArgs& consoleArgs)
{
	string archiveFilename = getArchiveName(consoleArgs);
	ArchiveFile* archive = ConsoleHelper::openArchive(archiveFilename);
	vector<string>* filesToRemove = getFilesToModify(consoleArgs);


}