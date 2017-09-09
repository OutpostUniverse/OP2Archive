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
    -Q / --Quiet: [Default false] Prevents application from issuing console messages.
    -O / --Overwrite: [Default false] Allows application to overwrite existing files.
    -D / --DestinationDirectory: [Default for single file is './', Default for all files is archive's filename].
                                Sets the destination directory for extracted file(s).
    -S / --SourceDirectory: CREATE: [Deafault is archive's filename]. Sets the source directory when creating an archive.
    -C / --Compression: [Default None]. Sets the compression alghorithim used when creating an archive (None|LZH).

For more information about Outpost 2 visit the Outpost Universe (http://outpost2.net/).


+ + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + +


+++ INSTALL DIRECTIONS +++

Unzip OP2Archive.exe using WINZIP compatible decompression algorithm. The typical place to put OP2Archive.exe is in the root OP2 install directory for ease of use, but this is not required.

Open a command prompt. Navigate to the directory containing OP2Archive and type 'OP2Archive -h' to see Usage Message.


+++ ARCHIVE (.VOL/.CLM) FILE CREATION BEST PRACTICES +++

The ADD and REMOVE command will create a new temp directory with the name ./OP2ArchiveTemp-(RANDOM INT) to store the contents of the archive file while rebuilding it. The ADD and REMOVE command will also eventually delete the original archive file before rebuilding it. If certain fatal exceptions occur during this process such as a power loss, the original archive file may be lost and/or the temp directory may not be deleted. If you are performing ADD and REMOVE commands on a heavily modifyied archive file, consider keeping a backup of the file somewhere. In case of a fatal error in the critical steps for the ADD and REMOVE coomands, you should be able to just recover the archive file from the Outpost Universe website by redownloading the game.

If you wish to overwrite a file currently in an archive file, use the ADD command and include the optional argument -O / --Overwrite to allow overwriting the original file.


+++ CLM FILES (AUDIO STORAGE) +++

Outpost 2 stores all music tracks except for the track that plays on the main menu in the archive file op2.clm.

If you wish to change out the sound tracks in Outpost 2, you must use specific settings and use the WAV file format.

As a starting point for manipulating audio tracks for Outpost 2, consider starting with the free program Audacity (http://www.audacityteam.org/). 

When naming audio files for storage in a CLM archive, the filename will be clipped down to 8 characters. Without siginificant modifications to the Outpost 2 application, you must use the names of the audio tracks provided with the stock download of the game to get modified or new tracks to play in game. 


+++ COMPRESSION +++

Outpost 2 contains references to 3 types of compression, RLE (Run - Length Encoded), LZ (Lempel - Ziv), and LZH (Lempel - Ziv, with adaptive Huffman encoding). 

Only LZH was used in the final release of Outpost 2 to compress the file sheets.vol. In subsequent releases of Outpost 2, sheets.vol was decompressed and included in the game download in uncompressed format. No other archive file was compressed.

OP2Archive is capable of reading and decompressing archives using LZH compression. However, it currently cannot CREATE archives or modify via the ADD or REMOVE file an archive using LZH compression.


+++ RELEASE COMPILATION INSTRUCTIONS +++

Source code may be found at: https://github.com/OutpostUniverse/OP2Archive and https://github.com/OutpostUniverse/OP2Utility.

Post Build Event Notes: When in release mode, a command prompt script will run once the source code is compiled. This script automates staging all required files for distribution. Included in the source code is 7za.exe, the 7-zip command line tool that facilitates zipping the final release package. See http://www.7-zip.org/ for source files and https://www.dotnetperls.com/7-zip-examples for examples of use.

The Post Build Event requires a SVN console interface. I recommend the TortoiseSVN command prompt. NOTE: The command prompt is not installed by default and must be manually selected during the install process of TortoiseSVN.

 1. If changing Major/Minor revision number, set new version number at top of OP2Archive main.cpp AND in post build event batch script.
 2. Run Commit and then push Updates to master repository.
 3. Set Solution Configuration to Release.
 4. Set Solution Platform to x86. (x64 is not currently supported due to some WINAPI code within OP2Utility VolDecompress).
 5. Compile Code. 
 6. The following files will automatically be copied into the zipped directory'OP2MapImager 1.0.XXXX' (XXXX is svn revision number): 
    * OP2Archive.exe (From Release Directory)
	* VolTemplate.vol
	* ClmTemplate.clm
	* ReadMe.txt (this file)
 7. Place zip file on the Outpost Universe Website.


+++ Change Log +++

Ver 1.0.0 (XXSep2017)
 * Initial Release
