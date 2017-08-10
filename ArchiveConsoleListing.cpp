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

	int maxFilenameSize = findMaxFilenameSize(archiveFile);

	cout << "Contents of " << XFile::getFilename(archiveFile->GetVolumeFileName()) << " (" << archiveFile->GetNumberOfPackedFiles() << " files)" << endl;
	cout << "ID  " << "NAME" << string(maxFilenameSize - 4, ' ') << "  SIZE (KB)" << endl;
	cout << "--------------------------------------------------" << endl;

	for (int i = 0; i < archiveFile->GetNumberOfPackedFiles(); ++i)
	{
		string filename(archiveFile->GetInternalFileName(i));
		string blanks(maxFilenameSize - filename.size(), ' ');
		int sizeKb = archiveFile->GetInternalFileSize(i) / 1024;
		cout.imbue(locale(""));

		cout << setw(2) << setfill('0') << i << "  " << filename << blanks << "  " << sizeKb << endl;
	}

	cout << endl;
}

int ArchiveConsoleListing::findMaxFilenameSize(ArchiveFile* archiveFile)
{
	int maxFilenameSize = 0;

	for (int i = 0; i < archiveFile->GetNumberOfPackedFiles(); ++i)
	{
		int filenameSize = string(archiveFile->GetInternalFileName(i)).size();
		if (filenameSize > maxFilenameSize)
			maxFilenameSize = filenameSize;
	}

	return maxFilenameSize;
}