#pragma once

#include "ConsoleSettings.h"
#include "OP2Utility.h"
#include <string>
#include <vector>
#include <functional>

class ConsoleArgumentParser
{
public:
	ConsoleArgumentParser();
	ConsoleArgs SortArguments(int argc, char **argv);

private:
	// Represents an argument (switch) passed into the console application from the command line.
	struct ConsoleSwitch
	{
		ConsoleSwitch() { }

		ConsoleSwitch(const std::string& shortSwitch, const std::string& longSwitch, std::function<void(const char* value, ConsoleArgs&)> parseFunction, int numberOfArgs) : 
			shortSwitch(shortSwitch), longSwitch(longSwitch), parseFunction(parseFunction), numberOfArgs(numberOfArgs) { }

		std::string shortSwitch;
		std::string longSwitch;
		std::function<void(const char*, ConsoleArgs&)> parseFunction;
		int numberOfArgs; // The switch statement itself does not count as an argument.

		bool ArgumentMatch(const std::string& argument) const
		{
			return argument == shortSwitch || argument == longSwitch;
		}
	};

	std::vector<ConsoleSwitch> consoleSwitches;

	ConsoleCommand ParseCommand(const std::string& commandStr);
	static bool IsTooFewArguments(int argumentCount);
	void ParseArgument(char** argv, int& i, int argc, ConsoleArgs& consoleArgs);

	void CheckForMissingSwitchArgument(int index, int argc, int numberOfArgsToPass);
	bool FindSwitch(char* argumentChar, ConsoleSwitch& currentSwitch);

	static void ParseHelp(const char* value, ConsoleArgs& consoleArgs);
	static void ParseQuiet(const char* value, ConsoleArgs& consoleArgs);
	static void ParseDestDirectory(const char* value, ConsoleArgs& consoleArgs);
	static void ParseOverwrite(const char* value, ConsoleArgs& consoleArgs);
	static void ParseCompressionFormat(const char* value, ConsoleArgs& consoleArgs);

	static Archive::CompressionType ParseCompression(const std::string& compressionStr);
	static bool ParseBool(const std::string& str);
};
