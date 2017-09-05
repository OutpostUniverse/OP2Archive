#include "ConsoleCreate.h"
#include "ConsoleRemove.h"
#include "ConsoleExtract.h"
#include "ConsoleHelper.h"
#include <iostream>

void ConsoleRemove::removeCommand(const ConsoleArgs& consoleArgs)
{
	string archiveFilename = getArchiveName(consoleArgs);
	ArchiveFile* archive = ConsoleHelper::openArchive(archiveFilename);
	vector<string>* filesToRemove = getFilesToModify(consoleArgs);
	
	checkFilesAvailableToRemove(archive, *filesToRemove, consoleArgs.consoleSettings.quiet);

	const vector<string>* archiveInternalFilenames = removeMatchingFilenames(archive, *filesToRemove);

	const string directory = ConsoleHelper::createTempDirectory();

	for (size_t i = 0; i < archiveInternalFilenames->size(); ++i)
	{
		string filename(archiveInternalFilenames->at(i));
		int index = archive->GetInternalFileIndex(filename.c_str());
		string pathToExtractTo = XFile::appendSubDirectory(filename, directory);
		if (!archive->ExtractFile(index, pathToExtractTo.c_str()))
		{
			XFile::deletePath(directory);
			throw exception(("Unable to extract file " + filename + " from original archive. Operation Aborted.").c_str());
		}
	}

	delete archive;

	XFile::deletePath(archiveFilename);

	vector<string> filenames = XFile::getFilesFromDirectory(directory);

	ConsoleCreate consoleCreate;
	consoleCreate.createArchiveFile(archiveFilename, filenames, consoleArgs.consoleSettings);

	delete archiveInternalFilenames;
	XFile::deletePath(directory);

	delete filesToRemove;
}

vector<string>* ConsoleRemove::removeMatchingStrings(const vector<string>& strings, const vector<string>& stringsToRemove)
{
	vector<string>* stringsToReturn = new vector<string>(strings.begin(), strings.end());

	auto pred = [&stringsToRemove](const std::string& key) ->bool
	{
		return std::find(stringsToRemove.begin(), stringsToRemove.end(), key) != stringsToRemove.end();
	};

	stringsToReturn->erase(std::remove_if(stringsToReturn->begin(), stringsToReturn->end(), pred), stringsToReturn->end());

	return stringsToReturn;
}

vector<string>* ConsoleRemove::removeMatchingFilenames(ArchiveFile* archive, const vector<string>& filesToRemove)
{
	vector<string> internalFilenames;

	for (int i = 0; i < archive->GetNumberOfPackedFiles(); ++i)
		internalFilenames.push_back(archive->GetInternalFileName(i));

	return removeMatchingStrings(internalFilenames, filesToRemove);
}

void ConsoleRemove::throwUnfoundFileDuringRemoveException(vector<string>* unfoundFilenames)
{
	string exceptionString("The Following filename(s) were not found in the archive:");

	for (size_t i = 0; i < unfoundFilenames->size(); ++i)
	{
		exceptionString += " " + unfoundFilenames->at(i);

		if (i < unfoundFilenames->size() - 1)
			exceptionString += ",";
	}

	exceptionString += ".";

	delete unfoundFilenames;

	throw exception(exceptionString.c_str());
}

void ConsoleRemove::checkFilesAvailableToRemove(ArchiveFile* archive, const vector<string>& filesToRemove, bool quiet)
{
	vector<string> internalFilenames;

	for (int i = 0; i < archive->GetNumberOfPackedFiles(); ++i)
		internalFilenames.push_back(archive->GetInternalFileName(i));

	vector<string>* unfoundFilenames = removeMatchingStrings(filesToRemove, internalFilenames);

	if (unfoundFilenames->size() > 0)
		throwUnfoundFileDuringRemoveException(unfoundFilenames);

	if (!quiet)
		cout << "All " << filesToRemove.size() << " files requested for removal located in archive." << endl;

	delete unfoundFilenames;
}