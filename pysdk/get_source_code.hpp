#pragma once
#include "stdafx.h"
#include <string>
#include <Windows.h>
#include "resource.h"
#ifdef USE_H
namespace pysdk {
	using string = std::string;
	const string read_resource(WORD id,LPCTSTR type = TEXT("PY")) {
		HRSRC res = FindResource(GetModuleHandle(NULL), MAKEINTRESOURCE(id), type);
		if (res) {
			HGLOBAL   global = LoadResource(GetModuleHandle(NULL), res);
			if (global) {
				LPVOID data = LockResource(global);
				if (data) {
					DWORD  size = SizeofResource(GetModuleHandle(NULL), res);
					return string((char*)data);
				}
			}
		}
		return "";
	}
};
#endif