#include "ConsoleRemove.h"
#include "ConsoleCreate.h"
#include "ConsoleExtract.h"
#include "ConsoleHelper.h"
#include <iostream>
#include <memory>
#include <stdexcept>

using namespace std;
using namespace Archives;

void ConsoleRemove::RemoveCommand(const ConsoleArgs& consoleArgs)
{
	string archiveFilename = GetArchiveName(consoleArgs);
	vector<string> filesToRemove = GetFilesToModify(consoleArgs);
	
	if (!consoleArgs.consoleSettings.quiet)
		OutputInitialAddMessage(archiveFilename, filesToRemove.size());

	unique_ptr<ArchiveFile> archive = ConsoleHelper::OpenArchive(archiveFilename);

	CheckFilesAvailableToRemove(*archive, filesToRemove, consoleArgs.consoleSettings.quiet);

	const vector<string> archiveInternalFilenames = RemoveMatchingFilenames(*archive, filesToRemove);

	ExtractFiles(*archive, archiveInternalFilenames);

	archive.reset();

	vector<string> filenames = XFile::getFilesFromDirectory(tempDirectory);

	CreateModifiedArchive(archiveFilename, filenames, consoleArgs.consoleSettings.quiet);
}

void ConsoleRemove::OutputInitialAddMessage(const string& archiveFilename, int fileCountToRemove)
{
	cout << "Attempting to remove " << fileCountToRemove << " file(s) from the archive " << archiveFilename << endl;
	cout << ConsoleHelper::dashedLine << endl;
}

vector<string> ConsoleRemove::RemoveMatchingFilenames(ArchiveFile& archive, const vector<string>& filesToRemove)
{
	vector<string> internalFilenames;

	for (int i = 0; i < archive.GetNumberOfPackedFiles(); ++i)
		internalFilenames.push_back(archive.GetInternalFileName(i));

	return StringHelper::removeMatchingStrings(internalFilenames, filesToRemove);
}

void ConsoleRemove::ThrowUnfoundFileDuringRemoveException(vector<string> unfoundFilenames)
{
	string exceptionString("The Following filename(s) were not found in the archive:");

	for (size_t i = 0; i < unfoundFilenames.size(); ++i)
	{
		exceptionString += " " + unfoundFilenames[i];

		if (i < unfoundFilenames.size() - 1)
			exceptionString += ",";
	}

	exceptionString += ".";

	throw runtime_error(exceptionString);
}

void ConsoleRemove::CheckFilesAvailableToRemove(ArchiveFile& archive, const vector<string>& filesToRemove, bool quiet)
{
	vector<string> internalFilenames;

	for (int i = 0; i < archive.GetNumberOfPackedFiles(); ++i)
		internalFilenames.push_back(archive.GetInternalFileName(i));

	vector<string> unfoundFilenames = StringHelper::removeMatchingStrings(filesToRemove, internalFilenames);

	if (unfoundFilenames.size() > 0)
		ThrowUnfoundFileDuringRemoveException(unfoundFilenames);
}

void ConsoleRemove::ExtractFiles(ArchiveFile& archive, const vector<string> internalFilenames)
{
	for (size_t i = 0; i < internalFilenames.size(); ++i)
	{
		string filename(internalFilenames[i]);
		int index = archive.GetInternalFileIndex(filename.c_str());
		string pathToExtractTo = XFile::appendSubDirectory(filename, tempDirectory);

		if (!archive.ExtractFile(index, pathToExtractTo.c_str()))
			throw runtime_error("Unable to extract file " + filename + " from original archive. Operation Aborted.");
	}
}
