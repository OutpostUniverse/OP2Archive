#include "ConsoleAdd.h"
#include "ConsoleHelper.h"
#include "ConsoleExtract.h"
#include "OP2Utility.h"
#include <iostream>

using namespace Archives;

void ConsoleAdd::addCommand(const ConsoleArgs& consoleArgs)
{
	string archiveFilename = getArchiveName(consoleArgs);
	
	vector<string> filesToAdd = getFilesToModify(consoleArgs);
	
	if (!consoleArgs.consoleSettings.quiet)
		outputInitialAddMessage(archiveFilename, filesToAdd.size());

	checkFilesExist(filesToAdd);

	vector<string> extractedFiles = extractFiles(archiveFilename, filesToAdd, consoleArgs.consoleSettings.overwrite);

	filesToAdd.insert(filesToAdd.end(), extractedFiles.begin(), extractedFiles.end());

	createModifiedArchive(archiveFilename, filesToAdd, consoleArgs.consoleSettings.quiet);
}

void ConsoleAdd::outputInitialAddMessage(const string& archiveFilename, int fileCountToAdd)
{
	cout << "Attempting to add " << fileCountToAdd << " file(s) to the archive " << archiveFilename << endl;
	cout << ConsoleHelper::dashedLine << endl;
}

bool ConsoleAdd::archivedFileTaggedForOverwrite(const string& internalFilename, const vector<string>& filesToAdd)
{
	for (string fileToAdd : filesToAdd)
	{
		if (XFile::pathsAreEqual(XFile::getFilename(fileToAdd), internalFilename))
			return true;
	}

	return false;
}

vector<string> ConsoleAdd::extractFiles(const string& archiveFilename, const vector<string>& internalFilenames, bool overwrite)
{
	ArchiveFile* archive = ConsoleHelper::openArchive(archiveFilename);

	for (int i = 0; i < archive->GetNumberOfPackedFiles(); ++i)
	{
		string internalFilename = archive->GetInternalFileName(i);

		bool taggedForOverwrite = archivedFileTaggedForOverwrite(internalFilename, internalFilenames);

		if (taggedForOverwrite && !overwrite)
			throw exception(("ADD aborted. " + internalFilename + " is already contained in " + archiveFilename + ". To overwrite, add argument -O.").c_str());

		if (!taggedForOverwrite)
		{
			bool extractionSuccessful = archive->ExtractFile(i, XFile::appendSubDirectory(internalFilename, tempDirectory).c_str());

			if (!extractionSuccessful)
			{
				delete archive;
				throw exception(("Error extracting " + internalFilename + " from archive " + archiveFilename + ". Add operation aborted.").c_str());
			}
		}
	}

	delete archive;		

	return XFile::getFilesFromDirectory(tempDirectory);
}

void ConsoleAdd::checkFilesExist(const vector<string>& filenames)
{
	for (string filename : filenames)
	{
		if (!XFile::isFile(filename))
			throw exception((filename + " was not found. Operation aborted.").c_str());
	}
}