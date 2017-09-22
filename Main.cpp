#include "ConsoleArgumentParser.h"
#include "ConsoleAdd.h"
#include "ConsoleCreate.h"
#include "ConsoleExtract.h"
#include "ConsoleList.h"
#include "ConsoleLocate.h"
#include "ConsoleRemove.h"
#include "ConsoleSettings.h"
#include "OP2Utility.h"
#include <string>
#include <vector>
#include <iostream>

using namespace std;
using namespace ConsoleArgumentParser;

int main(int argc, char **argv);
void selectCommand(const ConsoleArgs& consoleArgs);
void outputHelp();
void debugPause();

static string version = "0.1.0";

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

void selectCommand(const ConsoleArgs& consoleArgs)
{
	switch (consoleArgs.consoleCommand)
	{
	case ConsoleCommand::Help: {
		outputHelp();
	}break;
	case ConsoleCommand::Create: {
		ConsoleCreate consoleCreate;
		consoleCreate.createCommand(consoleArgs);
	}break;
	case ConsoleCommand::Extract: {
		ConsoleExtract consoleExtract;
		consoleExtract.extractCommand(consoleArgs);
	}break;
	case ConsoleCommand::Find: {
		ConsoleLocate consoleLocate;
		consoleLocate.locateCommand(consoleArgs);
	}break;
	case ConsoleCommand::List: {
		ConsoleList consoleList;
		consoleList.listCommand(consoleArgs);
	}break;
	case ConsoleCommand::Add: {
		ConsoleAdd consoleAdd;
		consoleAdd.addCommand(consoleArgs);
	}break;
	case ConsoleCommand::Remove: {
		ConsoleRemove consoleRemove;
		consoleRemove.removeCommand(consoleArgs);
	}break;
	}
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
	cout << "    * Lists the contents and size of all files contained in provided archives." << endl;
	cout << "  * OP2Archive FIND filename..." << endl;
	cout << "    * Determines which archive contains the file. Stops searching at first instance." << endl;
	cout << "    * Pulls archives from the directory appended to the provided filename to search." << endl;
	cout << "  * OP2Archive CREATE archivename.(vol|clm) [filename | directory]... [-q] [-o]" << endl; //If compression added [-c None|LZH]
	cout << "    * If no filename(s) or directory(s) provided," << endl;
	cout << "      archives all contents of the default source directory (./archiveFilename)." << endl;
	cout << "  * OP2Archive EXTRACT archivename.(vol|clm) [filename]... [-q] [-d destDirectory] [-o]" << endl;
	cout << "    * If no filename(s) provided, extracts entire contents of archive." << endl;
	cout << "  * OP2Archive EXTRACT directoryname [-q] [-d destDirectory] [-o]." << endl;
	cout << "    * Extracts entire contents of all archives in the provided directory." << endl;
	cout << "  * OP2Archive ADD archiveName.[vol|clm] filename... [-q] [-o]" << endl;
	cout << "  * OP2Archive REMOVE archiveName.[vol|clm] filename... [-q] [-o]" << endl;
	cout << endl;
	cout << "+++ OPTIONAL ARGUMENTS +++" << endl;
	cout << "  -H / --Help / -?: Displays help information." << endl;
	cout << "  -Q / --Quiet: [Default false] Prevents application from issuing console messages." << endl;
	cout << "  -O / --Overwrite: [Default false] Allows application to overwrite existing files." << endl;
	cout << "  -D / --DestinationDirectory: [Default is './']. Sets the destination directory for extracted file(s)." << endl;
	cout << "  -S / --SourceDirectory: [Deafault is archive's filename]. Sets the source directory when creating an archive." << endl;
	// Compression is currently not available when creating archives.
	// cout << "  -C / --Compression: [Default None]. Sets the compression alghorithim used when creating an archive (None|LZH)." << endl;
	cout << endl;
	cout << "For more information about Outpost 2 visit the Outpost Universe (http://outpost2.net/)." << endl;
	cout << endl;
}

void debugPause()
{
#if defined _DEBUG
	getchar();
#endif
}