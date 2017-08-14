#include "ArchiveConsoleListing.h"
#include <iostream>
#include <iomanip>
#include <string>

void ArchiveConsoleListing::listContents(ArchiveFile* archiveFile)
{
	if (archiveFile->GetNumberOfPackedFiles() == 0)
	{
		cout << archiveFile->GetVolumeFileName() << " is empty." << endl;
		return;
	}

	int filenameColumnSize = findMaxFilenameSize(archiveFile);

	cout << "Contents of " << XFile::getFilename(archiveFile->GetVolumeFileName()) << " (" << archiveFile->GetNumberOfPackedFiles() << " files)" << endl;
	cout << "ID  " << "NAME" << string(filenameColumnSize - 4, ' ') << "  SIZE (KB)" << endl;
	cout << "--------------------------------------------------" << endl;

	for (int i = 0; i < archiveFile->GetNumberOfPackedFiles(); ++i)
	{
		string filename(archiveFile->GetInternalFileName(i));

		int numberOfBlankChars = 0;
		if (filename.size() <= maxFilenameSize)
			numberOfBlankChars = filenameColumnSize - filename.size();

		string blanks(numberOfBlankChars, ' ');
		int sizeKb = archiveFile->GetInternalFileSize(i) / 1024;
		cout.imbue(locale(""));

		cout << setw(2) << setfill('0') << i << "  " << filename << blanks << "  " << sizeKb << endl;
	}

	cout << endl;
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