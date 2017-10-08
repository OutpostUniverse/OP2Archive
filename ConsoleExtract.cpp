#include "ConsoleExtract.h"
#include "ConsoleHelper.h"
#include <iostream>
#include <stdexcept>
#include <memory>

using namespace std;
using namespace Archives;

void ConsoleExtract::extractCommand(const ConsoleArgs& consoleArgs)
{
	ConsoleHelper::checkIfPathsEmpty(consoleArgs.paths);

	string pathToExtractFrom = consoleArgs.paths[0];


	if (ConsoleHelper::isArchiveExtension(pathToExtractFrom))
	{
		vector<string> filesToExtract(consoleArgs.paths.begin() + 1, consoleArgs.paths.end());
		extractFromArchive(pathToExtractFrom, filesToExtract, consoleArgs.consoleSettings);
	}
	else if (XFile::isDirectory(pathToExtractFrom))
	{
		extractFromDirectory(pathToExtractFrom, consoleArgs.consoleSettings);
	}
	else
	{
		throw runtime_error("You must specify either source archive file (.vol|.clm) or directory to extract from.");
	}
}

void ConsoleExtract::extractFromDirectory(const string& directory, const ConsoleSettings& consoleSettings)
{
	vector<string> archiveFilenames = ConsoleHelper::getArchiveFilenames(directory);

	for (string archiveFilename : archiveFilenames)
	{
		string archivePath = XFile::appendSubDirectory(archiveFilename, directory);
		unique_ptr<ArchiveFile> archive = ConsoleHelper::openArchive(archivePath);
		extractAllFiles(*archive, consoleSettings);
	}
}

void ConsoleExtract::extractFromArchive(const string& archiveFilename, const vector<string>& filesToExtract, const ConsoleSettings& consoleSettings)
{
	unique_ptr<ArchiveFile> archiveFile = ConsoleHelper::openArchive(archiveFilename);

	// If no files provided, extract entire contents of archive.
	if (filesToExtract.size() == 0)
	{
		extractAllFiles(*archiveFile, consoleSettings);
		return;
	}

	// If specific files provided, only extract provided files.
	for (size_t i = 0; i < filesToExtract.size(); ++i)
		extractSpecificFile(*archiveFile, filesToExtract[i], consoleSettings);
}

void ConsoleExtract::extractAllFiles(ArchiveFile& archiveFile, const ConsoleSettings& consoleSettings)
{
	if (!consoleSettings.quiet)
		cout << "Extracting all " << archiveFile.GetNumberOfPackedFiles() << " file(s) from archive " << archiveFile.GetVolumeFileName() << "." << endl;

	for (int i = 0; i < archiveFile.GetNumberOfPackedFiles(); ++i)
		extractSpecificFile(archiveFile, archiveFile.GetInternalFileName(i), consoleSettings);

	if (!consoleSettings.quiet)
		cout << "Extraction Finished." << endl;
}

void ConsoleExtract::extractSpecificFile(ArchiveFile& archiveFile, const string& filename, const ConsoleSettings& consoleSettings)
{
	if (!XFile::pathExists(consoleSettings.destDirectory))
		XFile::createDirectory(consoleSettings.destDirectory);

	string destPath = XFile::replaceFilename(consoleSettings.destDirectory, filename).c_str();
	int archiveFileIndex = archiveFile.GetInternalFileIndex(filename.c_str());
	
	if (!consoleSettings.overwrite)
	{
		if (checkIfFileExists(destPath, consoleSettings.quiet))
			return;
	}
		

	bool success = archiveFile.ExtractFile(archiveFileIndex, destPath.c_str());

	if (!consoleSettings.quiet)
		outputExtractionMessage(success, filename);
}

bool ConsoleExtract::checkIfFileExists(string path, bool quiet)
{
	if (XFile::pathExists(path))
	{
		if (!quiet)
			cerr << "A file with the same name already exists at " + path + "." << endl;

		return true;
	}

	return false;
}

void ConsoleExtract::outputExtractionMessage(bool success, string filename)
{
	if (success)
		cout << filename << " extracted." << endl;
	else
		cerr << "Error extracting " << filename << endl;
}