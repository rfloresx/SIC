//==========================================================
//	Otrebor4
//
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

	char intToHexChar(int hex)
	{
		char out;
		std::stringstream ss;
		ss << std::hex << hex;
		ss >> out;
		return str::toUperCase(out);
	}

	bool HexCharToInt(char hex, int & out)
	{
		if( !(hex >= '0' && hex <= '9') && !(hex >= 'A' && hex <= 'F'))
				return false;
		std::stringstream ss;
		ss << std::hex << hex;
		ss >> out;
		return true;
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

	bool HexStringToInt(string hex, unsigned int & out)
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

	bool CharsToHexVal(string sin, int & out)
	{
		string hex;
		for(unsigned int i = 0; i < sin.size(); i++)
		{
			hex += intToHexString( sin[i] );
		}
		return HexStringToInt(hex,out);
	}

	bool isNum(char c)
	{
		return (c >= '0' && c <= '9');
	}

	string getLine(istream  & in)
	{
		string out;
		std::getline(in,out);
		return out;
	}

	string expandHex(string hex, int size)
	{
		string out = "";
		int c = size - hex.size();
		for(int i = 0; i < c; i++)
		{
			out += '0';
		}
		for(unsigned int i = 0; i < hex.size(); i++)
		{
			out += hex[i];
		}
		return out;
	}
	string fixHex(string hex, unsigned int size)
	{
		string out = "";
		for(unsigned int i = 0; i  < hex.size()  && i < size; i++)
		{
			out += hex[i];
		}
		return out;
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
		class TexRecord{
		private:
			std::string addrs;
			std::string objCode;
			bool valid;

			int objCodeSize(){
				return objCode.size()/2;
			}
		public:
			void TextRecord(){
				addrs = "-";
				objCode = "";
				valid = false;
			}
			void TextRecord(string addr){
				addrs = addr;
				objCode = "";
				valid = true;
			}
			void clear(){
				addrs = "-";
				objCode = "";
				valid = false;
			}
			void start(string addr){
				addrs = addr;
				objCode = "";
				valid = true;
			}
			void addObjectCode(string code){
				objCode += code;
			}
			bool isFull(){
				return (objCodeSize() + 6 > 30);
			}
			bool isValid(){
				return valid;
			}
			std::string to_string(){
				std::string s = "T";
				s += expandHex( addrs,6);
				s += expandHex(intToHexString(objCodeSize()),2);
				s += objCode;
				return s;
			}
		};

		class LINE
		{
		private:
			string some;
		public:
			

			string line;		//line number
			//code
			string label;			//label
			string instrunction;	//instrunction
			string operand;			//operand

			string loc;			//memory position
			string opcode;			//instrunction opcode
			string objCode;		//object code

			vector<string> errorsLog;	//error log
			
			LINE(string s0, string s1, string s2, string s3, string s4, string s5)
			{
				line = s0;
				label = s1;
				instrunction= s2;
				operand= s3;
				loc= s4;
				opcode= s5;
				objCode = "";
				some = "";
			}
			
			void addErrors(vector<string> errs)
			{
				for(unsigned int i = 0; i < errs.size();i++)
				{
					errorsLog.push_back(errs[i]);
				}
			}

			void logError(string log)
			{
				errorsLog.push_back(log);
			}
			
			unsigned int size()
			{
				return 7 + errorsLog.size();
			}

			bool indexing()
			{
				vector<string> ops = str::split(operand, ',');
				return ops.size() == 2 && (ops[1] == "x" || ops[1] == "X");
			}

			string operan()
			{
				vector<string> ops = str::split(operand, ',');
				return ops.size() > 0 ? ops[0] : "";
			}

			string errors()
			{
				string errs = "";
				for(unsigned int i = 0; i < errorsLog.size(); i++)
				{
					errs += errorsLog[i] + " ";
				}
				return errs;
			}

			string code()
			{
				string ln = label;
				for(unsigned int i = label.size(); i < 10; i++){ln += ' ';} //fill the line to 10 char space
				ln += " " +instrunction;
				for(unsigned int i = instrunction.size(); i <  8; i++){ln += ' ';} //fill the line to 8 char space
				ln += " " +operand;
				for(unsigned int i = operand.size(); i < 10; i++){ln += ' ';} //fill the line to 10 char space
				return ln;
			}
			
			string& operator[](unsigned int i)
			{ 
				switch (i)
				{
				case 0:
					return line;
				case 1:
					return label;
				case 2:
					return instrunction;
				case 3:
					return operand;
				case 4:
					return loc;
				case 5:
					return opcode;
				case 6:
					return objCode;
				default:
					if(errorsLog.size() > (i - 7))
						return errorsLog[i-7];
					break;
				}
				return some;
			}
		};

		LINE makeLINE(vector<string> line)
		{
			LINE l(getLineNum(line), getLabel(line), getMnemonic(line), getOperand(line), getMepPos(line), getOpcode(line));
			l.addErrors(getErrors(line));
			return l;
		}

		int startAddrees;
		int endAddress;

		string filename;
		string asmFileName(){return filename+".asm";}
		string intermFileName(){return filename+".pss";}
		string objectFileName(){return filename+".o";}
		string listingFileName(){return filename+".lst";}

		bool error;

		map<string, Data> Mnemonic;
		map<string, string> symbolTable;

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

			Mnemonic["BYTE"]	= Data("00", Val(-1 , 1) );	//dinamic allocador
			Mnemonic["WORD"]	= Data("00", Val( 3 , 1) );
			Mnemonic["RESB"]	= Data("00", Val(-1 , 1) ); //dinamic allocador
			Mnemonic["RESW"]	= Data("00", Val(-3 , 1) ); //dinamic allocador

			Mnemonic["START"]	= Data("00", Val(0 , 1) );
			Mnemonic["END"]	= Data("00", Val(0 , 1) );
		}

		void inFile(std::ifstream  &out)
		{
			out.open( asmFileName().c_str() );
		}

		void midFile(std::ifstream & out)
		{
			out.open(intermFileName().c_str());
		}

		void midFile(std::ofstream & out)
		{
			out.open(intermFileName().c_str());
		}

		void listingFile(std::ofstream & out)
		{
			out.open(listingFileName().c_str());
		}

		void objectFile(std::ofstream & out)
		{
			out.open(objectFileName().c_str());
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

		int calculateCodeSize(LINE & line)
		{

			int offset = getOffset(line.instrunction);
			if(offset < 0)
			{
				int val;
				if(line.instrunction == "BYTE")
				{
					offset = calculateBYTESize(line.operand);
					if(offset <= 0)
						logError(line, "INVALID_OPERAND_LENGTH");
				}else if(intStringToInt( line.operand,val))
				{
					offset = (-offset)*val;
				}else{
					logError(line, "INVALID_OPERAND_VALUE");
				}
			}
			return offset;
		}

		int calculateBYTESize(string operant)
		{
			int size = operant.size()-3;
			//each two char is one WORD
			if(size > 0 && (operant[0] == 'x' || operant[0] == 'X'))
			{
				if(size%2 != 0)
					size++;
				size = size/2;
			}
			return size;
		}

		int calculateRESB(string operan)
		{
			int val = 0;
			if(intStringToInt(operan,val))
			{
				val = (-getOffset("RESB"))*val;
			}
			return val;
		}

		int calculateRESW(string operan)
		{
			int val = 0;
			if(intStringToInt(operan,val))
			{
				val = (-getOffset("RESW"))*val;
			}
			return val;
		}

		string hexOperad(LINE & line)
		{
			string hex = "";
			string operan = line.operan();
			if(operan.length() > 0)
			{
				//is hex value
				if(isNum( operan[0] )){
					int val = 0;
					HexStringToInt(operan, val);
					hex = expandHex(intToHexString(val),4);
				}else{ //is label
					if(getLabelValue(operan,hex)){
						hex = expandHex(hex,4);
					}else{
						logError(line,"ERROR_LABEL_"+operan+"_UNDEFINED");
					}
				}
				if(line.indexing())
				{
					int val = 0;
					if(HexCharToInt( hex[0], val)){
						val = val | 8; //set last vit to 1
					}
					hex[0] = intToHexChar(val);
				}
			}
			return hex;
		}

		int operandHexVal(LINE &line)
		{
			int val = 0;
			string operand = line.operand;
			//character array
			if(operand[0] == 'C' || operand[0] == 'c')
			{
				if(operand.size()-3 >= 30)
					logError(line, "INVALID_CHARACTER_STRING_LENGHT");
				else{
					string cstr = "";
					for(unsigned int i = 2; i < operand.size()-1; i++)//skip char '
						cstr += operand[i];
					if(!CharsToHexVal(cstr,val))
						logError(line, "UNABLE_TO_CONVERT_TO_HEX");
				}
			}else if(operand[0] == 'x' || operand[0] == 'X'){
				if(operand.size()-3 >= 32)
					logError(line, "INVALID_HEXADECIMAL_LENGHT");
				else{
					string cstr = "";
					for(unsigned int i = 2; i < operand.size()-1; i++)//skip char '
						cstr += operand[i];
					if(!HexStringToInt(cstr,val))
						logError(line, "UNABLE_TO_CONVERT_TO_HEX");
				}
			}else{
				intStringToInt(operand, val);
			}
			return val;
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

		vector<string> parseCode(string sin,int lnum)
		{
			sin =  str::replace(sin,'\t',' ');
			vector<string> crt;
			int num_id = 0;
			int lab_id = 1;
			int mne_id = 2;
			int arg_id = 3;
			crt.push_back(str::to_string(lnum));//line number
			crt.push_back("");//Label
			crt.push_back("");//mNemonic
			crt.push_back("");//arg
			unsigned int type = 1;//start at label
			char lc = '#'; //last character
			unsigned int args = 4; //default lnum label mnemonic operand {ignore comments}
			for(unsigned int i = 0; i < sin.length(); i++)
			{
				if(sin[i] == ' ' && sin[i] != lc){ //first space character hit add token to vector
					lc = sin[i];
					type++; //increase index;
					if(type == 3)//mnemonic loaded
					{
						args = 3 + getOperandSize(crt[mne_id]);
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

		vector<string> parseIntermetiateLine(string sin)
		{
			vector<string> tokens;
			string temp = "";
			for(unsigned int i = 0; i < sin.size(); i++)
			{
				if(sin[i] == '\t'){
					tokens.push_back(temp);
					temp.clear();
				}else{
					temp += sin[i];
				}
			}
			return tokens;
		}

		string formatLine(LINE &line)
		{
			string out = "";
			for(unsigned int i = 0; i < line.size(); i++)
			{
				out += line[i];
				out += "\t";
			}
			return out;
		}
		
		//line[0]
		string getLineNum(vector<string> line)
		{
			return line.size() >= 1 ? line[0] : "0";
		}
		
		//line[1]
		string getLabel(vector<string> line)
		{
			return line.size() >= 2 ? line[1] : "";
		}

		//line[2]
		string getMnemonic(vector<string> line)
		{
			return line.size() >= 3 ? line[2] : "";
		}

		//line[3]
		string getOperand(vector<string> line)
		{
			return line.size() >= 4 ? line[3] : "";
		}

		//line[4]
		string getMepPos(vector<string> line)
		{
			return line.size() >= 5 ? line[4] : "";
		}

		//line[5]
		string getOpcode(vector<string> line)
		{
			return line.size() >= 6 ? line[5] : "";
		}

		//
		vector<string> getErrors(vector<string> line)
		{
			vector<string> errs;
			for(unsigned int i = 6; i < line.size(); i++)
			{
				errs.push_back(line[i]);
			}
			return errs;
		}

		bool getLabelValue(string label, string & out)
		{
			out.clear();
			out = symbolTable[label];
			return out.length() > 0;
		}

		void logError(string description)
		{
			error = true;
			string s ="ERROR: "+ description;
			std::cout << s<<"\n";
		}

		void logError(LINE &line, string description)
		{
			error = true;
			string s = "Error on line: ";
			s += line.line;
			s += " <> ";
			s += description;
			std::cout << s<<"\n";
			line.logError(description);
		}

		void registerLabel(string label, int hex)
		{
			symbolTable[label] = intToHexString(hex);
		}

		bool validLINE(LINE & line)
		{
			return line.operand.size() >= getOperandSize(line.instrunction);
		}

		bool validCode(LINE & line)
		{
			bool error = false;
			//check if required operand
			bool operands = getOperandSize(line.instrunction) > 0;
			if(operands)
			{
				if(line.operand.empty()){
					logError(line, "MISSING_OPERAND");
					error = true;
				}
				//check if operands is valid
			}else if(!line.operand.empty()){
				logError(line, "UNEDED_OPERAND: "+line.operand);
				error = true;
			}
			return !error;
		}

		bool validOnTextRecord(string arg){
			return arg != "RESB" && arg != "RESW";
		}

		void checkLabel(LINE &code, int loc)
		{
			string label = code.label;
			if(balidLabel(label))
				registerLabel(label, loc);
			else if(!label.empty()){
				if(isNum(label[0]))
					logError(code, "INVALID_LABEL: " + label);
				else
					logError(code, "DUPLICATE_LABEL: " + label);
			}
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

		void savePass1(vector<LINE> lines)
		{
			std::ofstream out;
			midFile(out);
			for(unsigned int i = 0; i < lines.size(); i++)
			{
				out << formatLine(lines[i]) << "\n";
			}
			out.close();
		}

		string end(string hex){
			string s = "E";
			s += expandHex(hex,6);
			return s;
		}
		
		string header(LINE & line, int programSize)
		{
			string h = "H";
			h += line.label;
			h += " " + expandHex(line.operand,6);
			h += expandHex( intToHexString( programSize), 6);
			return h;
		}

		string generateObjectCode(LINE & line)
		{
			string code = "";
			if(line.instrunction == "WORD"){
				code = "00";
				code += expandHex(intToHexString(operandHexVal(line)),4);
			}else if(line.instrunction == "BYTE"){
				code +=  expandHex( intToHexString(operandHexVal(line)), calculateBYTESize(line.operand)*2 );
			}else if(line.instrunction == "RESB"){
				code += expandHex( "00", calculateRESB(line.operand));
			}else if(line.instrunction == "RESW"){
				code += expandHex( "00", calculateRESW(line.operand));
			}else{
				code = line.opcode;
				code += expandHex(hexOperad(line), 4);
			}
			return code;
		}

		string formatlisting(LINE & line)
		{
			//line	loc	code	objcode	errors
			string ln ="";
			ln += line.line;
			for( unsigned int i = line.line.size(); i < 4; i++)
				ln += " ";
			ln += '\t';
			ln += line.loc + '\t';
			ln += line.code() +'\t';
			ln += format(line.objCode,6) + '\t';
			ln += line.errors();
			return ln;
		}
		
		string format(string s,unsigned int size)
		{
			string out;
			for(unsigned int i = 0;  i < size; i++)
			{
				if( i < s.size())
					out += s[i];
				else
					out += " ";
			}
			return out;
		}

		string removeExtension(string s){
			unsigned int last = s.find(".asm");
			return s.substr(0,last);
		}

	public:
		assembler(string _filename)
		{
			filename = removeExtension( _filename);
			loadMnemonic();
			error = false;
		}

		void assembly(){
			std::cout <<"Starting Pass1....\n";
			pass1();
			if(error){
				std::cout <<"Exiting compiler, error encountered in PASS1.\n";
				return;
			}
			std::cout <<"Pass1 done...\n";
			std::cout <<"Starting Pass2...\n";
			pass2();
			if(error){
				std::cout <<"Exiting compiler, error encountered in PASS2.\n";
				return;
			}
			std::cout <<"Pass1 done...\n";
			std::cout <<"Compiling finished\n";
		}

		void pass1()
		{
			vector<LINE> lines;
			std::ifstream codeFile;
			inFile(codeFile);
			if(!codeFile.is_open()){
				logError("UNABLE TO OPEN FILE: "+ asmFileName());
				return;
			}
			int lnum = 1;
			int loc = 0;
			bool end = false;
			bool start = false;
			while(!codeFile.eof() && !end)
			{
				string line = str::getLine(&codeFile);
				if(!isComment(line))
				{
					LINE code = makeLINE(parseCode(line,lnum));
					if(str::equal(code.instrunction, "START"))
					{
						if(start)
							logError(code, "DUPLICATE_START_LABEL");
						else if(code.operand.empty())
							logError(code, "MISSING_OPERAND");
						else if(!HexStringToInt( code.operand, loc))
							logError(code, "UNRECOGNIZE_HEX_VALUE");
						else{
							checkLabel( code, loc);
						}
						code.loc = str::to_string(intToHexString(loc) );//add memory location
						start = true;
						startAddrees = loc;
					}else if(str::equal(code.instrunction, "END")){
						if(code.operand.empty())
							logError(code,"MISSING_OPERAND");
						checkLabel(code,loc);
						end = true;
						code.loc = str::to_string( intToHexString(loc) );//add memory location
						endAddress = loc;
					}else if(isMnemonic(code.instrunction)){
						if(validCode(code))//check if have correct num of arguments
						{
							checkLabel(code, loc);
						}
						code.loc = str::to_string( intToHexString(loc) );			//add memory location
						code.opcode = str::to_string( getOpcode(code.instrunction));	//set opcode
						loc += calculateCodeSize(code);
					}else{
						logError(code,"UNRECOGNIZED_MNEMONIC: "+code.instrunction);
						checkLabel( code, loc);
					}
					lines.push_back(code);
				}
				lnum++;//increase fisical line
			}
			codeFile.close();
			savePass1(lines);
		}

		void pass2()
		{
			std::ofstream lstFile;	//listing file
			listingFile(lstFile);
			lstFile << "Line\tLoc\t\t\tSource Statement\t\t\tObject Code\n";

			
			std::ofstream objFile;
			objectFile(objFile);

			std::ifstream file;
			midFile(file);
			if(!file.is_open()){
				logError("UNABLE TO OPEN FILE: "+ intermFileName());
				return;
			}

			TexRecord txtr;
			txtr.clear();
			while(!file.eof())
			{
				string objcode;
				LINE line = makeLINE( parseIntermetiateLine(getLine(file)));
				if( !validLINE(line))
					continue;
				if(line.instrunction == "START"){
					//save objectcode header
					objFile << header(line, endAddress-startAddrees) <<"\n";
				}else if(line.instrunction == "END"){
					//save last text record
					if(txtr.isValid()){
						objFile << txtr.to_string() << "\n";
						txtr.clear();
					}
					objFile << end( intToHexString(startAddrees)) <<"\n";
				}else if(isMnemonic(line.instrunction)){
					line.objCode = generateObjectCode(line);
					if( validOnTextRecord(line.instrunction)){
						if(!txtr.isValid()){
							txtr.start(line.loc);
						}
						txtr.addObjectCode(line.objCode);
						if(txtr.isFull()){
							objFile << txtr.to_string() << "\n";
							txtr.clear();
						}
					}else{
						if(txtr.isValid()){
							objFile << txtr.to_string() << "\n";
							txtr.clear();
						}
					}
				}
				lstFile << formatlisting(line) << "\n";
			}
			
			file.close();
			objFile.close();
			lstFile.close();
		}
	};


};




#endif /* ASSEMBLER_H_ */
