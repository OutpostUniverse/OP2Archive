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
	cout << "--------------------------------------------------" << endl << endl;

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
	{
		locateFileInArchives(path);
	}
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

void extractFromAnyArchive(const ConsoleArgs& consoleArgs)
{
	ResourceManager resourceManager("./");

	for each (string path in consoleArgs.paths)
	{

	}
}

void extractFromSpecificArchive(const ConsoleArgs& consoleArgs)
{

}

void extractCommand(const ConsoleArgs& consoleArgs)
{
	if (consoleArgs.paths.size() == 0)
		throw exception("You must specify a filename to extract or the source archive file (.vol|.clm) to extract from.");

	if (isArchiveFileExtension(consoleArgs.paths[0]))
		extractFromSpecificArchive(consoleArgs);
	else
		extractFromAnyArchive(consoleArgs);
}

void createCommand(const ConsoleArgs& consoleArgs)
{
	checkIfPathsEmpty(consoleArgs);


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

// OP2Archive contents archiveName.[vol|clm]
//  * Lists the contents of an archive file.

// OP2Archive find filename
//  * Lists the archive filename that contains the specified file.

// OP2Archive create archiveName.[vol|clm] [filename...]
//  * Creates an archive with the given name. Adds the listed files to the archive.

// OP2Archive add archiveName.[vol|clm] filename...
//  * Adds the specified file to the archive.

// OP2Archive remove archiveName.[vol|clm] filename...
//  * Removes the specified file from the archive.

// OP2Archvie extract archiveName.[vol|clm]... [filename]...
//  * Extracts the specified file from the archive. If no volFilenames provided, extracts the entire archive.

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
	cout << "  * OP2Archive CREATE archivename.(vol|clm) [filename | directory]... [-q] [-s sourceDirectory] [-o] [-c None|LZH]" << endl;
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
	cout << "  -S / --SourceDirectory: EXTRACT: [Deafault is archive's filename]. Sets the source file directory when creating an archive." << endl;
	cout << "  -C / --Compression: [Default None]. Sets the compression alghorithim used when creating an archive (None|LZH)." << endl;
	cout << endl;
	cout << "For more information about Outpost 2 visit the Outpost Universe (http://outpost2.net/)." << endl;
	cout << endl;
}