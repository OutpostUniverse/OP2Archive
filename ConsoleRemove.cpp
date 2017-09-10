#include "ConsoleRemove.h"
#include "ConsoleCreate.h"
#include "ConsoleExtract.h"
#include "ConsoleHelper.h"
#include <iostream>

void ConsoleRemove::removeCommand(const ConsoleArgs& consoleArgs)
{
	string archiveFilename = getArchiveName(consoleArgs);
	ArchiveFile* archive = ConsoleHelper::openArchive(archiveFilename);
	vector<string> filesToRemove = getFilesToModify(consoleArgs);
	
	checkFilesAvailableToRemove(archive, filesToRemove, consoleArgs.consoleSettings.quiet);

	const vector<string> archiveInternalFilenames = removeMatchingFilenames(archive, filesToRemove);

	const string tempDirectory = ConsoleHelper::createTempDirectory();

	for (size_t i = 0; i < archiveInternalFilenames.size(); ++i)
	{
		string filename(archiveInternalFilenames[i]);
		int index = archive->GetInternalFileIndex(filename.c_str());
		string pathToExtractTo = XFile::appendSubDirectory(filename, tempDirectory);

		if (!archive->ExtractFile(index, pathToExtractTo.c_str()))
			throw exception(("Unable to extract file " + filename + " from original archive. Operation Aborted.").c_str());
	}

	delete archive;

	vector<string> filenames = XFile::getFilesFromDirectory(tempDirectory);

	createModifiedArchive(archiveFilename, filenames, consoleArgs.consoleSettings.quiet);
}

vector<string> ConsoleRemove::removeMatchingFilenames(ArchiveFile* archive, const vector<string>& filesToRemove)
{
	vector<string> internalFilenames;

	for (int i = 0; i < archive->GetNumberOfPackedFiles(); ++i)
		internalFilenames.push_back(archive->GetInternalFileName(i));

	return StringHelper::removeMatchingStrings(internalFilenames, filesToRemove);
}

void ConsoleRemove::throwUnfoundFileDuringRemoveException(vector<string> unfoundFilenames)
{
	string exceptionString("The Following filename(s) were not found in the archive:");

	for (size_t i = 0; i < unfoundFilenames.size(); ++i)
	{
		exceptionString += " " + unfoundFilenames[i];

		if (i < unfoundFilenames.size() - 1)
			exceptionString += ",";
	}

	exceptionString += ".";

	throw exception(exceptionString.c_str());
}

void ConsoleRemove::checkFilesAvailableToRemove(ArchiveFile* archive, const vector<string>& filesToRemove, bool quiet)
{
	vector<string> internalFilenames;

	for (int i = 0; i < archive->GetNumberOfPackedFiles(); ++i)
		internalFilenames.push_back(archive->GetInternalFileName(i));

	vector<string> unfoundFilenames = StringHelper::removeMatchingStrings(filesToRemove, internalFilenames);

	if (unfoundFilenames.size() > 0)
		throwUnfoundFileDuringRemoveException(unfoundFilenames);

	if (!quiet)
		cout << "All " << filesToRemove.size() << " files requested for removal located in archive." << endl;
}