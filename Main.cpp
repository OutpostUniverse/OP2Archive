#include "ResourceManager.h"
#include <iostream>
#include <string>
#include <vector>
#include "OP2Utility.h"
#include "ArchiveConsoleListing.h"

using namespace std;

void outputHelp();

static string version = "0.1.0";

ArchiveConsoleListing archiveConsoleListing;

void listArchiveContent(const string& filename)
{
	if (XFile::extensionMatches(filename, ".vol"))
		archiveConsoleListing.listContents(new VolFile(filename.c_str()));
	else if (XFile::extensionMatches(filename, ".clm"))
		archiveConsoleListing.listContents(new ClmFile(filename.c_str()));
}

void listAllArchivesInDirectory()
{
	ResourceManager resourceManager("./");
	vector<string> volFilenames = resourceManager.getAllFilenamesOfType("./", ".vol", false);
	vector<string> clmFilenames = resourceManager.getAllFilenames("./", ".clm", false);

	cout << volFilenames.size() << " vol archive file(s) located." << endl;
	cout << clmFilenames.size() << " clm archive file(s) located." << endl;
	cout << "--------------------------------------------------" << endl << endl;

	for each (const string& filename in volFilenames)
		listArchiveContent(filename);

	for each(const string& filename in clmFilenames)
		listArchiveContent(filename);
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

int main(int argc, char **argv)
{
	listAllArchivesInDirectory();

	outputHelp();

	locateFileInArchives("eden04.map");

	locateFileInArchives("test.map");

	getchar();
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
	cout << "+++ USAGE NOTES +++" << endl;
	cout << "  * Capable of saving multiple map files and entire directories." << endl;
	cout << endl;
	cout << "+++ EXAMPLE COMMANDS +++" << endl;
	cout << "  * OP2Archive mapFilename.[map|OP2]" << endl;
	cout << "  * OP2MapImager -s 16 -o -q Ashes.map eden01.map sgame0.op2" << endl;
	cout << "  * OP2MapImager --Scale 8 --ImageFormat BMP [Directory of choice]" << endl;
	cout << endl;
	cout << "+++ OPTIONAL ARGUMENTS +++" << endl;
	cout << "  -H / --Help: Displays Help File" << endl;
	cout << "  -Q / --Quiet: [Default false] Add switch to run application without issuing console messages." << endl;
	cout << "  -O / --Overwrite: [Default false] Add switch to allow application to overwrite existing files." << endl;
	cout << "  -C / --Compression: [Default None]. Allows None|JPG|BMP. Sets the compression alghorithim used in archive. Only available to Create command." << endl;
	cout << endl;
	cout << "For more information about Outpost 2 visit the Outpost Universe (http://outpost2.net/)." << endl;
	cout << endl;
}