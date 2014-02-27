//==========================================================
//	Otrebor4
//----------------------------------------------------------
//contain assembler functions;
//save code lines, parse instruction, 
//
//function containKey: check if key exist on existing map
//function intToHexString: convert int to hexadecimal string representation
//function HexStringToInt: covert hexadecimal string literal to it's int value
//function intStringToInt: convert string to int
//function isNum: check if a character is a number
//class assembler:
//	assembler functions:
//	constructor: load mnemonic table, save file name to be used as input and outputs.
//	function pass1: load instruction from "<filename>.asm" file and save the output to "<filename>.pss"
//
//==========================================================


#ifndef ASSEMBLER_H_
#define ASSEMBLER_H_

#include <cstdlib>
#include <vector>
#include <iostream>
#include <sstream>
#include <fstream>
#include <map>
#include "StringLib.h"


using std::string;
using std::vector;
using std::istream;
using std::ifstream;
using std::map;

namespace ASM
{

	template<typename T>
	bool containKey(map<string,T> hash, string key)
	{

		for(typename map<string, T>::iterator iter = hash.begin();
			iter != hash.end();
			++iter)
		{
			if( str::equal(iter->first, key))
				return true;
		}
		//return hash[key] != NULL;
		return false;
	}

	string intToHexString(int hex)
	{
		string out;
		std::stringstream ss;
		ss << std::hex << hex;
		ss >> out;
		return str::toUperCase(out);
	}

	bool HexStringToInt(string hex, int & out)
	{
		hex = str::toUperCase(hex);
		for(unsigned int i = 0; i < hex.length(); i++)
		{
			if( !(hex[i] >= '0' && hex[i] <= '9') && !(hex[i] >= 'A' && hex[i] <= 'F'))
				return false;
		}
		std::stringstream ss;
		ss << std::hex << hex;
		ss >> out;
		return true;
	}

	bool intStringToInt(string sint, int & out)
	{
		for(unsigned int i = 0; i < sint.length(); i++)
		{
			if( !(sint[i] >= '0' && sint[i] <= '9'))
				return false;
		}
		out = std::atoi(sint.c_str());
		return true;
	}

	bool isNum(char c)
	{
		return (c >= '0' && c <= '9');
	}

#ifdef _MSC_VER
	typedef std::pair<int,int> Val;
	typedef std::pair<string, Val> Data;
#else
	typedef typename std::pair<int,int> Val;
	typedef typename std::pair<string, Val> Data;
#endif

	class assembler
	{
	private:
		string filename;

		vector<string> errorLog;
		vector<vector<string> > lines;

		map<string, Data> Mnemonic;
		map<string, string> symbolTable;

        void inFile(std::ifstream  &out)
		{
			out.open( (filename + ".asm").c_str() );
		}

		void midFile(std::ofstream & out)
		{
            out.open((filename + ".pss").c_str());
		}

		void outFile(std::ofstream & out)
		{
            out.open( (filename + ".obj").c_str() );
		}



		void loadMnemonic()
		{
			Mnemonic["ADD"]		= Data("18", Val(3 , 1) );
			Mnemonic["AND"]		= Data("58", Val(3 , 1) );
			Mnemonic["COMP"]	= Data("28", Val(3 , 1) );
			Mnemonic["DIV"]		= Data("24", Val(3 , 1) );
			Mnemonic["J"]		= Data("3C", Val(3 , 1) );
			Mnemonic["JEQ"]		= Data("30", Val(3 , 1) );
			Mnemonic["JGT"]		= Data("34", Val(3 , 1) );
			Mnemonic["JLT"]		= Data("38", Val(3 , 1) );
			Mnemonic["JSUB"]	= Data("48", Val(3 , 1) );
			Mnemonic["LDA"]		= Data("00", Val(3 , 1) );
			Mnemonic["LDCH"]	= Data("50", Val(3 , 1) );
			Mnemonic["LDL"]		= Data("08", Val(3 , 1) );
			Mnemonic["LDX"]		= Data("04", Val(3 , 1) );
			Mnemonic["MUL"]		= Data("20", Val(3 , 1) );
			Mnemonic["OR"]		= Data("44", Val(3 , 1) );
			Mnemonic["RD"]		= Data("D8", Val(3 , 1) );
			Mnemonic["RSUB"]	= Data("4C", Val(3 , 0) ); //???? use 3 bites no operator
			Mnemonic["STA"]		= Data("0C", Val(3 , 1) );
			Mnemonic["STCH"]	= Data("54", Val(3 , 1) );
			Mnemonic["STL"]		= Data("14", Val(3 , 1) );
			Mnemonic["STX"]		= Data("10", Val(3 , 1) );
			Mnemonic["SUB"]		= Data("1C", Val(3 , 1) );
			Mnemonic["TD"]		= Data("E0", Val(3 , 1) );
			Mnemonic["TIX"]		= Data("2C", Val(3 , 1) );
			Mnemonic["WD"]		= Data("DC", Val(3 , 1) );

			Mnemonic["BYTE"]	= Data("00", Val(3 , 1) );
			Mnemonic["WORD"]	= Data("00", Val(3 , 1) );
			Mnemonic["RESB"]	= Data("00", Val(-1 , 1) ); //dinamic allocador
			Mnemonic["RESW"]	= Data("00", Val(-3 , 1) ); //dinamic allocador

			Mnemonic["START"]	= Data("00", Val(0 , 1) );
			Mnemonic["END"]	= Data("00", Val(0 , 1) );
		}

		string getOpcode(string mnemonic)
		{
			return isMnemonic(mnemonic) ? Mnemonic[mnemonic].first : 0;
		}

		int getOperandSize(string mnemonic)
		{
			return isMnemonic(mnemonic) ? Mnemonic[mnemonic].second.second : 0;
		}

		int getOffset(string mnemonic)
		{
			return isMnemonic(mnemonic) ? Mnemonic[mnemonic].second.first : 0;
		}

		int calculateCodeSize(vector<string> line, int lnum)
		{
			int offset = getOffset(getMnemonic(line));
			if(offset < 0)
			{
				int val;
				if(intStringToInt(getOperand(line),val))
				{
					offset = (-offset)*val;
				}else{
					logError(lnum, "INVALID_OPERAND_VALUE");
				}
			}
			return offset;
		}

		bool balidLabel(string label)
		{
			return !label.empty() && !isNum(label[0]) ? !containKey<string>(symbolTable,label): false;
		}

		bool isMnemonic(string mNemonic)
		{
			return containKey<Data>(Mnemonic,mNemonic);
		}

		bool isComment(string line)
		{
			return line.size() > 0 ? line[0] == '.' : false;
		}

		vector<string> parseCode(string sin)
		{
			sin =  str::replace(sin,'\t',' ');
			vector<string> crt;
			crt.push_back("");//Label
			crt.push_back("");//mNemonic
			crt.push_back("");//arg
			unsigned int type = 0;//
			char lc = '#'; //last character
			unsigned int args = 3; //default label mnemonic operand {ignore comments}
			for(unsigned int i = 0; i < sin.length(); i++)
			{
				if(sin[i] == ' ' && sin[i] != lc){ //first space character hit add token to vector
					lc = sin[i];
					type++; //increase index;
					if(type == 2)//mnemonic loaded
					{
						args = 2 + getOperandSize(crt[1]);
					}
					if(type >= args){//comment start no needed
						break;
					}
				}else if(sin[i] != ' '){
					lc = sin[i];
					crt[type] += sin[i];
				}
			}
			for(unsigned int i = 0; i < crt.size(); i++)
				crt[i] = str::toUperCase(crt[i]);
			return crt;
		}

		string formatLine(vector<string> line)
		{
			string out = "";
			unsigned int l[]={10,7,12,6,4,1,1};
			for(unsigned int i = 0; i < line.size(); i++)
			{
				unsigned int c = 0;
				for( ; c < line[i].size(); c++)
				{
					out += line[i][c];
				}
				for(; c < l[i]; c++)
				{
					out += ' ';
				}
			}
			return out;
		}

		string getLabel(vector<string> line)
		{
			return line.size() >= 1 ? line[0] : "";
		}

		string getMnemonic(vector<string> line)
		{
			return line.size() >= 2 ? line[1] : "";
		}

		string getOperand(vector<string> line)
		{
			return line.size() >= 3 ? line[2] : "";
		}

		void logError(int line, string description)
		{
			string s = "Error on line: ";
			s += str::to_string(line);
			s += " <> ";
			s += description;
			std::cout << s<<"\n";
			errorLog.push_back(s);
		}

		void registerLabel(string label, int hex)
		{
			symbolTable[label] = intToHexString(hex);
		}

		bool validCode(vector<string> &line, int lnum)
		{
			bool error = false;
			//check if required operand
			bool operands = getOperandSize(getMnemonic(line)) > 0;
			if(operands)
			{
				if(getOperand(line).empty()){
					logError(lnum, "MISSING_OPERAND");
					error = true;
				}
				//check if operands is valid
			}else if(!getOperand(line).empty()){
				logError(lnum, "UNEDED_OPERAND: "+getOperand(line));
				error = true;
			}
			return !error;
		}

		void checkLabel(vector<string> &code, int memPos, int lnum)
		{
			string label = getLabel(code);
			if(balidLabel(label))
				registerLabel(label, memPos);
			else if(!label.empty()){
				if(isNum(label[0]))
					logError(lnum, "INVALID_LABEL: " + label);
				else
					logError(lnum, "DUPLICATE_LABEL: " + label);
			}
		}

		void saveObjCode()
		{
			std::ofstream objFile;
            outFile(objFile);
			for(unsigned int i = 0; i < lines.size(); i++)
			{
				vector<string> line = lines[i];
				if(line.size() > 0)
				{
					string codePos = line[line.size()-1];
					objFile << codePos << "\t";
					for(unsigned int j = 0; j < line.size()-1; j++)
					{
						objFile << line[j] << "\t";
					}
					objFile << "\n";
				}
			}
			objFile << "==============================================================\n";
			for(map<string, string>::iterator  iter = symbolTable.begin();
				iter != symbolTable.end();
				++iter)
			{
				objFile << iter->first << " - " << iter->second << "\n";
			}
			objFile.close();
			//objFile << EOF;
		}

		string getSysboltableString()
		{
			string out = "\n=========================SYMBOLTABLE=========================\n";
			for(map<string, string>::iterator  iter = symbolTable.begin();
				iter != symbolTable.end();
				++iter)
			{
				out += iter->first + " - " + iter->second + "\n";
			}
			return out;
		}

		void savePass1()
		{
			std::ofstream out;
			midFile(out);
			for(unsigned int i = 0; i < lines.size(); i++)
			{
				out << formatLine(lines[i]) << "\n";
			}
			out << getSysboltableString();
			out.close();
		}


	public:
		assembler(string _filename)
		{
			filename = _filename;
			loadMnemonic();
		}

		void pass1()
		{
			std::ifstream codeFile;
			inFile(codeFile);
			int lnum = 0;
			int memPos = 0;
			bool end = false;
			bool start = false;
			while(!codeFile.eof() && &end)
			{
				string line = str::getLine(&codeFile);
				if(!isComment(line))
				{
					vector<string> code = parseCode(line);
					if(str::equal(getMnemonic(code), "START"))
					{
						if(start)
							logError(lnum, "DUPLICATE_START_LABEL");
						else if(getOperand(code).empty())
							logError(lnum, "MISSING_OPERAND");
						else if(!HexStringToInt( getOperand(code), memPos))
							logError(lnum, "UNRECOGNIZE_HEX_VALUE");
						else{
							checkLabel( code, memPos, lnum);
						}
						code.push_back( intToHexString(memPos) );//add memory location
						start = true;
					}else if(str::equal(getMnemonic(code), "END")){
						if(getOperand(code).empty())
							logError(lnum,"MISSING_OPERAND");
						checkLabel(code,memPos, lnum);
						end = true;
						code.push_back( intToHexString(memPos) );//add memory location
					}else if(isMnemonic(getMnemonic(code))){
						if(validCode(code,lnum))//check if have correct num of arguments
						{
							checkLabel(code, memPos, lnum);
						}
						code.push_back( intToHexString(memPos) );//add memory location
						code.push_back( getOpcode(getMnemonic(code)));
						memPos += calculateCodeSize(code, lnum);
					}else{
						logError(lnum,"UNRECOGNIZED_MNEMONIC: "+getMnemonic(code));
						checkLabel( code, memPos, lnum);
					}
					lines.push_back(code);
				}
				lnum++;//increase fisical line
			}
			codeFile.close();
			savePass1();
		}
	};
};




#endif /* ASSEMBLER_H_ */
