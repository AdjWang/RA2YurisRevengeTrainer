#ifndef __TRAINERBASE_H__
#define __TRAINERBASE_H__

#pragma once

class TrainerBase
{
public:

	TrainerBase():GameCaption("gamemd.exe"){}

	const PCHAR GameCaption;        //标题名
	HANDLE pid; //进程句柄及权限

	BOOL GetProcessIDFromName(char *name,LPDWORD id);
	DWORD getID();
	BOOL IsGameRunning();
	BOOL readMemory(DWORD m,DWORD* dat);
	BOOL readMemory(const DWORD* add,const DWORD len,DWORD* dat);
	BOOL readAddress(const DWORD* add,const DWORD len,DWORD* dat);
	void writeMemory(DWORD m,DWORD n);
	void writeMemory(DWORD m,DWORD n,DWORD size);
	void virtualAE(LPVOID* address);
	void jmpWritProc(LPVOID n,LPVOID m);
	void writProcess(LPVOID callF);
};


#endif //__TRAINERBASE_H__
