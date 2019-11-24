OP2Archive - Outpost 2 Archive Access and Maintenance
Developed by Hooman and Brett208 (Vagabond)

Allows examining, creating, and extracting files from Outpost 2 .vol and .clm archives. Check the OP2Archive GitHub release page for the newest version at https://github.com/OutpostUniverse/OP2Archive/releases.

OP2Archive is released under a MIT license. See License.txt for details.

## COMMANDS
  * OP2Archive LIST (archivename.(vol|clm) | directory)...
    * Lists the contents and size of all files contained in provided archives.

  * OP2Archive FIND filename...
    * Determines which archive contains the file. Stops searching at first instance.
    * All archives in the same directory as the provided filename are searched.

  * OP2Archive CREATE archivename.(vol|clm) [filename | directory]... [-q] [-o]
    * If no filename(s) or directory(s) provided, creates an empty archive.

  * OP2Archive EXTRACT archivename.(vol|clm) [filename]... [-q] [-d destDirectory] [-o]
    * If no filename(s) provided, extracts entire contents of archive.

  * OP2Archive EXTRACT directoryname [-q] [-d destDirectory] [-o].
    * Extracts entire contents of all archives in the provided directory.

  * OP2Archive ADD archiveName.[vol|clm] filename... [-q] [-o]

  * OP2Archive REMOVE archiveName.[vol|clm] filename... [-q] [-o]

## OPTIONAL ARGUMENTS
  -H / --Help / -?: Displays help information.
  -Q / --Quiet: [Default false] Prevents application from issuing console messages.
  -O / --Overwrite: [Default false] Allows application to overwrite existing files.
  -D / --DestinationDirectory: [Default is './']. Sets the destination directory for extracted file(s).

For more information about Outpost 2 visit the Outpost Universe (http://outpost2.net/).


----


## INSTALL DIRECTIONS

Unzip OP2Archive.exe using a WINZIP compatible decompression algorithm. The typical place to put OP2Archive.exe is in the root OP2 install directory for ease of use, but this is not required.

Open a command prompt. Navigate to the directory containing OP2Archive and type 'OP2Archive -h' to see Usage Message.


## ARCHIVE (.VOL/.CLM) FILE CREATION BEST PRACTICES

If you wish to overwrite a file currently in an archive file, use the ADD command and include the optional argument -O / --Overwrite to allow overwriting the original file.

The ADD and REMOVE command will create a new temp directory with the name ./OP2ArchiveTemp-(RANDOM INT) to store the contents of the archive file while rebuilding it. The ADD and REMOVE command will also eventually delete the original archive file before rebuilding it. If certain fatal exceptions occur during this process such as a power loss, the original archive file may be lost and/or the temp directory may not be deleted. If you are performing ADD and REMOVE commands on a heavily modified archive file, consider keeping a backup of the file somewhere. For a stock archive, if a fatal error in a critical step of the ADD and REMOVE commands, you can recover the archive file from the Outpost Universe website by redownloading the game.


## CLM FILES (AUDIO STORAGE)

Outpost 2 stores all music tracks except for the main menu track in the archive file op2.clm.

If you wish to change the sound tracks in Outpost 2, you must use the WAV file format with specific settings.

As a starting point for manipulating audio tracks for Outpost 2, consider the free program Audacity (http://www.audacityteam.org/). 

When naming audio files for storage in a CLM archive, the filename (excluding extension) must be 8 characters or less. The names of the audio tracks provided with the stock download of the game must be used. The game will then select the music tracks as it sees appropriate based on name.

Outpost 2 audio tracks must be formatted as WAV files with the following settings: 
 * Frequency = 22050 Hz
 * Channels = 1 (Mono)
 * BitsPerSample = 16

The file size of each WAV audio track (the actual wav data) must be zero padded to a multiple of 32768 bytes (0x8000). Output 2 fills the music buffer in 0x8000 size chunks and will not zero fill if a chunk at the end of a file is too short. If the file is not a multiple of 32768 bytes (0x8000), some garbage data will be played in Outpost 2 before switches to the next track.


## COMPRESSION

Outpost 2 contains references to 3 types of compression, RLE (Run - Length Encoded), LZ (Lempel - Ziv), and LZH (Lempel - Ziv, with adaptive Huffman encoding). 

Only LZH was used in the final release of Outpost 2. Only one archive file was compressed, sheets.vol. In subsequent releases of Outpost 2 by the Outpost Universe community, sheets.vol was decompressed and included in the game download in uncompressed format.

OP2Archive is capable of reading and decompressing archives using LZH compression. However, it currently cannot CREATE archives or modify via the ADD or REMOVE an archive using LZH compression.


## SOURCE CODE LOCATION AND COMPILIATION

Source code may be found at: https://github.com/OutpostUniverse/OP2Archive. See the file DeveloperReadMe.txt in the source code for specific instructions.


## CHANGE LOG

Ver 1.2.2
 * Bug Fix: Make the ADD and REMOVE command work again.
 * Bug Fix: Allow the CREATE command to work when adding the contents of a directory to a new volume.
 * Bug Fix: Prevent adding a volume to itself.
 * Bug Fix: Make error messages show filename when problems arise parsing files for inclusion in a CLM.

Ver 1.2.1
 * Bug Fix: Allow creating archives in new directories.
 * Bug Fix: Stop attempting to open directories that appear to have proper extensions (IE a directory named archive.vol).
 * Bug Fix: Stop attempting to package subdirectories into an archive (The subdirectory itself was being added, not the contents of the subdirectory).
 * Numerous other directory traversal fixes brought in from updated OP2Utility.

Ver 1.2.0
 * Provide both an x86 and x64 Windows build.
 * Remove Windows specific dependencies from code base (may now be compiled for use on Linux).
 * Remove .clm and .vol template files from application download (no longer required).
 * Bug Fix: Allow providing filenames out of alphabetical order when CREATE is used.
 * Bug Fix: Align the internal name sorting algorithm to align with the Outpost 2 binary search.
 * Bug Fix: Reject filenames that contain non-ASCII characters (prevents sort discrepancies with Outpost 2 binary search).
 * Generally improve error handling messaging.
 * Rewrite sections of the ReadMe.

Ver 1.1.1
 * Bug Fix: During CREATE command, sort all filenames case insensitive.

Ver 1.1.0
 * Bug Fix and Feature Change: Sort all filenames alphabetically during CREATE command.
 * Error Handling: Attempting to CREATE a repository that would contain 2 files with the same name results in an error message.
 * Error Handling: Attempting to EXTRACT a directory from an Archive results in an error message.
 * General code cleaning.

Ver 1.0.2
 * Fix bug where program crashes when attempting to extract a directory from a vol file.

Ver 1.0.1 (07Nov2017)
 * Added MIT License to project in file Licence.txt.

Ver 1.0.0 (07Oct2017)
 * Initial Release
