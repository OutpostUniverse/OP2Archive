OP2Archive Compilation Instructions
Developed by Hooman and Brett208 (Vagabond)

If you wish to edit and compile OP2Archive's source code, read this document first. See ReadMe.txt for a basic description of the project, change log, and instructions on use once compiled. OP2Archive is released under a MIT license. See License.txt for details.

Source code may be found at: https://github.com/OutpostUniverse/OP2Archive and https://github.com/OutpostUniverse/OP2Utility.

OP2Archive is written in C++ and compiles for Windows (MSVC) or Linux (Clang or gcc). It may be compiled in x86 or x64. The Windows project files are built for Visual Studio 2017. Makefiles are used for the Linux builds. The code is designed to be cross-platform, but has never been tested on Macintosh. C++14 features are used.

OP2Archive depends on the project OP2Utility. C++17 features are used and recommended, although C++14 with experimental filesystem can also be used. 

OP2Utility may be found at: https://github.com/OutpostUniverse/OP2Utility

+++ RELEASE COMPILATION INSTRUCTIONS +++

Post Build Event Notes: When in RELEASE mode, a command prompt script will run once the source code is compiled. If compiling for test purposes, compile in DEBUG mode to prevent the script from running. This script automates staging all required files for distribution. Included in the source code is 7za.exe, the 7-zip command line tool that facilitates zipping the final release package. This is the Windows version of 7 zip and is not compatible with Linux. See http://www.7-zip.org/ for source files and https://www.dotnetperls.com/7-zip-examples for examples of use.

 1. Update ReadMe.txt with changes and new version numer. See http://semver.org/ for versioning guidelines.
 2. Set new version number at top of OP2Archive main.cpp, in OP2Archive.rc, AND in post build event batch script.
 3. Tag current version in repository.
 4. Set Solution Configuration to Release.
 5. Set Solution Platform to x86.
 6. Compile Code. 
 7. The following files will automatically be copied into the zipped directory 'OP2Archive X.X.X PLATFORM': 
    * OP2Archive.exe (From Release Directory)
    * ReadMe.txt
    * License.txt
 8. Place zip file on the Outpost Universe GitHub repository.
 9. Repeat steps 5 through 8 for x64.