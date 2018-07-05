#pragma once

#include "OP2Utility.h"
#include <vector>
#include <string>

struct ConsoleSettings
{
	std::string destDirectory = "./";
	bool overwrite = false;
	bool quiet = false;
	bool helpRequested = false;
	Archives::CompressionType compression = Archives::CompressionType::Uncompressed;
};

enum class ConsoleCommand
{
	List,
	Find,
	Extract,
	Create,
	Help,
	Add,
	Remove
};

struct ConsoleArgs
{
	ConsoleCommand consoleCommand = ConsoleCommand::Help;
	ConsoleSettings consoleSettings;
	std::vector<std::string> paths;
};
