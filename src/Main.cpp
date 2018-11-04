#include "ConsoleArgumentParser.h"
#include "ConsoleAdd.h"
#include "ConsoleCreate.h"
#include "ConsoleExtract.h"
#include "ConsoleArgumentParser.h"
#include "ConsoleList.h"
#include "ConsoleFind.h"
#include "ConsoleRemove.h"
#include "ConsoleSettings.h"
#include <string>
#include <iostream>

using namespace std;

int main(int argc, char **argv);
void SelectCommand(const ConsoleArgs& consoleArgs);
void OutputHelp();
void DebugPause();

static string version = "1.2.0";

int main(int argc, char **argv)
{
	try
	{
		ConsoleArgumentParser consoleArgumentParser;
		ConsoleArgs consoleArgs = consoleArgumentParser.SortArguments(argc, argv);
		SelectCommand(consoleArgs);
	}
	catch (const std::exception& e) {
		cerr << e.what() << endl;
		cerr << "Run without arguments to see usage message." << endl << endl;

		DebugPause();

		return 1;
	}

	DebugPause();

	return 0;
}

void SelectCommand(const ConsoleArgs& consoleArgs)
{
	switch (consoleArgs.consoleCommand)
	{
	case ConsoleCommand::Help: {
		OutputHelp();
	}break;
	case ConsoleCommand::Create: {
		ConsoleCreate consoleCreate;
		consoleCreate.CreateCommand(consoleArgs);
	}break;
	case ConsoleCommand::Extract: {
		ConsoleExtract consoleExtract;
		consoleExtract.ExtractCommand(consoleArgs);
	}break;
	case ConsoleCommand::Find: {
		ConsoleFind consoleFind;
		consoleFind.FindCommand(consoleArgs);
	}break;
	case ConsoleCommand::List: {
		ConsoleList consoleList;
		consoleList.ListCommand(consoleArgs);
	}break;
	case ConsoleCommand::Add: {
		ConsoleAdd consoleAdd;
		consoleAdd.AddCommand(consoleArgs);
	}break;
	case ConsoleCommand::Remove: {
		ConsoleRemove consoleRemove;
		consoleRemove.RemoveCommand(consoleArgs);
	}break;
	}
}

void OutputHelp()
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
	cout << "    * All archives in the same directory as the provided filename are searched." << endl;
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
	// Compression is currently not available when creating archives.
	// cout << "  -C / --Compression: [Default None]. Sets the compression alghorithim used when creating an archive (None|LZH)." << endl;
	cout << endl;
	cout << "For more information about Outpost 2 visit the Outpost Universe (http://outpost2.net/)." << endl;
	cout << endl;
}

void DebugPause()
{
#if defined _DEBUG
	getchar();
#endif
}
