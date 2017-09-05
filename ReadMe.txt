OP2Archive - Outpost 2 Archive Access and Maintenance
Developed by Hooman and Brett208 (Vagabond)

Allows examining, creating, and extracting files from Outpost 2 .vol and .clm archives.

+++ COMMANDS +++
	* OP2Archive LIST (archivename.(vol|clm) | directory)...
	* OP2Archive FIND filename...
	* OP2Archive CREATE archivename.(vol|clm) [filename | directory]... [-q] [-o] [-c None|LZH]
	  * If no filename(s) or directory(s) provided,
        archives all contents of the default source directory (./archiveFilename).
    * OP2Archive EXTRACT archivename.(vol|clm) [filename]... [-q] [-d destDirectory] [-o]
      * If no filename(s) provided, extracts the entire vol file.
      * Defaults destination for entire vol file is ./archiveFilename.
	* OP2Archive ADD archiveName.[vol|clm] filename...
	* OP2Archive REMOVE archiveName.[vol|clm] filename...

+++ OPTIONAL ARGUMENTS +++
    -H / --Help / -?: Displays help information." << endl;
	-Q / --Quiet: [Default false] Prevents application from issuing console messages." << endl;
	-O / --Overwrite: [Default false] Allows application to overwrite existing files." << endl;
	-D / --DestinationDirectory: [Default for single file is './', Default for all files is archive's filename]. " << endl;
	                             Sets the destination directory for extracted file(s)." << endl;
	-S / --SourceDirectory: CREATE: [Deafault is archive's filename]. Sets the source directory when creating an archive." << endl;
	-C / --Compression: [Default None]. Sets the compression alghorithim used when creating an archive (None|LZH)." << endl;

For more information about Outpost 2 visit the Outpost Universe (http://outpost2.net/).


+ + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + +


+++ INSTALL DIRECTIONS +++

Unzip OP2Archive.exe using WINZIP compatible decompression algorithm. The typical place to put OP2Archive.exe is in the root OP2 install directory for ease of use, but this is not required.

Open a command prompt. Navigate to the directory containing OP2Archive and type 'OP2Archive -h' to see Usage Message.


+++ ARCHIVE (.VOL/.CLM) FILE CREATION BEST PRACTICES +++


+++ CLM Files +++

Outpost 2 stores all music tracks in op2.clm.

If you wish to change out the sound tracks in Outpost 2, you must preserve the audio format of .

Recommend starting with the free program Audacity to manipulate the original audio files. 

Internal CLM filenames are clipped down to 8 characters. //.clm .wav

+++ RELEASE COMPILATION INSTRUCTIONS +++

Source code may be found at: https://svn.outpostuniverse.org:8443/!/#outpost2/view/head/GameResources/OP2MapImager.

Post Build Event Notes: When in release mode, a command prompt script will run once the source code is compiled. This script automates staging all required files for distribution. Included in the source code is 7za.exe, the 7-zip command line tool that facilitates zipping the final release package. See http://www.7-zip.org/ for source files and https://www.dotnetperls.com/7-zip-examples for examples of use.

The Post Build Event requires a SVN console interface. I recommend the TortoiseSVN command prompt. NOTE: The command prompt is not installed by default and must be manually selected during the install process of TortoiseSVN.

 1. If changing Major/Minor revision number, set new version number at top of OP2MapImager main.cpp AND in post build event batch script.
 2. Run SVN Commit and then SVN Update to merge committed changes and properly update revision number of repository.
 3. Set Solution Configuration to Release.
 4. Set Solution Platform to x86. (x64 is not currently supported due to some WINAPI code within OP2Utility VolDecompress).
 5. Compile Code. 
 6. The following files will automatically be copied into the zipped directory'OP2MapImager 1.0.XXXX' (XXXX is svn revision number): 
    * OP2MapImager.exe (From Release Directory) 
	* FreeImage.dll (x32 version)
	* Well0000.BMP-Well0012.BMP (Reformated BMPs that a normal image editor may open.)
	* ReadMe.txt (this file)
	* FreeImage liscense-gplv3.txt
 7. Place zip file on the Outpost Universe Website.


+++ Change Log +++

Ver 1.0.0 (XXSep2017)
 * Initial Release
