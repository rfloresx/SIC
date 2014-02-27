//==========================================================
//	Otrebor4
//
//----------------------------------------------------------
// StringLib.h
//	contain string functions:
//		function toLowerCase: make string all lower case
//		function split:	tokenization string
//		function trim:	remove white spaces at end and begin of string
//		function merge:	reduce of number of consecutive char on a string to a single char ex: "some      here" => "some here"
//		function GetLine: getline from standar string library
//		function copyStr: copy string character by character
//==========================================================


#ifndef STRINGLIB_H
#define STRINGLIB_H

#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>

using std::vector;
#ifndef EOF
# define EOF (-1)
#endif
namespace str
{

	typedef std::vector<std::string> strings;
	typedef std::string string;

	strings split(string,char);
	string trim(string sin);
	string merge(string sin, char targ);
	string replace(string sin, char targ, char rep);
	bool equal ( string s1, string s2 );
	bool match (string s1, string s2, unsigned int ch);
	char ** stringsToCharArray( strings list );
	string getLine(std::istream * sin, char targ);
	string getLine(std::istream * sin);
	void copyStr(string from, string & dest);
	void copyStr(const char from[], char ** dest, int count );
    template<typename T> string to_string(T);

		//parse instruction giving a vector of strings
	// 0 label, 1 instruction, 2 operand, 3 comment
	vector<string> ParseInstruction(string sin)
	{
		sin =  str::replace(sin,'\t',' ');
		vector<string> crt;
		char lc = '#'; //last character
		string token = "";
		bool comment = sin[0] == '.';
		for(unsigned int i = 0; i < sin.length(); i++)
		{
			if(comment)
			{
				token += sin[i];
			}else if(sin[i] == ' ' && sin[i] != lc){ //first space character hit add token to vector
				lc = sin[i];
				crt.push_back( token );
				token = "";
				if(crt.size() >= 3) //next lines are comments
					comment = true;
			}else if(sin[i] != ' '){
				token += sin[i];
				lc = sin[i];
			}
		}
		if(!token.empty())
			crt.push_back(token);
		return crt;
	}

	vector< vector<string> > ParseInstructions(std::istream &sin)
	{
		vector< vector<string> > code;
		while( !sin.eof() )
		{
			vector<string> instruction = ParseInstruction( str::merge( str::getLine(&sin), ' ' ) );
			code.push_back( instruction );
		}
		return code;
	}

	vector< vector<string> > ParseInstructionsFile(string fileName)
	{
		vector< vector<string> > out;

		std::fstream file(fileName.c_str() );
		if(file.is_open())
		{
			out = ParseInstructions( file );
		}
		file.close();
		return out;
	}

    template<typename T>
    string to_string(T in)
    {
        std::stringstream ss;
        ss << in;
        std::string s(ss.str());
        return s;
    }

	string toLowerCase(string in)
	{
		string _new;
		for(unsigned int i = 0; i < in.length(); i++)
		{
			if(in[i] >= 'A' && in[i] <= 'Z')
			{
				_new += char(in[i]+32);
			}else{
				_new += in[i];
			}

		}
		return _new;
	}

	string toUperCase(string in)
	{
		string _new;
		for(unsigned int i = 0; i < in.length(); i++)
		{
			if(in[i] >= 'a' && in[i] <= 'z')
			{
				_new += char(in[i]-32);
			}else{
				_new += in[i];
			}

		}
		return _new;
	}

	//string tokenized functions
	strings split( string sin, char targ)
	{
		strings tokens;
		string token;
		for(unsigned int i = 0; i < sin.length(); i++)
		{
			if(sin[i] == targ)
			{
				tokens.push_back(token);
				token = "";
			}else{
				token += sin[i];
			}
		}
		if(!token.empty())
			tokens.push_back(token);
		return tokens;
	}

	//remove white space at end and begin
	string trim(string sin)
	{
		string sout = sin;
		//unsigned int i = 0;
		//remove white space at begin
		while(!sout.empty() && sout[0] == ' ')
		{
			sout = sout.substr(1,sout.length());
		}

		//remove white spaces at last
		while(sout.length() > 2 && sout[sout.length()-1] == ' ')
		{
			sout = sout.substr(0,sout.length()-2);
		}
		return sout;
	}

	//merge consecutive targ charagter on sin
	string merge(string sin, char targ)
	{
		string sout = "";
		//merge target scharacters on string
		for( unsigned int i = 0; i < sin.length(); i++)
		{
			if(!(sin[i] == targ && (sout.length() > 0 && sout[sout.length()-1] == targ)))
			{
				sout += sin[i];
			}
		}
		return sout;
	}

	string replace(string sin, char targ, char rep)
	{
		string sout = "";
		//merge target scharacters on string
		for( unsigned int i = 0; i < sin.length(); i++)
		{
			if((sin[i] == targ))
			{
				sout += rep;
			}else{
				sout += sin[i];
			}
		}
		return sout;
	}

	bool equal ( string s1, string s2 )
	{
		if(s1.length() != s2.length())
			return false;
		unsigned int i = 0;
		for(; i < s1.length(); i++)
		{
			if(s1[i] != s2[i])
				return false;
		}
		return true;
	}

    bool match (string s1, string s2, unsigned int ch)
	{
		if(s1.length() == s2.length())
            return equal(s1, s2);
        if(s1.length() > s2.length())
        {
            string temp = s2;
            s2 = s1;
            s1 = temp;
        }
        if(s1.length() < ch)
            return false;
        unsigned int i = 0;
        for(; i < s1.length(); i++)
        {
            if(s1[i] != s2[i])
                return false;
        }
        return true;
	}

	char ** stringsToCharArray( strings list )
	{
		char ** out = new char*[list.size()];
		for(unsigned int i =0; i < list.size(); i++)
		{
			copyStr( list[i].c_str(), &out[i], list[i].length());
		}
		return out;
	}

	//GetLine from standar string library
	string getLine(std::istream * sin, char targ)
	{
		std::string out = "";
		bool end = false;
		std::ios_base::iostate _State = std::ios_base::goodbit;
		char ch = sin->rdbuf()->sgetc();
		do
		{
			out += ch;
			ch = sin->rdbuf()->snextc();
			if(ch == EOF)
			{
				end = true;
				_State  |= std::ios_base::eofbit;
			}
			if(ch == targ)
			{
				sin->rdbuf()->sbumpc();
				end = true;
			}
		}while(!end);
		sin->width(0);
		sin->setstate( _State);
		return out;
	}

	string getLine(std::istream * sin)
	{
		return getLine(sin, '\n');
	}

	//copy a string to new string
	void copyStr(string from, string & dest)
	{
		dest.clear();
		for(unsigned int i =0; i < from.length(); i++)
		{
			dest += from[i];
		}
	}

	//copy char_array string to char_array string
    void copyStr(const char from[], char ** dest, int count )
    {
        (*dest) = new char[count+1]();
        for( int i = 0; i < count+1; i++)
        {
            (*dest)[i] = from[i];
        }
    }

};

#endif
