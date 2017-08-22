#include "ConsoleArgumentParser.h"
#include "StringHelper.h"

namespace ConsoleArgumentParser
{
	ConsoleSwitch consoleSwitches[]
	{
		ConsoleSwitch("-H", "--HELP", parseHelp, 0),
		ConsoleSwitch("-?", "--?", parseHelp, 0),
		ConsoleSwitch("-D", "--DESTINATIONDIRECTORY", parseDestDirectory, 1),
		ConsoleSwitch("-Q", "--QUIET", parseQuiet, 0),
		ConsoleSwitch("-O", "--OVERWRITE", parseOverwrite, 0),
		ConsoleSwitch("-C", "--COMPRESSION", parseCompressionFormat, 1),
		ConsoleSwitch("-S", "--SOURCEDIRECTORY", parseSourceDirectory, 1)
	};

	bool findSwitch(char* argumentChar, ConsoleSwitch& currentSwitch)
	{
		string argument = StringHelper::convertToUpper(argumentChar);

		// Remove trailing colon from switch statements.
		if (argument.size() > 0 && argument[argument.size() - 1] == ':')
			argument.pop_back();

		for each (ConsoleSwitch consoleSwitch in consoleSwitches)
		{
			if (consoleSwitch.argumentMatch(argument))
			{
				currentSwitch = consoleSwitch;
				return true;
			}
		}

		return false;
	}

	ConsoleArgs sortArguments(int argc, char **argv)
	{
		ConsoleArgs consoleArgs;

		if (argc < 2)
		{
			consoleArgs.consoleCommand = ConsoleCommand::Help;
			return consoleArgs;
		}

		consoleArgs.consoleCommand = parseCommand(argv[1]);

		ConsoleSwitch currentSwitch;

		for (int i = 2; i < argc; ++i)
		{
			bool switchFound = findSwitch(argv[i], currentSwitch);

			if (switchFound)
			{
				checkForMissingSwitchArgument(i, argc, currentSwitch.numberOfArgs);

				if (currentSwitch.numberOfArgs == 0)
					currentSwitch.parseFunction(argv[i], consoleArgs);
				else
					currentSwitch.parseFunction(argv[i + 1], consoleArgs);

				i = i + currentSwitch.numberOfArgs;
			}
			else
			{
				consoleArgs.paths.push_back(argv[i]);
			}
		}

		return consoleArgs;
	}

	ConsoleCommand parseCommand(const string& commandStr)
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

		throw exception("A valid command was not provided.");
	}

	Archives::CompressionType parseCompression(const string& compressionStr)
	{
		string compressionStrUpper = StringHelper::convertToUpper(compressionStr);

		if (compressionStrUpper == "NONE" || compressionStrUpper == "UNCOMPRESSED")
			return Archives::CompressionType::Uncompressed;

		if (compressionStrUpper == "LZ")
			return Archives::CompressionType::LZ;

		if (compressionStrUpper == "LZH" )
			return Archives::CompressionType::LZH;

		if (compressionStrUpper == "RLE")
			return Archives::CompressionType::RLE;

		throw exception("Unable to determine compression type. Try None, LZ, LZH, or RLE.");
	}

	bool ParseBool(const string& str)
	{
		string upperStr = StringHelper::convertToUpper(str);

		if (upperStr == "TRUE" || upperStr == "YES")
			return true;

		if (upperStr == "FALSE" || upperStr == "NO")
			return false;

		throw invalid_argument("Unable to parse argument into a valid boolean (True/False).");
	}

	void checkForMissingSwitchArgument(int index, int argc, int numberOfArgsToPass)
	{
		if (index + numberOfArgsToPass >= argc)
			throw exception("Missing the final argument for the supplied switch.");
	}

	void parseHelp(const char* value, ConsoleArgs& consoleArgs)
	{
		consoleArgs.consoleSettings.helpRequested = true;
	}

	void parseQuiet(const char* value, ConsoleArgs& consoleArgs)
	{
		consoleArgs.consoleSettings.quiet = true;
	}

	void parseOverwrite(const char* value, ConsoleArgs& consoleArgs)
	{
		consoleArgs.consoleSettings.overwrite = true;
	}

	void parseDestDirectory(const char* value, ConsoleArgs& consoleArgs)
	{
		consoleArgs.consoleSettings.destDirectory = value;
	}

	void parseCompressionFormat(const char* value, ConsoleArgs& consoleArgs)
	{
		consoleArgs.consoleSettings.compression = parseCompression(value);
	}

	void parseSourceDirectory(const char* value, ConsoleArgs& consoleArgs)
	{
		consoleArgs.consoleSettings.sourceDirectory = value;
	}
}