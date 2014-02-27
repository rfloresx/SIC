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
#include "terminal.h"

int main(int ct, const char * args [])
{
	Terminal prom;
	prom.main(ct,args);

	return 0;
}

