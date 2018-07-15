cmake_minimum_required(VERSION 3.5.1 FATAL_ERROR)

project("OP2Archive")

add_subdirectory("OP2Utility")

# https://cmake.org/cmake/help/v3.12/command/file.html
# CMake documentation recommends not collecting all source files using a GLOB
file(GLOB SOURCE_CODE
	"src/ArchiveConsoleListing.cpp"
	"src/ArchiveConsoleListing.h"
	"src/ConsoleAdd.cpp"
	"src/ConsoleAdd.h"
	"src/ConsoleArgumentParser.cpp"
	"src/ConsoleArgumentParser.h"
	"src/ConsoleCreate.cpp"
	"src/ConsoleCreate.h"
	"src/ConsoleExtract.cpp"
	"src/ConsoleExtract.h"
	"src/ConsoleFind.cpp"
	"src/ConsoleFind.h"
	"src/ConsoleHelper.cpp"
	"src/ConsoleHelper.h"
	"src/ConsoleList.cpp"
	"src/ConsoleList.h"
	"src/ConsoleModifyBase.cpp"
	"src/ConsoleModifyBase.h"
	"src/ConsoleRemove.cpp"
	"src/ConsoleRemove.h"
	"src/ConsoleSettings.h"
	"src/Main.cpp")

add_executable(OP2Archive "${SOURCE_CODE}")

# Set Visual Studio to using static code generation in all default Cmake generated configurations
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT")
set(CMAKE_CXX_FLAGS_MINSIZEREL "${CMAKE_CXX_FLAGS_RELEASE} /MT")
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELEASE} /MT")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MTd")

target_link_libraries (OP2Archive OP2Utility)

# Add filters (virtual folders) in Visual Studio that correspond in name to actual directories
# https://cmake.org/cmake/help/v3.12/command/source_group.html
set(SRC_ROOT_PATH "${OP2Utility_SOURCE_DIR}/src")

foreach(source IN ITEMS ${SOURCE_CODE})
    get_filename_component(source_path "${source}" PATH)
    file(RELATIVE_PATH source_path_rel "${SRC_ROOT_PATH}" "${source_path}")
    string(REPLACE "/" "\\" group_path "${source_path_rel}")
    source_group("${group_path}" FILES "${source}")
endforeach()
