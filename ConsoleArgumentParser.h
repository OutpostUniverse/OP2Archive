#pragma once

#include "OP2Utility.h"
#include <string>
#include <vector>
#include <functional>

using namespace std;

namespace ConsoleArgumentParser
{
	struct ConsoleSettings
	{
		string destDirectory = "./";
		string sourceDirectory = "./";
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
		vector<string> paths;
	};

	struct ConsoleSwitch
	{
		ConsoleSwitch() { }

		ConsoleSwitch(string shortSwitch, string longSwitch, function<void(const char* value, ConsoleArgs&)> parseFunction, int numberOfArgs)
		{
			this->shortSwitch = shortSwitch;
			this->longSwitch = longSwitch;
			this->parseFunction = parseFunction;
			this->numberOfArgs = numberOfArgs;
		}

		string shortSwitch;
		string longSwitch;
		function<void(const char*, ConsoleArgs&)> parseFunction;
		int numberOfArgs; // The switch statement itself does not count as an argument.

		bool argumentMatch(string argument)
		{
			return argument == shortSwitch || argument == longSwitch;
		}
	};

	ConsoleArgs sortArguments(int argc, char **argv);

	ConsoleCommand parseCommand(const string& commandStr);
	Archives::CompressionType parseCompression(const string& compressionStr);
	void checkForMissingSwitchArgument(int index, int argc, int numberOfArgsToPass);
	bool findSwitch(char* argumentChar, ConsoleSwitch& currentSwitch);

	void parseQuiet(const char* value, ConsoleArgs& consoleArgs);
	void parseDestDirectory(const char* value, ConsoleArgs& consoleArgs);
	void parseHelp(const char* value, ConsoleArgs& consoleArgs);
	void parseOverwrite(const char* value, ConsoleArgs& consoleArgs);
	void parseCompressionFormat(const char* value, ConsoleArgs& consoleArgs);
	void parseSourceDirectory(const char* value, ConsoleArgs& consoleArgs);
}