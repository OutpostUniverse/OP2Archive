#include "ArchiveConsoleListing.h"
#include "ConsoleArgumentParser.h"
#include "ConsoleAdd.h"
#include "ConsoleCreate.h"
#include "ConsoleExtract.h"
#include "ConsoleList.h"
#include "ConsoleRemove.h"
#include "ConsoleHelper.h"
#include "ConsoleSettings.h"
#include "OP2Utility.h"
#include <string>
#include <vector>
#include <iostream>
#include <cstdlib>

using namespace std;
using namespace ConsoleArgumentParser;

void outputHelp();

static string version = "0.1.0";

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
	ConsoleHelper::checkIfPathsEmpty(consoleArgs);

	for (string path : consoleArgs.paths)
		locateFileInArchives(path);
}

void selectCommand(const ConsoleArgs& consoleArgs)
{
	switch (consoleArgs.consoleCommand)
	{
	case ConsoleCommand::Help:
		outputHelp();
		break;
	case ConsoleCommand::Create:
		ConsoleCreate consoleCreate;
		consoleCreate.createCommand(consoleArgs);
		break;
	case ConsoleCommand::Extract:
		ConsoleExtract consoleExtract;
		consoleExtract.extractCommand(consoleArgs);
		break;
	case ConsoleCommand::Find:
		locateCommand(consoleArgs);
		break;
	case ConsoleCommand::List: {
		ConsoleList consoleList;
		consoleList.listCommand(consoleArgs);
	}	break;
	case ConsoleCommand::Add: {
		ConsoleAdd consoleAdd;
		consoleAdd.addCommand(consoleArgs);
	}	break;
	case ConsoleCommand::Remove: {
		ConsoleRemove consoleRemove;
		consoleRemove.removeCommand(consoleArgs);
	}   break;
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