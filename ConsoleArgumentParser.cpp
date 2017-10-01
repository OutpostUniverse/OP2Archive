#include "ConsoleArgumentParser.h"
#include "StringHelper.h"

ConsoleArgumentParser::ConsoleArgumentParser() 
{
	consoleSwitches.push_back(ConsoleSwitch("-H", "--HELP", parseHelp, 0));
	consoleSwitches.push_back(ConsoleSwitch("-?", "--?", parseHelp, 0));
	consoleSwitches.push_back(ConsoleSwitch("-D", "--DESTINATIONDIRECTORY", parseDestDirectory, 1));
	consoleSwitches.push_back(ConsoleSwitch("-Q", "--QUIET", parseQuiet, 0));
	consoleSwitches.push_back(ConsoleSwitch("-O", "--OVERWRITE", parseOverwrite, 0));
	consoleSwitches.push_back(ConsoleSwitch("-C", "--COMPRESSION", parseCompressionFormat, 1));
}

bool ConsoleArgumentParser::findSwitch(char* argumentChar, ConsoleSwitch& currentSwitch)
{
	string argument = StringHelper::convertToUpper(argumentChar);

	// Remove trailing colon from switch statements.
	if (argument.size() > 0 && argument[argument.size() - 1] == ':')
		argument.pop_back();

	for (ConsoleSwitch consoleSwitch : consoleSwitches)
	{
		if (consoleSwitch.argumentMatch(argument))
		{
			currentSwitch = consoleSwitch;
			return true;
		}
	}

	return false;
}

ConsoleArgs ConsoleArgumentParser::sortArguments(int argc, char **argv)
{
	ConsoleArgs consoleArgs;

	if (checkTooFewArguments(argc))
	{
		consoleArgs.consoleCommand = ConsoleCommand::Help;
		return consoleArgs;
	}

	string commandStr = argv[1];
	consoleArgs.consoleCommand = parseCommand(commandStr);

	for (int i = 2; i < argc; ++i)
	{
		parseArgument(argv, i, argc, consoleArgs);
	}

	return consoleArgs;
}

ConsoleCommand ConsoleArgumentParser::parseCommand(const string& commandStr)
{
	string commandStrUpper = StringHelper::convertToUpper(commandStr);

	if (commandStrUpper == "LIST")
		return ConsoleCommand::List;

	if (commandStrUpper == "HELP" || commandStrUpper == "-?" || commandStrUpper == "-H")
		return ConsoleCommand::Help;

	if (commandStrUpper == "FIND")
		return ConsoleCommand::Find;

	if (commandStrUpper == "EXTRACT")
		return ConsoleCommand::Extract;

	if (commandStrUpper == "CREATE")
		return ConsoleCommand::Create;

	if (commandStrUpper == "ADD")
		return ConsoleCommand::Add;

	if (commandStrUpper == "REMOVE")
		return ConsoleCommand::Remove;

	throw invalid_argument("A valid command was not provided.");
}

void ConsoleArgumentParser::parseArgument(char** argv, int i, int argc, ConsoleArgs& consoleArgs)
{
	ConsoleSwitch currentSwitch;

	bool switchFound = findSwitch(argv[i], currentSwitch);

	if (switchFound)
	{
		checkForMissingSwitchArgument(i, argc, currentSwitch.numberOfArgs);

		if (currentSwitch.numberOfArgs == 0)
			currentSwitch.parseFunction(argv[i], consoleArgs);
		else
			currentSwitch.parseFunction(argv[i + 1], consoleArgs);

		i += currentSwitch.numberOfArgs;
	}
	else
	{
		consoleArgs.paths.push_back(argv[i]);
	}
}

CompressionType ConsoleArgumentParser::parseCompression(const string& compressionStr)
{
	string compressionStrUpper = StringHelper::convertToUpper(compressionStr);

	if (compressionStrUpper == "NONE" || compressionStrUpper == "UNCOMPRESSED")
		return Archives::CompressionType::Uncompressed;

	if (compressionStrUpper == "LZ")
		return Archives::CompressionType::LZ;

	if (compressionStrUpper == "LZH")
		return Archives::CompressionType::LZH;

	if (compressionStrUpper == "RLE")
		return Archives::CompressionType::RLE;

	throw invalid_argument("Unable to determine compression type. Try None, LZ, LZH, or RLE.");
}

bool ConsoleArgumentParser::parseBool(const string& str)
{
	string upperStr = StringHelper::convertToUpper(str);

	if (upperStr == "TRUE" || upperStr == "YES")
		return true;

	if (upperStr == "FALSE" || upperStr == "NO")
		return false;

	throw invalid_argument("Unable to parse argument into a valid boolean (True/False).");
}

void ConsoleArgumentParser::checkForMissingSwitchArgument(int index, int argc, int numberOfArgsToPass)
{
	if (index + numberOfArgsToPass >= argc)
		throw range_error("Missing the final argument for the supplied switch.");
}

void ConsoleArgumentParser::parseHelp(const char* value, ConsoleArgs& consoleArgs)
{
	consoleArgs.consoleSettings.helpRequested = true;
	consoleArgs.consoleCommand = ConsoleCommand::Help;
}

void ConsoleArgumentParser::parseQuiet(const char* value, ConsoleArgs& consoleArgs)
{
	consoleArgs.consoleSettings.quiet = true;
}

void ConsoleArgumentParser::parseOverwrite(const char* value, ConsoleArgs& consoleArgs)
{
	consoleArgs.consoleSettings.overwrite = true;
}

void ConsoleArgumentParser::parseDestDirectory(const char* value, ConsoleArgs& consoleArgs)
{
	consoleArgs.consoleSettings.destDirectory = value;
}

void ConsoleArgumentParser::parseCompressionFormat(const char* value, ConsoleArgs& consoleArgs)
{
	consoleArgs.consoleSettings.compression = parseCompression(value);
}

bool ConsoleArgumentParser::checkTooFewArguments(int numberOfArguments)
{
	return numberOfArguments < 2;
}
