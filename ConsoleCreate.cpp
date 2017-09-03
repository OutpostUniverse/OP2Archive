#include "ConsoleCreate.h"
#include <iostream>
#include "ConsoleHelper.h"

ArchiveFile* createArchiveTemplate(const string& archiveFilename)
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

void outputCreateBegan(const string& archiveFilename, int packedFileCount)
{
	cout << "Creating archive " << archiveFilename << ", containing " << packedFileCount << " file(s)." << endl;
	cout << dashedLine << endl;
}

void outputCreateResults(bool success, int packedFileCount)
{
	if (success)
	{
		cout << "Archive created." << endl;

		if (packedFileCount < 1)
			cout << "Caution: Created archive is empty (contains no files)." << endl;
	}
	else
		cerr << "Error creating archive." << endl;
}

void createArchiveFile(const string& archiveFilename, const vector<string>& filenames, const ConsoleSettings& consoleSettings)
{
	ArchiveFile* archiveFile = createArchiveTemplate(archiveFilename);

	vector<string> internalFilenames;

	for (string filename : filenames)
		internalFilenames.push_back(XFile::getFilename(filename));

	if (!consoleSettings.quiet)
		outputCreateBegan(archiveFilename, filenames.size());

	const char** filenamesCArray = StringHelper::vectorToCharArray(filenames);
	const char** internalFilenamesCArray = StringHelper::vectorToCharArray(internalFilenames);

	bool success = archiveFile->CreateVolume(archiveFilename.c_str(), filenames.size(), filenamesCArray, internalFilenamesCArray);

	delete filenamesCArray;
	delete internalFilenamesCArray;
	delete archiveFile;

	if (!consoleSettings.quiet)
		outputCreateResults(success, filenames.size());
}

void createUsingDefaultDirectory(const string& archiveFilename, const ConsoleSettings& consoleSettings)
{
	string sourceDir = XFile::changeFileExtension(archiveFilename, "");

	if (!XFile::isDirectory(sourceDir))
		throw exception(("The directory " + sourceDir + " does not exist. Either create this directory or explicity specify the source directory/files.").c_str());

	vector<string> filenames = XFile::getFilesFromDirectory(sourceDir);

	createArchiveFile(archiveFilename, filenames, consoleSettings);
}

vector<string> gatherFilesForArchive(const vector<string>& paths)
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

void checkCreateOverwrite(const string& archiveFilename, bool overwrite, bool quiet)
{
	if (XFile::pathExists(archiveFilename))
	{
		if (!overwrite)
			throw exception("Archive file already exists at specified path. If overwrite is desired add argument -o.");
		else if (overwrite && !quiet)
			cout << "An archive file already exists at the specified path. Overwrite authorized by user." << endl;
	}
}

void createCommand(const ConsoleArgs& consoleArgs)
{
	checkIfPathsEmpty(consoleArgs);

	string archiveFilename = consoleArgs.paths[0];

	if (!isArchiveExtension(archiveFilename))
		throw exception("A .vol or .clm filename must be provided to create an archive.");

	checkCreateOverwrite(archiveFilename, consoleArgs.consoleSettings.overwrite, consoleArgs.consoleSettings.quiet);

	if (consoleArgs.paths.size() == 1)
	{
		if (false) // Brett 23AUG17: Currently no way to specify to use the default source directory.
			createUsingDefaultDirectory(archiveFilename, consoleArgs.consoleSettings);

		createArchiveFile(archiveFilename, vector<string>(), consoleArgs.consoleSettings);
	}
	else
	{
		vector<string> filenames = gatherFilesForArchive(consoleArgs.paths);
		createArchiveFile(archiveFilename, filenames, consoleArgs.consoleSettings);
	}
}