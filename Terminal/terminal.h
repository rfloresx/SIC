//==========================================================
//	Otrebor4
//	
//----------------------------------------------------------
//main terminal logic
//	class commands: data container where the availables commands are store
//		variable cmds: map of available commands
//		variable cmdh: map of info from available commands
//		function AddCommad: add command to the data map, and store the function to execute when the command is call
//		function Run: execute a command if is register on the data
//		function Help: help command executable, requireds data map.
//
//	class terminal: terminal user interface main object
//		variable cmds: commands container
//		function init: map commands to its executable functions
//		function printInfo: Display terminal main page
//		function main: Main terminal log, get user input and call commands.
//==========================================================

#ifndef COMMANDS_H
#define COMMANDS_H

//execution status
#define RUN 1
#define EXIT 0
#define ERROR -1
#define NOFOUND -2

#include <iostream>
#include <cstdlib>
#include <map>
#include "StringLib.h"


using std::map;
using std::vector;
using std::string;

class commands;
//function delegate to define commands functions
typedef int (*Handler)(commands * _this, int argc, char ** args);

class commands
{
private:
	map<string,Handler> cmds;
	map<string,string> cmdh;

	//get all available commands
	vector<string> GetCommands()
	{
		vector<string> commands;
		for(map<string, string>::iterator  iter = cmdh.begin(); 
                                        iter != cmdh.end(); 
                                        ++iter)
		{
			commands.push_back(iter->first);
		}
		return commands;
	}

	//get correct key
	string GetKey(string skey)
	{
		string key = str::toLowerCase(skey);
		//keylist keys = cmds.getKeys();
		for(map<string, string>::iterator  iter = cmdh.begin(); 
                                        iter != cmdh.end(); 
                                        ++iter)
		{
			std::string s =iter->first;
			if( str::match( skey, s, 3 ) )
			{
				key = s;
			}
		}
		return key;
	}

	//display command info
	void display(string key)
	{
		string info = cmdh[key];
		if(!info.empty())
			std::cout << info.c_str() << "\n";
		else
			std::cout << key.c_str() << " no found\n";
	}


public:
	//add map command to a function and command info
	void AddCommand(string cmdName, Handler cmd, string info)
	{
		cmdName = str::toLowerCase(cmdName);
		cmds[cmdName] = cmd;
		cmdh[cmdName] = info;
	}

	//try to executa a command
	int Run(string cmd)
	{
		string s = str::trim(cmd);
		s = str::merge(s,' ');
		str::strings tokens = str::split(s, ' '); 
		char ** args = str::stringsToCharArray(tokens);
		Handler exe = cmds[GetKey(tokens[0])];
		if(exe != NULL)
			return (exe)(this, tokens.size(), args);
		return NOFOUND;
	}

	//help function
	int Help(int argc, char**args)
	{
		//execution is correct
		if(argc >= 1)
		{
			//check if have more arguments
			if(argc >= 2)
			{
				for(int i = 1; i < argc; i++)
				{
					display( GetKey(args[i]) );
				}
			}else{ //no arguments display all commands info
				vector<string> keys = GetCommands();
				for(unsigned int i =0; i < keys.size(); i++)
				{
					display(keys[i]);
				}
			}
		}
		return RUN;
	}

	//delegate function to be maped to the help command
	static int SHelp(commands * _this, int argc, char**args)
	{
		return _this->Help(argc, args);
	}
	
	//add default help command
	commands()
	{
		AddCommand("help", SHelp, "help <command>"); 
	}
	
};
//command to exit terminal
int ExitCmd	(commands * _this, int argc, char ** args){ return EXIT;}
//delegated to be maped on unimplemented command
int Empty	(commands * _this, int argc, char ** args){ std::cout << "command no implemented\n"; return RUN;}
//show directory command
int DirCmd (commands * _this, int argc, char ** args)
{
	std::string arg = "ls";
    if(argc > 1)
    {
        for(int i = 0; i < argc; i++)
		{
			arg += " ";
			arg += args[i];
		}
    }
    system(arg.c_str() );
	return RUN;
}

//main terminal
class Terminal
{
private:
	commands cmds;

	//map commands to functions and info
	void init()
	{
		cmds.AddCommand("exit", ExitCmd, "Exit terminal");
		cmds.AddCommand("load", Empty, "load <fileName>"); 
		cmds.AddCommand("execute", Empty, "execute <prog>");
		cmds.AddCommand("debug", Empty, "debug");
		cmds.AddCommand("dump", Empty, "dump"); 
		cmds.AddCommand("assemble", Empty, "assemble");
		cmds.AddCommand("directory", DirCmd, "directory");
	}
	//show terminal info
	void printInfo()
	{
	    std::cout << "Terminal Me [Version 0.0.2013]\n";
		std::cout << "(c) 2013 Roberto Flores.\n";
	}

	//get user input
	string userInput()
	{
		std::cout <<"~$ ";
		return str::getLine(&std::cin);
	}

public:
	Terminal()
	{
		init();
	}

	//terminal main logic
	int main(int argc, const char * args [])
	{
		printInfo();
		int state = RUN;
		while(state == RUN)
		{
			string input = userInput();
			state = cmds.Run(input);
			if(state  == NOFOUND){
				state = RUN;
				std::cout << input.c_str() << ": command not found\n";
			}
		}
		return state;
	}
};

#endif
