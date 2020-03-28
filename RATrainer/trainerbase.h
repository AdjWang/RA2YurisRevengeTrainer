#ifndef __TRAINERBASE_H__
#define __TRAINERBASE_H__

#pragma once

#define	RUNNING_SILENT	0
#define	RUNNING_ALERT	1

class TrainerBase
{
public:
	TrainerBase():GameCaption("gamemd.exe"){
		hDLL = LoadLibrary(TEXT("aa_engine.dll"));  // 载入DLL
		if (hDLL == 0) {
			::MessageBox(NULL, "没有找到aa_engine.dll，修改器必须使用同目录下的该文件", "错误:", NULL);
			exit(1);
		}
	}
	~TrainerBase() {
		FreeLibrary(hDLL);  // 释放DLL
	}

	const PCHAR GameCaption;        //标题名
	HMODULE hDLL;	// aa_engine.dll
	HANDLE pid; //进程句柄及权限

	BOOL GetProcessIDFromName(char *name,LPDWORD id);
	DWORD getID();
	BOOL IsGameRunning(DWORD mode);
	BOOL readMemory(DWORD m,DWORD* dat);
	BOOL readMemory(const DWORD* add,const DWORD len,DWORD* dat);
	BOOL readAddress(const DWORD* add,const DWORD len,DWORD* dat);
	void writeMemory(DWORD m,DWORD n);
	void writeMemory(DWORD m,DWORD n,DWORD size);
	void virtualAE(LPVOID* address);
	void jmpWritProc(LPVOID n,LPVOID m);
	void writProcess(LPVOID callF);

	bool AutoAssemble(HANDLE hProcess, char * aa_script, int command);
	DWORD GetAddress(HANDLE hProcess, char * address_line);
	char * Ver();
};

#endif //__TRAINERBASE_H__
