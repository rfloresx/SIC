//==========================================================
//	Otrebor4
//
//----------------------------------------------------------
//main function,
//crease a new assembler using "source" as the input/output file-name
//execute pass1
//==========================================================

#include <iostream>
#include "assembler.h"

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
}

int main()
{
	ASM::assembler a("source");
	a.pass1();
	a.pass2();
	ASM::assembler b("fail");
	b.pass1();
	b.pass2();
	System::Pause();
    return 0;
}

