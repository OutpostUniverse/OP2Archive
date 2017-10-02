OP2Archive Compilation Instructions
Developed by Hooman and Brett208 (Vagabond)

If you wish to edit and compile OP2Archive's source code, read this document first. See ReadMe.txt for a basic description of the project, change log, and instructions on use once compiled. 

Source code may be found at: https://github.com/OutpostUniverse/OP2Archive and https://github.com/OutpostUniverse/OP2Utility.

OP2Archive is written in C++ and the solution/project files are built for Visual Studio 2017. C++11 and C++14 features are used. The currently expiremental C++ standard library filesystem is used and is supported by both VS2017 and GCC. 

OP2Archive depends on the project OP2Utility. OP2Utility requires a Windows machine to compile due to use of windows.h. In particular, the archive read/write code depends on Windows.h. There is some desire to remove the Windows specific code, but the efforts are not yet completed. The project must be compiled for x86. 

OP2Utility may be found at: https://github.com/OutpostUniverse/OP2Utility

+++ RELEASE COMPILATION INSTRUCTIONS +++

Post Build Event Notes: When in RELEASE mode, a command prompt script will run once the source code is compiled. If compiling for test purposes, compile in DEBUG mode to prevent the script from running. This script automates staging all required files for distribution. Included in the source code is 7za.exe, the 7-zip command line tool that facilitates zipping the final release package. This is the Windows version of 7 zip and is not compatible with Linux. See http://www.7-zip.org/ for source files and https://www.dotnetperls.com/7-zip-examples for examples of use.

 1. If changing Major/Minor revision number, set new version number at top of OP2Archive main.cpp AND in post build event batch script.
 2. Run Commit and then push Updates to master repository.
 3. Set Solution Configuration to Release.
 4. Set Solution Platform to x86. (x64 is not currently supported due to some WINAPI code within OP2Utility VolDecompress).
 5. Compile Code. 
 6. The following files will automatically be copied into the zipped directory 'OP2Archive X.X.X': 
    * OP2Archive.exe (From Release Directory)
    * VolTemplate.vol
    * ClmTemplate.clm
    * ReadMe.txt (this file)
 7. Place zip file on the Outpost Universe Website.
