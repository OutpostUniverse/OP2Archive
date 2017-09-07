#include "ConsoleAdd.h"
#include "ConsoleHelper.h"
#include "ConsoleExtract.h"
#include "OP2Utility.h"

using namespace Archives;

void ConsoleAdd::addCommand(const ConsoleArgs& consoleArgs)
{
	string archiveFilename = getArchiveName(consoleArgs);
	
	vector<string>* filesToAdd = getFilesToModify(consoleArgs);

	//TODO: Check Files to add exist on hard drive.

	string tempDirectory = ConsoleHelper::createTempDirectory();

	extractCurrentArchiveContents(archiveFilename, tempDirectory);

	
}

void ConsoleAdd::extractCurrentArchiveContents(const string& archiveFilename, const string& tempDirectory)
{
	ArchiveFile* archive = ConsoleHelper::openArchive(archiveFilename);
	bool success = archive->ExtractAllFiles(tempDirectory.c_str());

	delete archive;

	if (!success)
	{
		XFile::deletePath(tempDirectory);
		throw exception(("Error extracting files from archive " + archiveFilename + ". Add operation aborted.").c_str());
	}
}

//void ConsoleAdd::checkFilesExist(const vector<string>& filenames)
//{
//
//}