#include <iostream>
#include <string>
#include <vector>
#include "OP2Utility.h"
#include "ArchiveConsoleListing.h"
#include "ConsoleArgumentParser.h"
#include "ConsoleExtract.h"
#include <cstdlib>

using namespace std;
using namespace ConsoleArgumentParser;

void outputHelp();

static string version = "0.1.0";
const string dashedLine = "--------------------------------------------------";

ArchiveConsoleListing archiveConsoleListing;

bool isArchiveExtension(const std::string& filename)
{
	return XFile::extensionMatches(filename, "VOL") || XFile::extensionMatches(filename, "CLM");
}

void listArchiveContent(const string& filename)
{
	if (XFile::extensionMatches(filename, ".vol"))
		archiveConsoleListing.listContents(new VolFile(filename.c_str()));
	else if (XFile::extensionMatches(filename, ".clm"))
		archiveConsoleListing.listContents(new ClmFile(filename.c_str()));
}

void listAllArchivesInDirectory(const string& directory)
{
	ResourceManager resourceManager(directory);
	vector<string> volFilenames = resourceManager.getAllFilenamesOfType(directory, ".vol", false);
	vector<string> clmFilenames = resourceManager.getAllFilenames(directory, ".clm", false);

	cout << volFilenames.size() << " vol archive file(s) located." << endl;
	cout << clmFilenames.size() << " clm archive file(s) located." << endl;
	cout << dashedLine << endl << endl;

	for (const string& filename : volFilenames)
		listArchiveContent(filename);

	for (const string& filename : clmFilenames)
		listArchiveContent(filename);
}

void checkIfPathsEmpty(const ConsoleArgs& consoleArgs)
{
	if (consoleArgs.paths.size() == 0)
		throw exception("You must provide at least one file or directory. To provide the current directory, enter './'.");
}

void locateFileInArchives(const string& path)
{
	ResourceManager resourceManager(XFile::getDirectory(path));

	string filename = XFile::getFilename(path);

	string archiveFilename = resourceManager.findContainingArchiveFile(filename);

	if (archiveFilename == "")
		cout << "The file " << filename << " is not located in an archive at the supplied directory." << endl << endl;
	else
		cout << "The file " << filename << " is located in the archive " << archiveFilename << "." << endl << endl;
}

void locateCommand(const ConsoleArgs& consoleArgs)
{
	checkIfPathsEmpty(consoleArgs);

	for (string path : consoleArgs.paths)
		locateFileInArchives(path);
}

void listCommand(const ConsoleArgs& consoleArgs)
{
	checkIfPathsEmpty(consoleArgs);

	for (string path : consoleArgs.paths)
	{
		if (XFile::isDirectory(path))
			listAllArchivesInDirectory(path);
		else if (isArchiveExtension(path))
			listArchiveContent(path);
		else
			throw exception("You must provide either a directory or a file of type (.vol|.clm).");
	}
}

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

void extractCommand(const ConsoleArgs& consoleArgs)
{
	if (consoleArgs.paths.size() == 0)
		throw exception("You must specify either a filename to extract or a source archive file (.vol|.clm) to extract from.");

	if (isArchiveExtension(consoleArgs.paths[0]))
		extractSpecificArchive(consoleArgs);
	else
		consoleExtractFiles(consoleArgs);
}

string createTempDirectory()
{
	srand((int)time(NULL)); // For creating a unique directory.
	int number = rand();

	string directory("./OP2ArchiveTemp-" + to_string(number));

	XFile::createDirectory(directory);

	return directory;
}

vector<string>* removeMatchingStrings(const vector<string>& strings, const vector<string>& stringsToRemove)
{
	vector<string>* stringsToReturn = new vector<string>(strings.begin(), strings.end());

	auto pred = [&stringsToRemove](const std::string& key) ->bool
	{
		return std::find(stringsToRemove.begin(), stringsToRemove.end(), key) != stringsToRemove.end();
	};

	stringsToReturn->erase(std::remove_if(stringsToReturn->begin(), stringsToReturn->end(), pred), stringsToReturn->end());

	return stringsToReturn;
}

vector<string>* removeFilenames(ArchiveFile* archive, const vector<string>& filesToRemove)
{
	vector<string> internalFilenames;

	for (int i = 0; i < archive->GetNumberOfPackedFiles(); ++i)
		internalFilenames.push_back(archive->GetInternalFileName(i));

	return removeMatchingStrings(internalFilenames, filesToRemove);
}

void throwUnfoundFileDuringRemoveException(vector<string>* unfoundFilenames)
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

void checkFilesAvailableToRemove(ArchiveFile* archive, const vector<string>& filesToRemove, bool quiet)
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

vector<string>* getFilesToRemove(const ConsoleArgs& consoleArgs)
{
	vector<string>* filesToRemove = new vector<string>(consoleArgs.paths.begin() + 1, consoleArgs.paths.end());

	if (filesToRemove->size() == 0)
		throw exception("No file(s) provided to remove from the archive.");

	return filesToRemove;
}

ArchiveFile* checkAndOpenArchive(const ConsoleArgs& consoleArgs)
{
	if (consoleArgs.paths.size() == 0)
		throw exception("No archive filename provided.");

	const string archiveFilename = consoleArgs.paths[0];

	return openArchive(archiveFilename);
}

void removeCommand(const ConsoleArgs& consoleArgs)
{
	ArchiveFile* archive = checkAndOpenArchive(consoleArgs);
	vector<string>* filesToRemove = getFilesToRemove(consoleArgs);

	checkFilesAvailableToRemove(archive, *filesToRemove, consoleArgs.consoleSettings.quiet);

	const vector<string>* archiveInternalFilenames = removeFilenames(archive, *filesToRemove);

	const string directory = createTempDirectory();


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
	
	string archiveFilename = archive->GetVolumeFileName();
	delete dynamic_cast<ClmFile*>(archive);

	XFile::deletePath(archiveFilename);

	vector<string> filenames = XFile::getFilesFromDirectory(directory);
	createArchiveFile(archiveFilename, filenames, consoleArgs.consoleSettings);

	delete archiveInternalFilenames;
	XFile::deletePath(directory);

	delete filesToRemove;
}

void addCommand(const ConsoleArgs& consoleArgs)
{

}

void selectCommand(const ConsoleArgs& consoleArgs)
{
	switch (consoleArgs.consoleCommand)
	{
	case ConsoleCommand::Help:
		outputHelp();
		break;
	case ConsoleCommand::Create:
		createCommand(consoleArgs);
		break;
	case ConsoleCommand::Extract:
		extractCommand(consoleArgs);
		break;
	case ConsoleCommand::Find:
		locateCommand(consoleArgs);
		break;
	case ConsoleCommand::List:
		listCommand(consoleArgs);
		break;
	case ConsoleCommand::Add:
		addCommand(consoleArgs);
		break;
	case ConsoleCommand::Remove:
		removeCommand(consoleArgs);
		break;
	}
}

void debugPause()
{
#if defined _DEBUG
	getchar();
#endif
}

int main(int argc, char **argv)
{
	try
	{
		ConsoleArgs consoleArgs = sortArguments(argc, argv);
		selectCommand(consoleArgs);
	}
	catch (exception e) {
		cerr << e.what() << endl;
		cerr << "Run without arguments to see usage message." << endl << endl;

		debugPause();

		return 1;
	}

	debugPause();

	return 0;
}

void outputHelp()
{
	cout << endl;
	cout << "OP2Archive Ver " << version << " - Outpost 2 Archive Access and Maintenance" << endl;
	cout << "Developed by Hooman and Brett208 (Vagabond)" << endl;
	cout << endl;
	cout << "Allows examining, creating, and extracting files from Outpost 2 .vol and .clm archives." << endl;
	cout << endl;
	cout << "+++ COMMANDS +++" << endl;
	cout << "  * OP2Archive LIST (archivename.(vol|clm) | directory)..." << endl;
	cout << "  * OP2Archive FIND filename..." << endl;
	cout << "  * OP2Archive CREATE archivename.(vol|clm) [filename | directory]... [-q] [-o] [-c None|LZH]" << endl;
	cout << "      * If no filename(s) or directory(s) provided," << endl;
	cout << "        archives all contents of the default source directory (./archiveFilename)." << endl;
	cout << "  * OP2Archive EXTRACT archivename.(vol|clm) [filename]... [-q] [-d destDirectory] [-o]" << endl;
	cout << "      * If no filename(s) provided, extracts the entire vol file. " << endl;
	cout << "      * Defaults destination for entire vol file is ./archiveFilename." << endl;
	cout << "  * OP2Archive ADD archiveName.[vol|clm] filename..." << endl;
	cout << "  * OP2Archive REMOVE archiveName.[vol|clm] filename..." << endl;
	cout << endl;
	cout << "+++ OPTIONAL ARGUMENTS +++" << endl;
	cout << "  -H / --Help / -?: Displays help information." << endl;
	cout << "  -Q / --Quiet: [Default false] Prevents application from issuing console messages." << endl;
	cout << "  -O / --Overwrite: [Default false] Allows application to overwrite existing files." << endl;
	cout << "  -D / --DestinationDirectory: [Default for single file is './', Default for all files is archive's filename]. " << endl;
	cout << "                               Sets the destination directory for extracted file(s)." << endl;
	cout << "  -S / --SourceDirectory: CREATE: [Deafault is archive's filename]. Sets the source directory when creating an archive." << endl;
	cout << "  -C / --Compression: [Default None]. Sets the compression alghorithim used when creating an archive (None|LZH)." << endl;
	cout << endl;
	cout << "For more information about Outpost 2 visit the Outpost Universe (http://outpost2.net/)." << endl;
	cout << endl;
}