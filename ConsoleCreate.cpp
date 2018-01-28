#include "ConsoleCreate.h"
#include "ConsoleHelper.h"
#include <iostream>
#include <stdexcept>

using namespace std;
using namespace Archives;

// Cannot exist inside the class definition and work as a function object for passing into std::stable_sort.
bool comparePathFilenames(const std::string path1, const std::string path2);

void ConsoleCreate::createCommand(const ConsoleArgs& consoleArgs)
{
	ConsoleHelper::checkIfPathsEmpty(consoleArgs.paths);

	string archiveFilename = consoleArgs.paths[0];

	if (!ConsoleHelper::isArchiveExtension(archiveFilename))
		throw runtime_error("A .vol or .clm filename must be provided to create an archive.");

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

void ConsoleCreate::createArchiveFile(const string& archiveFilename, const vector<string>& paths, bool quiet)
{
	vector<string> sortedPaths = sortPathsByFilename(paths);
	vector<string> filenames = getFilenamesFromPaths(sortedPaths);

	unique_ptr<ArchiveFile> archiveFile = createArchiveTemplate(archiveFilename);

	if (!quiet) {
		outputInitialCreateMessage(archiveFilename, sortedPaths.size());
	}

	const char** pathsCArray = StringHelper::vectorToCharArray(sortedPaths);
	const char** filenamesCArray = StringHelper::vectorToCharArray(filenames);

	bool success = archiveFile->CreateVolume(archiveFilename.c_str(), sortedPaths.size(), pathsCArray, filenamesCArray);

	delete pathsCArray;
	delete filenamesCArray;

	if (!success) {
		throw runtime_error("Error creating archive.");
	}

	if (!quiet) {
		outputCreateResults(sortedPaths.size(), archiveFilename);
	}
}

unique_ptr<ArchiveFile> ConsoleCreate::createArchiveTemplate(const string& archiveFilename)
{
	if (XFile::extensionMatches(archiveFilename, "VOL"))
		return make_unique<VolFile>("VolTemplate.vol");

	if (XFile::extensionMatches(archiveFilename, "CLM"))
		return make_unique<ClmFile>("ClmTemplate.clm");

	throw runtime_error("Unable to open archive template files VolTemplate.vol and/or ClmTemplate.clm. Ensure both files exist in same directory as OP2Archive.exe and are not open in another application.");
}

void ConsoleCreate::createUsingDefaultDirectory(const string& archiveFilename, const ConsoleSettings& consoleSettings)
{
	string sourceDir = XFile::changeFileExtension(archiveFilename, "");

	if (!XFile::isDirectory(sourceDir))
		throw runtime_error("The directory " + sourceDir + " does not exist. Either create this directory or explicity specify the source directory/files.");

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
			throw runtime_error("Archive file already exists at specified path. If overwrite is desired add argument -o.");
		else if (overwrite && !quiet)
			cout << "An archive file already exists at the specified path. Overwrite authorized by user." << endl << endl;
	}
}

void ConsoleCreate::outputInitialCreateMessage(const string& archiveFilename, int packedFileCount)
{
	cout << "Creating archive " << archiveFilename << ", containing " << packedFileCount << " file(s)." << endl;
	cout << ConsoleHelper::dashedLine << endl;
}

void ConsoleCreate::outputCreateResults(int packedFileCount, const string& archiveFilename)
{
		cout << "Archive created." << endl << endl;

		if (packedFileCount < 1)
			cout << "Caution: Created archive is empty (contains no files)." << endl;
		else
			ConsoleHelper::listContentsOfArchive(archiveFilename);
}

vector<string> ConsoleCreate::sortPathsByFilename(vector<string> paths)
{
	vector<string> sortedPaths(paths);
	stable_sort(sortedPaths.begin(), sortedPaths.end(), comparePathFilenames);

	return sortedPaths;
}

// Compares 2 filenames to determine which comes first alphabetically.
// Does not compare the entire path, but only the filename.
bool comparePathFilenames(const string path1, const string path2)
{
	return XFile::getFilename(path1) < XFile::getFilename(path2);
}

vector<string> ConsoleCreate::getFilenamesFromPaths(vector<string> paths)
{
	vector<string> filenames;

	for (string filename : paths) {
		filenames.push_back(XFile::getFilename(filename));
	}

	return filenames;
}
