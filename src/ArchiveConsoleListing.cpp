#include "ArchiveConsoleListing.h"
#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>

using namespace std;
using namespace Archives;

void ArchiveConsoleListing::ListContents(ArchiveFile& archiveFile)
{
	string filename = XFile::GetFilename(archiveFile.GetVolumeFilename());

	if (archiveFile.GetNumberOfPackedFiles() == 0) {
		cout << filename << " is empty." << endl << endl;
		return;
	}

	int filenameColumnSize = FindMaxFilenameSize(archiveFile);

	cout << "Contents of " << filename << ", " << archiveFile.GetNumberOfPackedFiles() << " file(s)." << endl << endl;
	cout << "ID  " << "NAME" << string(filenameColumnSize - 4, ' ') << "  SIZE (Bytes)" << endl;
	cout << "--------------------------------------------------" << endl;

	std::size_t maxCharsInFileSize;
	unique_ptr<vector<string>> fileSizes = FormatFileSizes(archiveFile, maxCharsInFileSize);

	for (int i = 0; i < archiveFile.GetNumberOfPackedFiles(); ++i)
	{
		string filename(archiveFile.GetInternalFilename(i));

		string filenameBlanks = CreateBlankChars(filename.size(), filenameColumnSize);

		string sizeBlanks;
		if (filename.size() <= maxFilenameSize) {
			sizeBlanks = CreateBlankChars(fileSizes->at(i).size(), maxCharsInFileSize);
		}

		cout.imbue(locale(""));
		
		cout << setw(2) << setfill('0') << i << "  " << filename << filenameBlanks << "  " << sizeBlanks << fileSizes->at(i) << endl;
	}

	cout << endl;
}

int ArchiveConsoleListing::FindMaxFilenameSize(ArchiveFile& archiveFile)
{
	std::size_t largestFilenameSize = 0;

	for (int i = 0; i < archiveFile.GetNumberOfPackedFiles(); ++i)
	{
		std::size_t filenameSize = string(archiveFile.GetInternalFilename(i)).size();
		if (filenameSize > largestFilenameSize && filenameSize <= maxFilenameSize) {
			largestFilenameSize = filenameSize;
		}
	}

	return largestFilenameSize;
}

unique_ptr<vector<string>> ArchiveConsoleListing::FormatFileSizes(ArchiveFile& archiveFile, std::size_t& maxCharsInSize)
{
	maxCharsInSize = 0;
	unique_ptr<vector<string>> fileSizes = make_unique<vector<string>>();

	stringstream buffer;
	buffer.imbue(locale(""));

	for (std::size_t i = 0; i < static_cast<std::size_t>(archiveFile.GetNumberOfPackedFiles()); ++i)
	{
		buffer << archiveFile.GetInternalFileSize(i);

		fileSizes->push_back(buffer.str());

		if (fileSizes->at(i).size() > maxCharsInSize) {
			maxCharsInSize = fileSizes->at(i).size();
		}

		buffer.str(string());
	}

	return fileSizes;
}

string ArchiveConsoleListing::CreateBlankChars(std::size_t stringSize, int columnSize)
{
	std::size_t numberOfBlankChars = 0;
	if (stringSize <= maxFilenameSize) {
		numberOfBlankChars = columnSize - stringSize;
	}

	return string(numberOfBlankChars, ' ');
}