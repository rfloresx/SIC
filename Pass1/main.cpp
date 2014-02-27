//==========================================================
// Otrebor4
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

#elif __linux
		std::cout << "Enter to exit the program: ";
		std::cin.ignore().get();
    // linux
#elif __unix
    // Unix
#elif __posix
    // POSIX
#endif
	}
}

int main()
{
	ASM::assembler a("source");
	a.pass1();

	System::Pause();
    return 0;
}

