#include "ConsoleAdd.h"
#include "ConsoleHelper.h"
#include "ConsoleExtract.h"
#include "OP2Utility.h"
#include <iostream>
#include <memory>
#include <stdexcept>

using namespace std;
using namespace Archive;

void ConsoleAdd::AddCommand(const ConsoleArgs& consoleArgs)
{
	string archiveFilename = GetArchiveName(consoleArgs);
	
	vector<string> filesToAdd = GetFilesToModify(consoleArgs);
	
	if (!consoleArgs.consoleSettings.quiet) {
		OutputInitialAddMessage(archiveFilename, filesToAdd.size());
	}

	CheckFilesExist(filesToAdd);

	vector<string> extractedFiles = ExtractFilesFromOriginalArchive(archiveFilename, filesToAdd, consoleArgs.consoleSettings.overwrite);

	filesToAdd.insert(filesToAdd.end(), extractedFiles.begin(), extractedFiles.end());

	CreateModifiedArchive(archiveFilename, filesToAdd, consoleArgs.consoleSettings.quiet);
}

void ConsoleAdd::OutputInitialAddMessage(const string& archiveFilename, std::size_t fileCountToAdd)
{
	cout << "Attempting to add " << fileCountToAdd << " file(s) to the archive " << archiveFilename << endl;
	cout << ConsoleHelper::dashedLine << endl;
}

bool ConsoleAdd::ArchivedFileTaggedForOverwrite(const string& internalFilename, const vector<string>& filesToAdd)
{
	for (const auto& fileToAdd : filesToAdd)
	{
		if (XFile::PathsAreEqual(XFile::GetFilename(fileToAdd), internalFilename)) {
			return true;
		}
	}

	return false;
}

// The ADD command actually extracts the contents of an archive, deletes the archive, and then recreates the archive.
vector<string> ConsoleAdd::ExtractFilesFromOriginalArchive(const string& archiveFilename, const vector<string>& internalFilenames, bool overwrite)
{
	unique_ptr<ArchiveFile> archive = ConsoleHelper::OpenArchive(archiveFilename);

	for (std::size_t i = 0; i < archive->GetCount(); ++i)
	{
		string internalFilename = archive->GetName(i);

		bool taggedForOverwrite = ArchivedFileTaggedForOverwrite(internalFilename, internalFilenames);

		if (taggedForOverwrite && !overwrite) {
			throw runtime_error("ADD aborted. " + internalFilename + " is already contained in " + archiveFilename + ". To overwrite, add argument -O.");
		}

		if (!taggedForOverwrite)
		{
			try
			{
				archive->ExtractFile(i, XFile::AppendSubDirectory(internalFilename, tempDirectory));
			}
			catch (const std::exception& e)
			{
				throw runtime_error("Error extracting " + internalFilename + " from archive " + archiveFilename + ". Add operation aborted. Internal error: " + e.what());
			}
		}
	}		

	return XFile::GetFilesFromDirectory(tempDirectory);
}

void ConsoleAdd::CheckFilesExist(const vector<string>& filenames)
{
	for (const auto& filename : filenames)
	{
		if (!XFile::IsFile(filename)) {
			throw runtime_error(filename + " was not found. Operation aborted.");
		}
	}
}
