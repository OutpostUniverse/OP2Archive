#include "ConsoleArgumentParser.h"
#include "StringHelper.h"
#include <stdexcept>

using namespace std;
using namespace Archives;

ConsoleArgumentParser::ConsoleArgumentParser() 
{
	consoleSwitches.push_back(ConsoleSwitch("-H", "--HELP", ParseHelp, 0));
	consoleSwitches.push_back(ConsoleSwitch("-?", "--?", ParseHelp, 0));
	consoleSwitches.push_back(ConsoleSwitch("-D", "--DESTINATIONDIRECTORY", ParseDestDirectory, 1));
	consoleSwitches.push_back(ConsoleSwitch("-Q", "--QUIET", ParseQuiet, 0));
	consoleSwitches.push_back(ConsoleSwitch("-O", "--OVERWRITE", ParseOverwrite, 0));
	consoleSwitches.push_back(ConsoleSwitch("-C", "--COMPRESSION", ParseCompressionFormat, 1));
}

bool ConsoleArgumentParser::FindSwitch(char* argumentChar, ConsoleSwitch& currentSwitch)
{
	string argument = StringHelper::ConvertToUpper(argumentChar);

	// Remove trailing colon from switch statements.
	if (argument.size() > 0 && argument[argument.size() - 1] == ':')
		argument.pop_back();

	for (const auto& consoleSwitch : consoleSwitches)
	{
		if (consoleSwitch.ArgumentMatch(argument)) {
			currentSwitch = consoleSwitch;
			return true;
		}
	}

	return false;
}

ConsoleArgs ConsoleArgumentParser::SortArguments(int argc, char **argv)
{
	ConsoleArgs consoleArgs;

	if (CheckTooFewArguments(argc)) {
		consoleArgs.consoleCommand = ConsoleCommand::Help;
		return consoleArgs;
	}

	string commandStr = argv[1];
	consoleArgs.consoleCommand = ParseCommand(commandStr);

	for (int i = 2; i < argc; ++i) {
		ParseArgument(argv, i, argc, consoleArgs);
	}

	return consoleArgs;
}

ConsoleCommand ConsoleArgumentParser::ParseCommand(const string& commandStr)
{
	string commandStrUpper = StringHelper::ConvertToUpper(commandStr);

	if (commandStrUpper == "LIST") {
		return ConsoleCommand::List;
	}
	if (commandStrUpper == "HELP" || commandStrUpper == "-?" || commandStrUpper == "-H") {
		return ConsoleCommand::Help;
	}
	if (commandStrUpper == "FIND") {
		return ConsoleCommand::Find;
	}
	if (commandStrUpper == "EXTRACT") {
		return ConsoleCommand::Extract;
	}
	if (commandStrUpper == "CREATE") {
		return ConsoleCommand::Create;
	}
	if (commandStrUpper == "ADD") {
		return ConsoleCommand::Add;
	}
	if (commandStrUpper == "REMOVE") {
		return ConsoleCommand::Remove;
	}

	throw invalid_argument("A valid command was not provided.");
}

void ConsoleArgumentParser::ParseArgument(char** argv, int& i, int argc, ConsoleArgs& consoleArgs)
{
	ConsoleSwitch currentSwitch;

	bool switchFound = FindSwitch(argv[i], currentSwitch);

	if (switchFound)
	{
		CheckForMissingSwitchArgument(i, argc, currentSwitch.numberOfArgs);

		if (currentSwitch.numberOfArgs == 0) {
			currentSwitch.parseFunction(argv[i], consoleArgs);
		}
		else {
			currentSwitch.parseFunction(argv[i + 1], consoleArgs);
		}

		i += currentSwitch.numberOfArgs;
	}
	else {
		consoleArgs.paths.push_back(argv[i]);
	}
}

CompressionType ConsoleArgumentParser::ParseCompression(const string& compressionStr)
{
	string compressionStrUpper = StringHelper::ConvertToUpper(compressionStr);

	if (compressionStrUpper == "NONE" || compressionStrUpper == "UNCOMPRESSED") {
		return Archives::CompressionType::Uncompressed;
	}
	if (compressionStrUpper == "LZ") {
		return Archives::CompressionType::LZ;
	}
	if (compressionStrUpper == "LZH") {
		return Archives::CompressionType::LZH;
	}
	if (compressionStrUpper == "RLE") {
		return Archives::CompressionType::RLE;
	}

	throw invalid_argument("Unable to determine compression type. Try None, LZ, LZH, or RLE.");
}

bool ConsoleArgumentParser::ParseBool(const string& str)
{
	string upperStr = StringHelper::ConvertToUpper(str);

	if (upperStr == "TRUE" || upperStr == "YES") {
		return true;
	}

	if (upperStr == "FALSE" || upperStr == "NO") {
		return false;
	}

	throw invalid_argument("Unable to parse argument into a valid boolean (True/False).");
}

void ConsoleArgumentParser::CheckForMissingSwitchArgument(int index, int argc, int numberOfArgsToPass)
{
	if (index + numberOfArgsToPass >= argc) {
		throw range_error("Missing the final argument for the supplied switch.");
	}
}

void ConsoleArgumentParser::ParseHelp(const char* value, ConsoleArgs& consoleArgs)
{
	consoleArgs.consoleSettings.helpRequested = true;
	consoleArgs.consoleCommand = ConsoleCommand::Help;
}

void ConsoleArgumentParser::ParseQuiet(const char* value, ConsoleArgs& consoleArgs)
{
	consoleArgs.consoleSettings.quiet = true;
}

void ConsoleArgumentParser::ParseOverwrite(const char* value, ConsoleArgs& consoleArgs)
{
	consoleArgs.consoleSettings.overwrite = true;
}

void ConsoleArgumentParser::ParseDestDirectory(const char* value, ConsoleArgs& consoleArgs)
{
	consoleArgs.consoleSettings.destDirectory = value;
}

void ConsoleArgumentParser::ParseCompressionFormat(const char* value, ConsoleArgs& consoleArgs)
{
	consoleArgs.consoleSettings.compression = ParseCompression(value);
}

bool ConsoleArgumentParser::CheckTooFewArguments(int numberOfArguments)
{
	return numberOfArguments < 2;
}
