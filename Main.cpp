#include <iostream>
#include <string>
#include <vector>
#include "OP2Utility.h"
#include "ArchiveConsoleListing.h"
#include "ConsoleArgumentParser.h"

using namespace std;
using namespace ConsoleArgumentParser;

void outputHelp();

static string version = "0.1.0";
const string dashedLine = "--------------------------------------------------";

ArchiveConsoleListing archiveConsoleListing;

bool isArchiveFileExtension(const std::string& filename)
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

	for each (const string& filename in volFilenames)
		listArchiveContent(filename);

	for each(const string& filename in clmFilenames)
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

	for each (string path in consoleArgs.paths)
		locateFileInArchives(path);
}

void listCommand(const ConsoleArgs& consoleArgs)
{
	checkIfPathsEmpty(consoleArgs);

	for each (string path in consoleArgs.paths)
	{
		if (XFile::isDirectory(path))
			listAllArchivesInDirectory(path);
		else if (isArchiveFileExtension(path))
			listArchiveContent(path);
		else
			throw exception("You must provide either a directory or a file of type (.vol|.clm).");
	}
}

void consoleExtractFromAllArchivesInDirectory(const ConsoleArgs& consoleArgs)
{
	ResourceManager resourceManager("./");

	vector<string> volFilenames = XFile::getFilesFromDirectory("./", "vol");
	vector<string> clmFilenames = XFile::getFilesFromDirectory("./", "clm");

	for each (string path in consoleArgs.paths)
	{
		
	}
}

ArchiveFile* openArchiveFile(const string& archivePath)
{
	Archives::ArchiveFile* archiveFile;

	if (XFile::extensionMatches(archivePath, "VOL"))
		archiveFile = new Archives::VolFile(archivePath.c_str());
	else
		archiveFile = new Archives::ClmFile(archivePath.c_str());

	return archiveFile;
}

void extractFileFromArchive(ArchiveFile* archiveFile, const string& filename, const ConsoleSettings& consoleSettings)
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

void extractAllFilesFromArchive(ArchiveFile* archiveFile, const ConsoleSettings& consoleSettings)
{
	if (!consoleSettings.quiet)
		cout << "Extracting all " << archiveFile->GetNumberOfPackedFiles() << " files from archive " << archiveFile->GetVolumeFileName() << "." << endl;
	
	for (int i = 0; i < archiveFile->GetNumberOfPackedFiles(); ++i)
		extractFileFromArchive(archiveFile, archiveFile->GetInternalFileName(i), consoleSettings);

	if (!consoleSettings.quiet)
		cout << "Extraction Finished." << endl;
}

void consoleExtractFromSpecificArchive(const ConsoleArgs& consoleArgs)
{
	ArchiveFile* archiveFile = openArchiveFile(consoleArgs.paths[0]);

	if (consoleArgs.paths.size() == 1)
	{
		extractAllFilesFromArchive(archiveFile, consoleArgs.consoleSettings);
		return;
	}
		
	for (size_t i = 1; i < consoleArgs.paths.size(); ++i)
	{
		extractFileFromArchive(archiveFile, consoleArgs.paths[i], consoleArgs.consoleSettings);
	}

	delete archiveFile;
}

void extractFromArchive(const ConsoleArgs& consoleArgs)
{

}

void extractCommand(const ConsoleArgs& consoleArgs)
{
	if (consoleArgs.paths.size() == 0)
		throw exception("You must specify either a filename to extract or a source archive file (.vol|.clm) to extract from.");

	if (isArchiveFileExtension(consoleArgs.paths[0]))
		consoleExtractFromSpecificArchive(consoleArgs);
	else if (XFile::isDirectory(consoleArgs.paths[0]))
		consoleExtractFromAllArchivesInDirectory(consoleArgs);
	else
		extractFromArchive(consoleArgs);
}

void createArchiveFile(const string& archiveFilename, const vector<string>& filenames, const ConsoleSettings& consoleSettings)
{
	ArchiveFile* archiveFile;
	if (XFile::extensionMatches(archiveFilename, "VOL"))
		archiveFile = new VolFile("VolTemplate.vol");
	else if (XFile::extensionMatches(archiveFilename, "CLM"))
		archiveFile = new ClmFile("ClmTemplate.clm");
	else
		throw exception("An archive filename must be provided (.vol|.clm).");

	vector<string> internalFilenames;

	for each (string filename in filenames)
		internalFilenames.push_back(XFile::getFilename(filename));

	if (!consoleSettings.quiet)
	{
		cout << "Creating archive " << archiveFilename << ", containing " << filenames.size() << " file(s)." << endl;
		cout << dashedLine << endl;
	}

	const char** filenamesCArray = StringHelper::vectorToCharArray(filenames);
	const char** internalFilenamesCArray = StringHelper::vectorToCharArray(internalFilenames);

	bool success = archiveFile->CreateVolume(archiveFilename.c_str(), filenames.size(), filenamesCArray, internalFilenamesCArray);

	delete filenamesCArray;
	delete internalFilenamesCArray;
	delete archiveFile;

	if (consoleSettings.quiet)
		return;

	if (success)
	{
		cout << "Archive created." << endl;

		if (filenames.size() == 0)
			cout << "Caution: Created archive is empty (contains no files)." << endl;
	}
	else
		cerr << "Error creating archive." << endl;
}

void createUsingDefaultDirectory(const string& archiveFilename, const ConsoleSettings& consoleSettings)
{
	/*string sourceDir = XFile::changeFileExtension(archiveFilename, "");

	if (!XFile::isDirectory(sourceDir))
		throw exception(("The directory " + sourceDir + " does not exist. Either create this directory or explicity specify the source directory/files.").c_str());

	vector<string> filenamesStr = XFile::getFilesFromDirectory(sourceDir);*/

	createArchiveFile(archiveFilename, vector<string>(), consoleSettings);
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

void createCommand(const ConsoleArgs& consoleArgs)
{
	checkIfPathsEmpty(consoleArgs);

	string archiveFilename = consoleArgs.paths[0];

	if (!isArchiveFileExtension(archiveFilename))
		throw exception("A .vol or .clm filename must be provided to create an archive.");

	if (XFile::pathExists(archiveFilename))
	{
		if (!consoleArgs.consoleSettings.overwrite)
			throw exception("Archive file already exists at specified path. If overwrite is desired add argument -o.");
		else if (consoleArgs.consoleSettings.overwrite && !consoleArgs.consoleSettings.quiet)
			cout << "An archive file already exists at the specified path. Overwrite authorized by user." << endl;
	}

	//TODO: Create an archive with filename as directory source.

	if (consoleArgs.paths.size() == 1)
	{
		createUsingDefaultDirectory(consoleArgs.paths[0], consoleArgs.consoleSettings);
	}
	else
	{
		vector<string> filenames = gatherFilesForArchive(consoleArgs.paths);
		createArchiveFile(archiveFilename, filenames, consoleArgs.consoleSettings);
	}
}

int main(int argc, char **argv)
{
	try
	{
		ConsoleArgs consoleArgs = sortArguments(argc, argv);

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
		}
	}
	catch (exception e) {
		cerr << e.what() << endl;
		cerr << "Run without arguments to see usage message." << endl << endl;

#if defined _DEBUG
		getchar();
#endif

		return 1;
	}

#if defined _DEBUG
	getchar();
#endif

	return 0;
}

// OP2Archive add archiveName.[vol|clm] filename...
//  * Adds the specified file to the archive.

// OP2Archive remove archiveName.[vol|clm] filename...
//  * Removes the specified file from the archive.

//.vol .bmp .map .prt .raw .txt .wav .rtf
//.clm .wav

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