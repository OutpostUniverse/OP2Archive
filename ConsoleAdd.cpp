#include "ConsoleAdd.h"
#include "ArchiveConsoleListing.h"
#include "ConsoleHelper.h"
#include "ConsoleExtract.h"
#include "ConsoleCreate.h"
#include "OP2Utility.h"
#include <iostream>

using namespace Archives;

void ConsoleAdd::addCommand(const ConsoleArgs& consoleArgs)
{
	string archiveFilename = getArchiveName(consoleArgs);
	
	vector<string> filesToAdd = getFilesToModify(consoleArgs);
	
	if (!consoleArgs.consoleSettings.quiet)
	{
		cout << "Attempting to add " << filesToAdd.size() << " file(s) to the archive " << archiveFilename << endl;
		cout << ConsoleHelper::dashedLine << endl;
	}

	checkFilesExist(filesToAdd);

	string tempDirectory = ConsoleHelper::createTempDirectory();

	extractCurrentArchiveContents(archiveFilename, tempDirectory);

	vector<string> extractedFiles = XFile::getFilesFromDirectory(tempDirectory);

	filesToAdd.insert(filesToAdd.end(), extractedFiles.begin(), extractedFiles.end());

	try
	{
		ConsoleCreate consoleCreate;
		consoleCreate.createArchiveFile(archiveFilename, filesToAdd, true);

		if (!consoleArgs.consoleSettings.quiet)
		{
			cout << "File(s) successfully added to archive " + archiveFilename << endl << endl;
			ArchiveConsoleListing listing;
			listing.listContents(ConsoleHelper::openArchive(archiveFilename));
		}
	}
	catch (exception e)
	{
		cleanup(tempDirectory);
		throw exception(e.what());
	}

	cleanup(tempDirectory);
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

void ConsoleAdd::checkFilesExist(const vector<string>& filenames)
{
	for (string filename : filenames)
	{
		if (!XFile::isFile(filename))
			throw exception((filename + " was not found. Operation aborted.").c_str());
	}
}

void ConsoleAdd::cleanup(const string& tempDirectory)
{
	XFile::deletePath(tempDirectory);
}