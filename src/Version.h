#pragma once

#include <string>

// For information on how to define version information, see https://semver.org/.
namespace OP2ArchiveVersion
{
	extern const int Major;
	extern const int Minor;
	extern const int Patch;

	// Returns the version string formatted as Major.Minor.Patch
	std::string GetVersionString();
}
