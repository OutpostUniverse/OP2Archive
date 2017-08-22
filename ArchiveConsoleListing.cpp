#include "ArchiveConsoleListing.h"
#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>

void ArchiveConsoleListing::listContents(ArchiveFile* archiveFile)
{
	if (archiveFile->GetNumberOfPackedFiles() == 0)
	{
		cout << archiveFile->GetVolumeFileName() << " is empty." << endl;
		return;
	}

	int filenameColumnSize = findMaxFilenameSize(archiveFile);

	cout << "Contents of " << XFile::getFilename(archiveFile->GetVolumeFileName()) << ", " << archiveFile->GetNumberOfPackedFiles() << " file(s)." << endl << endl;
	cout << "ID  " << "NAME" << string(filenameColumnSize - 4, ' ') << "  SIZE (Bytes)" << endl;
	cout << "--------------------------------------------------" << endl;

	size_t maxCharsInFileSize;
	vector<string>* fileSizes = formatFileSizes(archiveFile, maxCharsInFileSize);

	for (int i = 0; i < archiveFile->GetNumberOfPackedFiles(); ++i)
	{
		string filename(archiveFile->GetInternalFileName(i));

		string filenameBlanks = CreateBlankChars(filename.size(), filenameColumnSize);

		string sizeBlanks;
		if (filename.size() <= maxFilenameSize)
			sizeBlanks = CreateBlankChars(fileSizes->at(i).size(), maxCharsInFileSize);

		cout.imbue(locale(""));
		
		cout << setw(2) << setfill('0') << i << "  " << filename << filenameBlanks << "  " << sizeBlanks << fileSizes->at(i) << endl;
	}

	cout << endl;

	delete fileSizes;
}

int ArchiveConsoleListing::findMaxFilenameSize(ArchiveFile* archiveFile)
{
	size_t largestFilenameSize = 0;

	for (int i = 0; i < archiveFile->GetNumberOfPackedFiles(); ++i)
	{
		size_t filenameSize = string(archiveFile->GetInternalFileName(i)).size();
		if (filenameSize > largestFilenameSize && filenameSize <= maxFilenameSize)
			largestFilenameSize = filenameSize;
	}

	return largestFilenameSize;
}

vector<string>* ArchiveConsoleListing::formatFileSizes(ArchiveFile* archiveFile, size_t& maxCharsInSize)
{
	maxCharsInSize = 0;
	vector<string>* fileSizes = new vector<string>();

	stringstream buffer;
	buffer.imbue(locale(""));

	for (size_t i = 0; i < (size_t)archiveFile->GetNumberOfPackedFiles(); ++i)
	{
		buffer << archiveFile->GetInternalFileSize(i);

		fileSizes->push_back(buffer.str());

		if (fileSizes->at(i).size() > maxCharsInSize)
			maxCharsInSize = fileSizes->at(i).size();

		buffer.str(string());
	}

	return fileSizes;
}

string ArchiveConsoleListing::CreateBlankChars(size_t stringSize, int columnSize)
{
	size_t numberOfBlankChars = 0;
	if (stringSize <= maxFilenameSize)
		numberOfBlankChars = columnSize - stringSize;

	return string(numberOfBlankChars, ' ');
}