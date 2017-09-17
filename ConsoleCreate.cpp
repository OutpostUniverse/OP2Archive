#include "ConsoleCreate.h"
#include "ConsoleHelper.h"
#include <iostream>

void ConsoleCreate::createCommand(const ConsoleArgs& consoleArgs)
{
	ConsoleHelper::checkIfPathsEmpty(consoleArgs.paths);

	string archiveFilename = consoleArgs.paths[0];

	if (!ConsoleHelper::isArchiveExtension(archiveFilename))
		throw exception("A .vol or .clm filename must be provided to create an archive.");

	checkCreateOverwrite(archiveFilename, consoleArgs.consoleSettings.overwrite, consoleArgs.consoleSettings.quiet);

	if (consoleArgs.paths.size() == 1)
	{
		if (false) // Brett 23AUG17: Currently no way to specify to use the default source directory.
			createUsingDefaultDirectory(archiveFilename, consoleArgs.consoleSettings);

		createArchiveFile(archiveFilename, vector<string>(), consoleArgs.consoleSettings.quiet);
	}
	else
	{
		vector<string> filenames = gatherFilesForArchive(consoleArgs.paths);
		createArchiveFile(archiveFilename, filenames, consoleArgs.consoleSettings.quiet);
	}
}

void ConsoleCreate::createArchiveFile(const string& archiveFilename, const vector<string>& filenames, bool quiet)
{
	ArchiveFile* archiveFile = createArchiveTemplate(archiveFilename);

	vector<string> internalFilenames;

	for (string filename : filenames)
		internalFilenames.push_back(XFile::getFilename(filename));

	if (!quiet)
		outputInitialCreateMessage(archiveFilename, filenames.size());

	const char** filenamesCArray = StringHelper::vectorToCharArray(filenames);
	const char** internalFilenamesCArray = StringHelper::vectorToCharArray(internalFilenames);

	bool success = archiveFile->CreateVolume(archiveFilename.c_str(), filenames.size(), filenamesCArray, internalFilenamesCArray);

	delete filenamesCArray;
	delete internalFilenamesCArray;
	delete archiveFile;

	if (!success)
		throw(exception("Error creating archive."));

	if (!quiet)
		outputCreateResults(filenames.size());
}

ArchiveFile* ConsoleCreate::createArchiveTemplate(const string& archiveFilename)
{
	ArchiveFile* archiveFile;

	if (XFile::extensionMatches(archiveFilename, "VOL"))
		archiveFile = new VolFile("VolTemplate.vol");
	else if (XFile::extensionMatches(archiveFilename, "CLM"))
		archiveFile = new ClmFile("ClmTemplate.clm");
	else
		throw exception("An archive filename must be provided (.vol|.clm).");

	return archiveFile;
}

void ConsoleCreate::createUsingDefaultDirectory(const string& archiveFilename, const ConsoleSettings& consoleSettings)
{
	string sourceDir = XFile::changeFileExtension(archiveFilename, "");

	if (!XFile::isDirectory(sourceDir))
		throw exception(("The directory " + sourceDir + " does not exist. Either create this directory or explicity specify the source directory/files.").c_str());

	vector<string> filenames = XFile::getFilesFromDirectory(sourceDir);

	createArchiveFile(archiveFilename, filenames, consoleSettings.quiet);
}

vector<string> ConsoleCreate::gatherFilesForArchive(const vector<string>& paths)
{
	vector<string> filenames;

	for (size_t i = 1; i < paths.size(); i++) //Skip the first path since it is the archive name.
	{
		if (XFile::isDirectory(paths[i]))
		{
			vector<string> dirFilenames = XFile::getFilesFromDirectory(paths[i]);
			filenames.insert(std::end(filenames), std::begin(dirFilenames), std::end(dirFilenames));
		}
		else
			filenames.push_back(paths[i]);
	}

	return filenames;
}

void ConsoleCreate::checkCreateOverwrite(const string& archiveFilename, bool overwrite, bool quiet)
{
	if (XFile::pathExists(archiveFilename))
	{
		if (!overwrite)
			throw exception("Archive file already exists at specified path. If overwrite is desired add argument -o.");
		else if (overwrite && !quiet)
			cout << "An archive file already exists at the specified path. Overwrite authorized by user." << endl;
	}
}

void ConsoleCreate::outputInitialCreateMessage(const string& archiveFilename, int packedFileCount)
{
	cout << "Creating archive " << archiveFilename << ", containing " << packedFileCount << " file(s)." << endl;
	cout << ConsoleHelper::dashedLine << endl;
}

void ConsoleCreate::outputCreateResults(int packedFileCount)
{
		cout << "Archive created." << endl;

		if (packedFileCount < 1)
			cout << "Caution: Created archive is empty (contains no files)." << endl;
}