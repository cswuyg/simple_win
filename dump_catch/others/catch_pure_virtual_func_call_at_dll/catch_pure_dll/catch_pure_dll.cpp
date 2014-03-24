// catch_pure_dll.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "catch_pure_dll.h"
#include <windows.h>
#include <stdexcept>
#include "D:\temp\catch_pure_virtual_func_call\catch_pure_virtual_func_call\model\pure_call.h"


// This is an example of an exported variable
CATCH_PURE_DLL_API int ncatch_pure_dll=0;

static void MyPureCallHandler(void)
{    
	MessageBox(0, 0, 0,0);
	throw std::invalid_argument("");
}

// This is an example of an exported function.
CATCH_PURE_DLL_API int fncatch_pure_dll(void)
{
	_purecall_handler m_prePch = _set_purecall_handler(MyPureCallHandler);   //At application, this call can stop show the error message box.
	CPureCall obj;
	//int i = 0;
	//int c = 10 / i;
	//if (c)
	//	return 42;
	//else 
	return 43;
}

// This is the constructor of a class that has been exported.
// see catch_pure_dll.h for the class definition
Ccatch_pure_dll::Ccatch_pure_dll()
{
	return;
}
