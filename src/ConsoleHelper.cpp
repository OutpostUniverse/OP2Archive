#include "ConsoleHelper.h"
#include "ArchiveConsoleListing.h"
#include <stdexcept>

using namespace std;
using namespace OP2Utility;
using namespace OP2Utility::Archive;

const string ConsoleHelper::dashedLine = "--------------------------------------------------";

bool ConsoleHelper::IsArchiveExtension(const string& filename)
{
	return XFile::ExtensionMatches(filename, "VOL") || XFile::ExtensionMatches(filename, "CLM");
}

void ConsoleHelper::CheckIfPathsEmpty(const vector<string>& paths)
{
	if (paths.size() == 0) {
		throw invalid_argument("You must provide at least one file or directory. To provide the current directory, enter './'.");
	}
}

unique_ptr<ArchiveFile> ConsoleHelper::OpenArchive(const string& archivePath)
{
	if (XFile::ExtensionMatches(archivePath, "VOL")) {
		return make_unique<VolFile>(archivePath);
	}

	if (XFile::ExtensionMatches(archivePath, "CLM")) {
		return make_unique<ClmFile>(archivePath);
	}

	throw invalid_argument("Provided filename is not an archive file (.VOL/.CLM)");
}

vector<string> ConsoleHelper::GetArchiveFilenames(const string& directory)
{
	auto archiveFilenames = XFile::DirFilesWithExtension(directory, ".vol");
	auto clmFilenames = XFile::DirFilesWithExtension(directory, ".clm");

	archiveFilenames.insert(std::end(archiveFilenames), std::begin(clmFilenames), std::end(clmFilenames));

	return archiveFilenames;
}

string ConsoleHelper::CreateTempDirectory()
{
	srand(static_cast<int>(time(NULL))); // For creating a unique directory.
	int number = rand();

	string directory("./OP2ArchiveTemp-" + to_string(number));

	XFile::NewDirectory(directory);

	return directory;
}

void ConsoleHelper::ListContentsOfArchive(const string& archiveFilename)
{
	ArchiveConsoleListing listing;
	unique_ptr<ArchiveFile> archive = ConsoleHelper::OpenArchive(archiveFilename);
	listing.ListContents(*archive);
}
