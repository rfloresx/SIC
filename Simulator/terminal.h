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
#define C_FAIL 2
#define C_RUN 1
#define C_EXIT 0
#define C_ERROR -1
#define C_NOFOUND -2

#include <iostream>
#include <cstdlib>
#include <map>
#include "StringLib.h"
#include "sicengine.h"
#include "utils.h"
#include "assembler.h"


using std::map;
using std::vector;
using std::string;

class commands;
//function delegate to define commands functions
typedef int (*Handler)(commands * _this, int argc, char ** args);
bool HexStringToByte(std::string hex, BYTE & b){
		if(hex.size() > 2)
			return false;
		hex = str::toUperCase(hex);
		for(unsigned int i = 0; i < hex.length(); i++)
		{
			if( !(hex[i] >= '0' && hex[i] <= '9') && !(hex[i] >= 'A' && hex[i] <= 'F'))
				return false;
		}
		unsigned int v = 0;
		for(unsigned int i = 0; i < hex.size(); i++){
			int t = 0;
			ASM::HexCharToInt(hex[i],t);
			v += 16*v+t;
		}
		b = BYTE(v);
		return true;
	}

bool HexStringToAddress(std::string hex, ADDRESS & address){
	hex = str::toUperCase(hex);
	for(unsigned int i = 0; i < hex.length(); i++)
	{
		if( !(hex[i] >= '0' && hex[i] <= '9') && !(hex[i] >= 'A' && hex[i] <= 'F'))
			return false;
	}
	std::stringstream ss;
	ss << std::hex << hex;
	ss >> address;
	return true;
}

void SICDump(ADDRESS start, int size)
{
	if(start < MSIZE){
		if(start+size > MSIZE)
			size = MSIZE - start;
		std::stringstream ss;
		ADDRESS cur = start;
		int offset = 0;
		for(int i = 0; i < size; i++){
			if(offset== 0)//pring address
				printf("%05X ",cur);
				//std::cout << std::hex << cur << " ";

			printf(" %02X", Memory[cur+offset]);
			//std::cout << std::hex << Memory[cur+offset] << " ";

			offset++;
			if(offset%16 == 0){
				offset = 0;
				cur = start + i;
				std::cout << "\n";
			}
		}
	}
	std::cout << "\n";

}

void SICDump(std::string addrs, int size){
	ADDRESS start;
	if(HexStringToAddress(addrs,start)){
		SICDump(start,size);
		return;
	}
	std::cout << "INVALID_ADDRESS\n";
}

void SICLoad(string fileName){
	std::ifstream f;
	if(fileName.find(".o") == -1){
		fileName.append(".o");
	}
	f.open(fileName.c_str());
	if(f.is_open()){
		while(!f.eof()){
			string line = ASM::getLine( f);
			if(!line.empty()){
				if(line[0] == 'H'){
					vector<string> tokens = str::split(line,' ');
					if(tokens.size() == 2){
						string pns = tokens[1];
						string start = pns.substr(0,6);
						int pc = 0;
						if(!ASM::HexStringToInt(start,pc)){
							pc = 0;
							std::cout << "ERROR_INVALID_START_POSITION\n";
						}
						PC = pc;
					}
				}else if(line[0] == 'T'){
					//T|000000|00
					int addres = 0;
					if(line.size() >= 9){
						string s = line.substr(1,6);
						if(!ASM::HexStringToInt(s,addres)){
							addres = 0;
							std::cout <<"ERROR_TEXT_RECORD_INVALID_ADDRESS\n";
						}
					}
					for(int i = 9; i+1 < line.size(); i+=2){
						string s = "FF";
						s[0] = line[i];
						s[1] = line[i+1];
						unsigned int c = -1;
						if(!ASM::HexStringToInt(s,c))
							c = -1;
						BYTE * d = new BYTE(c);
						PutMem(addres,d, 0);
						addres++;
					}
				}else if(line[0] == 'E'){
					return;
				}
			}
		}
	}
}


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
		return C_NOFOUND;
	}

	void RunHelp(string cmd){
		string s = str::trim(cmd);
		s = str::merge(s,' ');
		str::strings tokens = str::split(s, ' ');
		char ** args = str::stringsToCharArray(tokens);
		Handler exe = cmds[GetKey("help")];
		if(exe != NULL)
			(exe)(this, tokens.size(), args);
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
		return C_RUN;
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
int ExitCmd	(commands * _this, int argc, char ** args){ return C_EXIT;}

//delegated to be maped on unimplemented command
int Empty	(commands * _this, int argc, char ** args){ std::cout << "command no implemented\n"; return C_RUN;}

//show directory command
int DirCmd (commands * _this, int argc, char ** args)
{
	std::string arg = "";
    if(argc > 1)
    {
        for(int i = 0; i < argc; i++)
		{
			arg += " ";
			arg += args[i];
		}
    }
	System::Directory(arg);
    //system(arg.c_str() );
	return C_RUN;
}

//assembly asm file
int AsmCmd(commands * _this, int argc, char ** args){
	if(argc >= 1){
		if(argc == 2){
			ASM::assembler a(args[1]);
			a.assembly();
			return C_RUN;
		}
	}
	return C_FAIL;
}

int LoadCmd(commands * _this, int argc, char ** args){
	if(argc >= 1){
		if(argc == 2){
			string file(args[1]);
			SICLoad(file);
		}
	}
	return C_RUN;
}

int ExeCmd(commands * _this, int argc, char ** args){
	if(argc >= 1){
		SICRun('\0');
	}
	return C_RUN;
}

int DumpCmd(commands * _this, int argc, char ** args){
	if(argc >= 1){
		ADDRESS addr = GetPC();
		int size = 16;
		if(argc >= 2){
			if(!HexStringToAddress(args[1],addr)){
				std::cout << "INVALID_ADDRESS\n";
				return C_FAIL;
			}
			if(argc >= 3){
				if(!ASM::intStringToInt(args[2], size)){
					std::cout << "INVALID_SIZE\n";
					return C_FAIL;
				}
			}
		}

		SICDump(addr,size);
	}

	return C_RUN;
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
		cmds.AddCommand("load", LoadCmd, "load <fileName>");
		cmds.AddCommand("execute", ExeCmd, "execute");
		cmds.AddCommand("debug", Empty, "debug");
		cmds.AddCommand("dump", DumpCmd, "dump");
		cmds.AddCommand("assemble", AsmCmd, "assemble <fileName>");
		cmds.AddCommand("directory", DirCmd, "directory");
		SICInit();
		//SICStart();
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
		int state = C_RUN;
		while(state == C_RUN)
		{
			string input = userInput();
			state = cmds.Run(input);
			if(state  == C_NOFOUND){
				state = C_RUN;
				std::cout << input.c_str() << ": command not found\n";
			}
			if(state == C_FAIL){
				state = C_RUN;
				cmds.RunHelp(input);
			}
		}
		return state;
	}
};

#endif
