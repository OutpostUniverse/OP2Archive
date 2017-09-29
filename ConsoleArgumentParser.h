#pragma once

#include "ConsoleSettings.h"
#include "OP2Utility.h"
#include <string>
#include <vector>
#include <functional>

using namespace std;
using namespace Archives;

class ConsoleArgumentParser
{
public:
	ConsoleArgumentParser();
	ConsoleArgs sortArguments(int argc, char **argv);

private:
	// Represents an argument (switch) passed into the console application from the command line.
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

	vector<ConsoleSwitch> consoleSwitches;

	ConsoleCommand parseCommand(const string& commandStr);
	bool checkTooFewArguments(int numberOfArguments);
	void parseArgument(char** argv, int i, int argc, ConsoleArgs& consoleArgs);

	void checkForMissingSwitchArgument(int index, int argc, int numberOfArgsToPass);
	bool findSwitch(char* argumentChar, ConsoleSwitch& currentSwitch);

	static void parseHelp(const char* value, ConsoleArgs& consoleArgs);
	static void parseQuiet(const char* value, ConsoleArgs& consoleArgs);
	static void parseDestDirectory(const char* value, ConsoleArgs& consoleArgs);
	static void parseOverwrite(const char* value, ConsoleArgs& consoleArgs);
	static void parseCompressionFormat(const char* value, ConsoleArgs& consoleArgs);
	static void parseSourceDirectory(const char* value, ConsoleArgs& consoleArgs);

	static CompressionType parseCompression(const string& compressionStr);
	bool parseBool(const string& str);
};