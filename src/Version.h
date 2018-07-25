#pragma once

#include <string>

// For information on how to define version information, see https://semver.org/.
namespace VersionInfo
{
	extern const int Major;
	extern const int Minor;
	extern const int Patch;

	std::string GetVersionString();
}
