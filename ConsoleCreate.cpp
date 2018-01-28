#include "ConsoleCreate.h"
#include "ConsoleHelper.h"
#include <iostream>
#include <stdexcept>

using namespace std;
using namespace Archives;

void ConsoleCreate::CreateCommand(const ConsoleArgs& consoleArgs)
{
	ConsoleHelper::CheckIfPathsEmpty(consoleArgs.paths);

	string archiveFilename = consoleArgs.paths[0];

	if (!ConsoleHelper::IsArchiveExtension(archiveFilename))
		throw runtime_error("A .vol or .clm filename must be provided to create an archive.");

	CheckCreateOverwrite(archiveFilename, consoleArgs.consoleSettings.overwrite, consoleArgs.consoleSettings.quiet);

	if (consoleArgs.paths.size() == 1)
	{
		if (false) // Brett 23AUG17: Currently no way to specify to use the default source directory.
			CreateUsingDefaultDirectory(archiveFilename, consoleArgs.consoleSettings);

		CreateArchiveFile(archiveFilename, vector<string>(), consoleArgs.consoleSettings.quiet);
	}
	else
	{
		vector<string> filenames = GatherFilesForArchive(consoleArgs.paths);
		CreateArchiveFile(archiveFilename, filenames, consoleArgs.consoleSettings.quiet);
	}
}

void ConsoleCreate::CreateArchiveFile(const string& archiveFilename, const vector<string>& paths, bool quiet)
{
	vector<string> sortedPaths = SortPathsByFilename(paths);
	vector<string> filenames = GetFilenamesFromPaths(sortedPaths);

	unique_ptr<ArchiveFile> archiveFile = CreateArchiveTemplate(archiveFilename);

	if (!quiet) {
		OutputInitialCreateMessage(archiveFilename, sortedPaths.size());
	}

	const char** pathsCArray = StringHelper::VectorToCharArray(sortedPaths);
	const char** filenamesCArray = StringHelper::VectorToCharArray(filenames);

	bool success = archiveFile->CreateVolume(archiveFilename.c_str(), sortedPaths.size(), pathsCArray, filenamesCArray);

	delete pathsCArray;
	delete filenamesCArray;

	if (!success) {
		throw runtime_error("Error creating archive.");
	}

	if (!quiet) {
		OutputCreateResults(sortedPaths.size(), archiveFilename);
	}
}

unique_ptr<ArchiveFile> ConsoleCreate::CreateArchiveTemplate(const string& archiveFilename)
{
	if (XFile::ExtensionMatches(archiveFilename, "VOL"))
		return make_unique<VolFile>("VolTemplate.vol");

	if (XFile::ExtensionMatches(archiveFilename, "CLM"))
		return make_unique<ClmFile>("ClmTemplate.clm");

	throw runtime_error("Unable to open archive template files VolTemplate.vol and/or ClmTemplate.clm. Ensure both files exist in same directory as OP2Archive.exe and are not open in another application.");
}

void ConsoleCreate::CreateUsingDefaultDirectory(const string& archiveFilename, const ConsoleSettings& consoleSettings)
{
	string sourceDir = XFile::ChangeFileExtension(archiveFilename, "");

	if (!XFile::IsDirectory(sourceDir))
		throw runtime_error("The directory " + sourceDir + " does not exist. Either create this directory or explicity specify the source directory/files.");

	vector<string> filenames = XFile::GetFilesFromDirectory(sourceDir);

	CreateArchiveFile(archiveFilename, filenames, consoleSettings.quiet);
}

vector<string> ConsoleCreate::GatherFilesForArchive(const vector<string>& paths)
{
	vector<string> filenames;

	for (size_t i = 1; i < paths.size(); i++) //Skip the first path since it is the archive name.
	{
		if (XFile::IsDirectory(paths[i]))
		{
			vector<string> dirFilenames = XFile::GetFilesFromDirectory(paths[i]);
			filenames.insert(std::end(filenames), std::begin(dirFilenames), std::end(dirFilenames));
		}
		else
			filenames.push_back(paths[i]);
	}

	return filenames;
}

void ConsoleCreate::CheckCreateOverwrite(const string& archiveFilename, bool overwrite, bool quiet)
{
	if (XFile::PathExists(archiveFilename))
	{
		if (!overwrite)
			throw runtime_error("Archive file already exists at specified path. If overwrite is desired add argument -o.");
		else if (overwrite && !quiet)
			cout << "An archive file already exists at the specified path. Overwrite authorized by user." << endl << endl;
	}
}

void ConsoleCreate::OutputInitialCreateMessage(const string& archiveFilename, int packedFileCount)
{
	cout << "Creating archive " << archiveFilename << ", containing " << packedFileCount << " file(s)." << endl;
	cout << ConsoleHelper::dashedLine << endl;
}

void ConsoleCreate::OutputCreateResults(int packedFileCount, const string& archiveFilename)
{
		cout << "Archive created." << endl << endl;

		if (packedFileCount < 1)
			cout << "Caution: Created archive is empty (contains no files)." << endl;
		else
			ConsoleHelper::ListContentsOfArchive(archiveFilename);
}

vector<string> ConsoleCreate::SortPathsByFilename(vector<string> paths)
{
	vector<string> sortedPaths(paths);
	stable_sort(sortedPaths.begin(), sortedPaths.end(), ComparePathFilenames);

	return sortedPaths;
}

// Compares 2 filenames to determine which comes first alphabetically.
// Does not compare the entire path, but only the filename.
bool ConsoleCreate::ComparePathFilenames(const string path1, const string path2)
{
	return XFile::GetFilename(path1) < XFile::GetFilename(path2);
}

vector<string> ConsoleCreate::GetFilenamesFromPaths(vector<string> paths)
{
	vector<string> filenames;

	for (string filename : paths) {
		filenames.push_back(XFile::GetFilename(filename));
	}

	return filenames;
}
