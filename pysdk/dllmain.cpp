// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "stdafx.h"
#include "include/python.h"

extern PyObject *py_obj_handler;
extern PyObject *module_dict;
BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{

	SetDllDirectory(TEXT("./plugin/py/"));

    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
		Py_SetPythonHome(L"./plugin/py/");
		//Python初始化
		Py_Initialize();

		
		break;
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
		Py_Finalize();
        break;
    }
    return TRUE;
}
