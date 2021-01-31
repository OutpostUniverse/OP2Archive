#include "ConsoleCreate.h"
#include "ConsoleHelper.h"
#include "OP2Utility.h"
#include <iostream>
#include <stdexcept>

using namespace std;
using namespace OP2Utility;
using namespace OP2Utility::Archive;

void ConsoleCreate::CreateCommand(const ConsoleArgs& consoleArgs)
{
	ConsoleHelper::CheckIfPathsEmpty(consoleArgs.paths);

	string archiveFilename = consoleArgs.paths[0];

	if (!ConsoleHelper::IsArchiveExtension(archiveFilename)) {
		throw runtime_error("A .vol or .clm filename must be provided to create an archive.");
	}

	CheckCreateOverwrite(archiveFilename, consoleArgs.consoleSettings.overwrite, consoleArgs.consoleSettings.quiet);

	if (consoleArgs.paths.size() == 1)
	{
		CreateArchiveFile(archiveFilename, vector<string>(), consoleArgs.consoleSettings.quiet);
	}
	else {
		vector<string> filenames = GatherFilesForArchive(consoleArgs.paths);
		CreateArchiveFile(archiveFilename, filenames, consoleArgs.consoleSettings.quiet);
	}
}

void ConsoleCreate::CreateArchiveFile(const string& archiveFilename, const vector<string>& paths, bool quiet)
{
	CheckForIllegalFilenames(paths);

	if (!quiet) {
		OutputInitialCreateMessage(archiveFilename, paths.size());
	}

	if (XFile::ExtensionMatches(archiveFilename, "vol")) {
		VolFile::CreateArchive(archiveFilename, paths);
	}
	else if (XFile::ExtensionMatches(archiveFilename, "clm")) {
		ClmFile::CreateArchive(archiveFilename, paths);
	}
	else {
		throw std::runtime_error("Error attempting to create new archive. Extension must match vol or clm");
	}

	if (!quiet) {
		OutputCreateResults(paths.size(), archiveFilename);
	}
}

unique_ptr<ArchiveFile> ConsoleCreate::CreateArchiveTemplate(const string& archiveFilename)
{
	if (XFile::ExtensionMatches(archiveFilename, "VOL")) {
		return make_unique<VolFile>("VolTemplate.vol");
	}

	if (XFile::ExtensionMatches(archiveFilename, "CLM")) {
		return make_unique<ClmFile>("ClmTemplate.clm");
	}

	throw runtime_error("Unable to open archive template files VolTemplate.vol and/or ClmTemplate.clm. Ensure both files exist in same directory as OP2Archive.exe and are not open in another application.");
}

vector<string> ConsoleCreate::GatherFilesForArchive(const vector<string>& paths)
{
	vector<string> filenames;

	for (std::size_t i = 1; i < paths.size(); ++i) //Skip the first path since it is the archive name.
	{
		if (XFile::IsDirectory(paths[i]))
		{
			auto dirFilenames = XFile::DirFiles(paths[i]);

			for (auto& filename : dirFilenames) {
				filename = XFile::Append(paths[i], filename);
			}

			filenames.insert(std::end(filenames), std::begin(dirFilenames), std::end(dirFilenames));
		}
		else {
			filenames.push_back(paths[i]);
		}
	}

	return filenames;
}

void ConsoleCreate::CheckCreateOverwrite(const string& archiveFilename, bool overwrite, bool quiet)
{
	if (XFile::PathExists(archiveFilename))
	{
		if (!overwrite) {
			throw runtime_error("Archive file already exists at specified path. If overwrite is desired add argument -o.");
		}
		else if (overwrite && !quiet) {
			cout << "An archive file already exists at the specified path. Overwrite authorized by user." << endl << endl;
		}
	}
}

void ConsoleCreate::OutputInitialCreateMessage(const string& archiveFilename, std::size_t packedFileCount)
{
	cout << "Creating archive " << archiveFilename << ", containing " << packedFileCount << " file(s)." << endl;
	cout << ConsoleHelper::dashedLine << endl;
}

void ConsoleCreate::OutputCreateResults(std::size_t packedFileCount, const string& archiveFilename)
{
	cout << "Archive created." << endl << endl;

	if (packedFileCount < 1) {
		cout << "Caution: Created archive is empty (contains no files)." << endl;
	}
	else {
		ConsoleHelper::ListContentsOfArchive(archiveFilename);
	}
}

void ConsoleCreate::CheckForIllegalFilenames(const std::vector<std::string>& paths)
{
	for (const auto& path : paths)
	{
		auto filename = XFile::GetFilename(path);

		if (StringUtility::ContainsNonAsciiChars(filename)) {
			throw std::runtime_error("The following filename contains an illegal character and cannot be packed: " + filename + ".");
		}
	}
}
