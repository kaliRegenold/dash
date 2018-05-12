/*
* Author: Kali Regenold
* Date last edited: 2018-02-16
*/

#include <algorithm>
#include <dirent.h>
#include <fstream>
#include <iostream>
#include <string.h>
#include <vector>

/*
Splits the command line arguments on spaces.

param: string line - Line to be split
param: char delim - Delimiter to split the line on
return: vector<string> - Vector of string to hold command arguments
*/
std::vector<std::string> split(std::string line, char delim)
{
	std::vector<std::string> list;
	int size = line.length();
	std::string temp = "";

	for(int i = 0; i < size; i++)
	{
		if(line[i] != delim)
		{
			temp += line[i];
		}
		else
		{
			list.push_back(temp);
			temp = "";
		}
	}
	list.push_back(temp);
	return list;
}


/*
Print out the command name of a given PID number.
Check for validity by trying to convert the argument from string to long (strtol).
If successful, the char pointer passed into the strtol function will be 0.
Using ifstream, open the file matching the PID number /proc/<PID>/comm.

param: vector<string> &args - Vector of strings to hold the arguments given in the shell
return: void
*/
void cmdnm( std::vector<std::string> &args )
{
	// checks for correct number of args
	if(args.size() != 2)
	{
		std::cout << "cmdnm: incorrect number of arguments: cmdnm <pid>" << std::endl; 
		return;
	}

	// checks arg is numerical
	char *p;
	strtol(args[1].c_str(), &p, 10);
	if(*p != 0)
	{
		std::cout << "cmdnm: argument not a number: cmdnm <pid>" <<std::endl;
		return;
	}

	std::string file_name = "/proc/" + args[1] + "/comm";
	std::ifstream proc_file;
	std::string command_name = "";

	// checks for valid PID
	proc_file.open(file_name);
	if(!proc_file.is_open())
	{
		std::cout << "cmdnm: invalid pid" << std::endl;
	}
	else
	{
		proc_file >> command_name;
		std::cout << command_name << std::endl;
	}

	proc_file.close();

}


/*
Using the DIR objects from dirent.h, cycle through all directories matching the 
files provided by the file_list parameter: /proc/PID.

Original Credit: detorresrc http://www.cplusplus.com/forum/beginner/9173/
vector<string> &file_list - Vector of strings to hold all PID numbers of current
	processes (and therefore current open-able directories).
return: void
*/
void get_pid_nums(std::vector<std::string> &file_list){
    DIR *pDIR;
    struct dirent *entry;
    char* p;

    if( (pDIR=opendir("/proc")) )
    {
        while((entry = readdir(pDIR)))
        {
            if( strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0 )
            {
            	// checks for numerical directory names
				strtol(entry->d_name, &p, 10);
				if (!(*p))
				{
				    file_list.push_back(entry->d_name);
				}
            }
        }
        closedir(pDIR);
    }
}


/*
Cycle through all PID directories (given by get_pid_nums) and retrieves the string in the comm file.
Compare comm string to target string, provided by args.
Print out all PIDs with a matching comm string.

param: vectorstring &args - Vector of strings to hold the arguments given in the shell.
return: void
*/
void pid( std::vector<std::string> &args )
{
	// checks for correct number of args
	if(args.size() != 2)
	{
		std::cout << "pid: incorrect number of arguments: pid <command name>" << std::endl; 
		return;
	}

	std::vector<std::string> file_list;
	get_pid_nums(file_list);
	int size = file_list.size();

	std::ifstream file;
	std::string file_name;
	std::string comm_name;
	std::string target = args[1];

	for(int i = 0; i < size; i++)
	{
		file_name = "/proc/" + file_list[i] + "/comm";
		file.open(file_name);
		if(file.is_open())
		{
			getline(file, comm_name);
			if(comm_name == target)
				std::cout << file_list[i] << std::endl;
		}
		file.close();
	}

}


/*
Dump contents of a file between given line numbers.
If no line numbers given, dump the whole file.

param: string file_name - Name of the file to be read from.
param: string start_line - Line to start dumping from, inclusive.
param: string start_line - Line to stop dumping from, exclusive.
return: void
*/
void dump_file(std::string file_name, int start_line=-1, int end_line=-1)
{
	std::ifstream file;
	std::string line;
	int count = 0;
	bool print_flag = false;

	// checks if the entire file should be dumped, or just certain lines
	if(start_line == -1 && end_line == -1)
	{
		print_flag = true;
	}

	file.open(file_name);
	if(!file.is_open())
	{
		file.close();
		std::cout << "cannot open " << file_name << std::endl;
		return;
	}

	while( getline(file, line) )
	{
		// fun logic
		if( print_flag || (count >= start_line && count < end_line) )
			std::cout << line << std::endl;
		count++;
	}

	file.close();

}


/*
Print all file contents of version, uptime, meminfo in /proc.
Print lines from vendor_id (1) to cache size (8) of cpuinfo in /proc.

return: void
*/
void systat()
{
	std::cout << std::endl << "version:" << std::endl;
	dump_file("/proc/version");
	std::cout << std::endl << "uptime:" << std::endl;
	dump_file("/proc/uptime");
	std::cout << std::endl << "meminfo:" << std::endl;
	dump_file("/proc/meminfo");
	std::cout << std::endl << "cpuinfo:" << std::endl;
	dump_file("/proc/cpuinfo", 1, 9);
}

/*
Using all_of algorithm, return true if command consists only of whitespace.
This helps handle errors from receiving no commands.

param: string cmd - The command from the shell.
return: void
*/
bool is_whitespace( std::string cmd )
{
	return std::all_of(cmd.begin(),cmd.end(),isspace);
}


/*
Main loop for shell.
Receives commands and sends them to their respective functions.
Handles whitespace commands and nonexistent commands.

return: void
*/
void parse()
{
	bool exit_flag = false;
	char command[100];
	std::string cmd;
	std::vector<std::string> args;

	while( exit_flag == false )
	{
		std::cout << "dash>";
		std::cin.getline(command, sizeof(command));
		cmd = std::string(command);

		if( !is_whitespace(cmd) )
		{
			args = split(cmd, ' ');
			
			if( args[0] == "cmdnm" )
			{
				  cmdnm(args);
			}
			else if( args[0] == "pid" )
			{
				pid(args);
			}
			else if( args[0] == "systat" )
			{
				systat();
			}
			else if( args[0] == "exit" )
			{
				exit_flag = true;
			}
			else
			{
				std::cout << "dash: command not found: " << command << std::endl;
			}
		}
	}
}


/*
Calls parse.

return: int - Always return 0
*/
int main()
{
	parse();
	return 0;
}