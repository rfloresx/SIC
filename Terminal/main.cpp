//==========================================================
//	Otrebor4
//	
//----------------------------------------------------------
//main call:
//	Terminal: object that handle terminal logic
//	execute Terminal::main function.
//==========================================================

#include <iostream>
#include <string>
#include "terminal.h"


using namespace std;
int main(int ct, const char * args [])
{
	Terminal promt;
	promt.main(ct,args);
	//system("pause");
	return 0;
}
