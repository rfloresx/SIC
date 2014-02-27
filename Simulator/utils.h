
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <iostream>
#ifndef UTILS_H
#define UTILS_H

#ifdef _WIN32
#define _WIN_
#elif _WIN64
#define _WIN_
#endif

namespace util{
	char *m_strdup(const char *s){
	    #ifdef _WIN_
            return _strdup(s);
	    #elif __linux__
            return strdup(s);
		#endif
		return new char();
	}

	int m_fscanf ( FILE * stream, const char * format, ... ){
	    #ifdef _WIN_
		return fscanf_s(stream, format);
        #elif __linux__
        return fscanf(stream, format);
        #endif
        return -1;
	}

	FILE * m_fopen ( const char * filename, const char * mode )
	{
	    #ifdef _WIN_
		FILE * file;
		fopen_s(&file,filename,mode);
		return file;
		#elif __linux__
        return fopen(filename,mode);
		#endif

	}

	int m_sprintf ( char * str, const char * format, ... ){
	    #ifdef _WIN_
		return sprintf_s(str, sizeof(str), format);
		#elif __linux__
		return sprintf(str,format);
		#endif
	}

	void m_strcat ( char * destination, const char * source )
	{
	    #ifdef _WIN_
		strcat_s(destination, sizeof(destination), source);
		#elif __linux__
		strcat(destination,source);
		#endif
	}

}

namespace System
{
	void Pause()
	{
#ifdef _WIN64
		std::system("pause");
   //define something for Windows (64-bit)
#elif _WIN32
		std::system("pause");
   //define something for Windows (32-bit)
#elif __APPLE__
		//apple pause need implementation
#elif __linux__
		std::cout << "Enter to exit the program: ";
		std::cin.ignore().get();
    // linux
#elif __unix__
    // Unix
#endif
	}

	void Directory(std::string arg)
	{
		std::string cmd = "";
#ifdef _WIN64
		cmd = "dir";
   //define something for Windows (64-bit)
#elif _WIN32
		cmd = "dir";
   //define something for Windows (32-bit)
#elif __APPLE__
		//apple pause need implementation
#elif __linux__
		cmd = "ls";
    // linux
#elif __unix__
    // Unix
#endif
		cmd += arg;
		#ifdef _WIN_
		std::system( cmd.c_str());
		#elif _linux_
		system(cmd.c_str());
		#endif
	}
}

#endif
