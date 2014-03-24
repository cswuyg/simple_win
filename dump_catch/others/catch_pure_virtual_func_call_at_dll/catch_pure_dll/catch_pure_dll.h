// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the CATCH_PURE_DLL_EXPORTS
// symbol defined on the command line. This symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// CATCH_PURE_DLL_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef CATCH_PURE_DLL_EXPORTS
#define CATCH_PURE_DLL_API __declspec(dllexport)
#else
#define CATCH_PURE_DLL_API __declspec(dllimport)
#endif

// This class is exported from the catch_pure_dll.dll
class CATCH_PURE_DLL_API Ccatch_pure_dll {
public:
	Ccatch_pure_dll(void);
	// TODO: add your methods here.
};


extern CATCH_PURE_DLL_API int ncatch_pure_dll;

extern "C"
{
CATCH_PURE_DLL_API int fncatch_pure_dll(void);
}
