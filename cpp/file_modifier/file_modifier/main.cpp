#include <iostream>
#include <fstream>
#include <string>

// Application error codes:
constexpr int CODE_SUCCESS = 0;

// Application commandline arguments
constexpr char ARG_COMMAND_HELP[] = "help";
constexpr char ARG_COMMAND_OPEN[] = "open";
constexpr char ARG_COMMAND_LOCK[] = "lock";
constexpr char ARG_COMMAND_MODIFY[] = "modify";
constexpr char ARG_COMMAND_DELETE[] = "delete";

// Other constants:
constexpr char TEXT[] = "this content was modified by file modifier utility";
constexpr char HELP[] =
"filemod [open|modify] <path>\n"
"open	Opens <path> for reading and writing and closes it afterwards.\n"
"modify	Opens <path> for reading and writing, resets the cursor, writes arbitrary text and closes it.\n";

void command_open(const std::string & path)
{
	std::fstream file;
	file.open(path, std::ios::in | std::ios::out);

	if (file.is_open() == false)
	{
		std::cerr << "command_open() - Failed to open file '" << path << "' for reading/writing" << std::endl;
		return;
	}

	file.close();
}

void command_lock(const std::string & path)
{
	std::cerr << "command_lock() - Function not implemented." << std::endl;
}

void command_modify(const std::string & path)
{
	std::fstream file;
	file.open(path, std::ios::in | std::ios::out);

	if (file.is_open() == false)
	{
		std::cerr << "command_modify() - Failed to open file '" << path << "' for reading/writing" << std::endl;
	}

	file << TEXT << std::flush;

	file.close();
}

void command_delete(const std::string & path)
{
	std::cerr << "command_delete() - Function not implemented." << std::endl;
}

void command_help()
{
	std::cout << HELP << std::endl;
}

int main(int argc, char ** argv)
{
	// Parse command line
	std::string executable;
	std::string command;
	std::string path;

	if (argc >= 1)
	{
		executable = argv[0];
	}

	if (argc >= 2)
	{
		command = argv[1];
	}

	if (argc >= 3)
	{
		path = argv[2];
	}

	// Handle command line:
	if (command == ARG_COMMAND_HELP)
	{
		command_help();
		return CODE_SUCCESS;
	}

	if (command == ARG_COMMAND_OPEN)
	{
		command_open(path);
		return CODE_SUCCESS;
	}

	if (command == ARG_COMMAND_LOCK)
	{
		command_lock(path);
		return CODE_SUCCESS;
	}

	if (command == ARG_COMMAND_MODIFY)
	{
		command_modify(path);
		return CODE_SUCCESS;
	}

	if (command == ARG_COMMAND_DELETE)
	{
		command_delete(path);
		return CODE_SUCCESS;
	}

	// Display help by default if no commands were received
	command_help();

	return CODE_SUCCESS;
}