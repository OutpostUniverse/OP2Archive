#include "ConsoleExtract.h"
#include "ConsoleHelper.h"
#include "OP2Utility.h"
#include <iostream>

using namespace Archives;

void ConsoleExtract::extractCommand(const ConsoleArgs& consoleArgs)
{
	if (consoleArgs.paths.size() == 0)
		throw exception("You must specify either a filename to extract or a source archive file (.vol|.clm) to extract from.");

	if (ConsoleHelper::isArchiveExtension(consoleArgs.paths[0]))
		extractSpecificArchive(consoleArgs);
	else
		consoleExtractFiles(consoleArgs);
}

void ConsoleExtract::consoleExtractFiles(const ConsoleArgs& consoleArgs)
{
	for (string path : consoleArgs.paths)
	{
		if (XFile::isDirectory(path))
			consoleExtractDirectory(path, consoleArgs.consoleSettings);
		else
			consoleExtractFile(path, consoleArgs.consoleSettings);
	}
}

void ConsoleExtract::extractSpecificArchive(const ConsoleArgs& consoleArgs)
{
	ArchiveFile* archiveFile = ConsoleHelper::openArchive(consoleArgs.paths[0]);

	// If no files provided, extract entire contents of archive.
	if (consoleArgs.paths.size() == 1)
	{
		extractAllFilesSpecificArchive(archiveFile, consoleArgs.consoleSettings);
		return;
	}

	// If specific files provided, only extract provided files.
	for (size_t i = 1; i < consoleArgs.paths.size(); ++i)
		extractFileSpecificArchive(archiveFile, consoleArgs.paths[i], consoleArgs.consoleSettings);

	delete archiveFile;
}


void ConsoleExtract::extractFileSpecificArchive(ArchiveFile* archiveFile, const string& filename, const ConsoleSettings& consoleSettings)
{
	if (!XFile::pathExists(consoleSettings.destDirectory))
		XFile::createDirectory(consoleSettings.destDirectory);

	bool success = archiveFile->ExtractFile(
		archiveFile->GetInternalFileIndex(filename.c_str()),
		XFile::replaceFilename(consoleSettings.destDirectory, filename).c_str());

	if (consoleSettings.quiet)
		return;

	if (success)
		cout << filename << " extracted." << endl;
	else
		cout << "Error extracting " << filename << endl;
}

void ConsoleExtract::extractAllFilesSpecificArchive(ArchiveFile* archiveFile, const ConsoleSettings& consoleSettings)
{
	if (!consoleSettings.quiet)
		cout << "Extracting all " << archiveFile->GetNumberOfPackedFiles() << " file(s) from archive " << archiveFile->GetVolumeFileName() << "." << endl;

	for (int i = 0; i < archiveFile->GetNumberOfPackedFiles(); ++i)
		extractFileSpecificArchive(archiveFile, archiveFile->GetInternalFileName(i), consoleSettings);

	if (!consoleSettings.quiet)
		cout << "Extraction Finished." << endl;
}

void ConsoleExtract::consoleExtractDirectory(const string& directory, const ConsoleSettings& consoleSettings)
{
	vector<ArchiveFile*> archives = openArchivesInDirectory(directory);

	for (ArchiveFile* archive : archives)
	{
		extractAllFilesSpecificArchive(archive, consoleSettings);
		delete archive;
	}
}

void ConsoleExtract::consoleExtractFile(const string& internalFilename, const ConsoleSettings& consoleSettings)
{
	vector<ArchiveFile*> archives = openArchivesInDirectory("./");

	// TODO: Change to ResourceManager...
}

vector<ArchiveFile*> ConsoleExtract::openArchivesInDirectory(const string& directory)
{
	vector<string> archiveFilenames = XFile::getFilesFromDirectory("./", "vol");
	vector<string> clmFilenames = XFile::getFilesFromDirectory("./", "clm");

	archiveFilenames.insert(std::end(archiveFilenames), std::begin(clmFilenames), std::end(clmFilenames));

	vector<ArchiveFile*> archives;

	for (string filename : archiveFilenames)
		archives.push_back(ConsoleHelper::openArchive(filename));

	return archives;
}