#include "ConsoleAdd.h"
#include "ConsoleHelper.h"
#include "ConsoleExtract.h"
#include "OP2Utility.h"
#include <iostream>
#include <memory>
#include <stdexcept>

using namespace std;
using namespace Archives;

void ConsoleAdd::AddCommand(const ConsoleArgs& consoleArgs)
{
	string archiveFilename = GetArchiveName(consoleArgs);
	
	vector<string> filesToAdd = GetFilesToModify(consoleArgs);
	
	if (!consoleArgs.consoleSettings.quiet)
		OutputInitialAddMessage(archiveFilename, filesToAdd.size());

	CheckFilesExist(filesToAdd);

	vector<string> extractedFiles = ExtractFiles(archiveFilename, filesToAdd, consoleArgs.consoleSettings.overwrite);

	filesToAdd.insert(filesToAdd.end(), extractedFiles.begin(), extractedFiles.end());

	CreateModifiedArchive(archiveFilename, filesToAdd, consoleArgs.consoleSettings.quiet);
}

void ConsoleAdd::OutputInitialAddMessage(const string& archiveFilename, int fileCountToAdd)
{
	cout << "Attempting to add " << fileCountToAdd << " file(s) to the archive " << archiveFilename << endl;
	cout << ConsoleHelper::dashedLine << endl;
}

bool ConsoleAdd::ArchivedFileTaggedForOverwrite(const string& internalFilename, const vector<string>& filesToAdd)
{
	for (string fileToAdd : filesToAdd)
	{
		if (XFile::pathsAreEqual(XFile::getFilename(fileToAdd), internalFilename))
			return true;
	}

	return false;
}

vector<string> ConsoleAdd::ExtractFiles(const string& archiveFilename, const vector<string>& internalFilenames, bool overwrite)
{
	unique_ptr<ArchiveFile> archive = ConsoleHelper::OpenArchive(archiveFilename);

	for (int i = 0; i < archive->GetNumberOfPackedFiles(); ++i)
	{
		string internalFilename = archive->GetInternalFileName(i);

		bool taggedForOverwrite = ArchivedFileTaggedForOverwrite(internalFilename, internalFilenames);

		if (taggedForOverwrite && !overwrite)
			throw runtime_error("ADD aborted. " + internalFilename + " is already contained in " + archiveFilename + ". To overwrite, add argument -O.");

		if (!taggedForOverwrite)
		{
			bool extractionSuccessful = archive->ExtractFile(i, XFile::appendSubDirectory(internalFilename, tempDirectory).c_str());

			if (!extractionSuccessful)
				throw runtime_error("Error extracting " + internalFilename + " from archive " + archiveFilename + ". Add operation aborted.");
		}
	}		

	return XFile::getFilesFromDirectory(tempDirectory);
}

void ConsoleAdd::CheckFilesExist(const vector<string>& filenames)
{
	for (string filename : filenames)
	{
		if (!XFile::isFile(filename))
			throw runtime_error(filename + " was not found. Operation aborted.");
	}
}