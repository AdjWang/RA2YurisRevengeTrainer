#include "stdafx.h"
#include "Psapi.h"
#include "tlhelp32.h"
#include "windows.h"
#include "trainerbase.h"
#pragma comment(lib,"Psapi.lib")


BOOL TrainerBase::GetProcessIDFromName(char *name,LPDWORD id)
{
    //获得系统快照句柄 (通俗的讲, 就是得到当前的所有进程)   
    HANDLE hSnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS,0) ;   
    PROCESSENTRY32 pInfo; //用于保存进程信息的一个数据结构   
    pInfo.dwSize = sizeof(pInfo);   
    //从快照中获取进程列表   
    Process32First(hSnapShot, &pInfo) ; //从第一个进程开始循环   
    do   
    {   
        //这里的 pszProcessName 为你的进程名称   
        if(_stricmp(pInfo.szExeFile, name) == 0)   
        {   
            *id = pInfo.th32ProcessID ;    //id 就是你要的进程PID 了..   
			return TRUE;
		}   
    }while(Process32Next(hSnapShot, &pInfo) != FALSE);   
    return FALSE;	//没有找到
} 

DWORD TrainerBase::getID()        //得到窗口进程句柄
{
//	HWND gameh;        //窗口句柄
	DWORD id;        //进程ID
//	DWORD gwtpi;

	//1 取窗口句柄
//	gameh = ::FindWindow(NULL,GameCaption);
	//2 取进程ID
//	gwtpi = ::GetWindowThreadProcessId(gameh,&id);
	if(!GetProcessIDFromName(GameCaption,&id))	//使用进程名获取ID
	{
		return 0;	//获取失败
	}
	//3 取窗口进程句柄及权限
	pid = OpenProcess(PROCESS_ALL_ACCESS,false,id);
	return id;
}

BOOL TrainerBase::IsGameRunning(DWORD mode)		//游戏运行中
{
	DWORD hHandle = getID();
	if (hHandle == 0) {
		if (mode == RUNNING_ALERT) {
			::MessageBox(NULL, "请开启游戏!!", "提示:", NULL);
		}
		return FALSE;
	}
	return TRUE;
}

BOOL TrainerBase::readMemory(DWORD m,DWORD* dat) //读取 内存数据
{
	DWORD add_m;
	getID();
	if(!ReadProcessMemory(pid,(void *)m,&add_m,4,0))
		return FALSE;	//读取失败
	*dat = add_m;
	return TRUE;
}

//const DWORD add[?]={首地址,偏移1,偏移2,...};
BOOL TrainerBase::readMemory(const DWORD* add,const DWORD len,DWORD* dat) //读取 地址+偏移 的内存数据
{
	DWORD add_m,n,i;
	DWORD m = *add;		//获取首地址
	getID();
	if(!ReadProcessMemory(pid,(void *)m,&add_m,4,0))
		return FALSE;	//读取失败
	for(i=1;i<len;i++)
	{
		n = *(add+i);		//获取偏移
		m = add_m + n; 	
		if(!ReadProcessMemory(pid,(void *)m,&add_m,4,0))
			return FALSE;	//读取失败
	}
	*dat = add_m;
	return TRUE;
}

//const DWORD add[?]={首地址,偏移1,偏移2,...};
BOOL TrainerBase::readAddress(const DWORD* add,const DWORD len,DWORD* dat) //读取 地址+偏移 的内存数据的地址
{
	DWORD add_m,n,i;
	DWORD m = *add;		//获取首地址
	getID();
	if(!ReadProcessMemory(pid,(void *)m,&add_m,4,0))
		return FALSE;	//读取失败
	for(i=1;i<len-1;i++)
	{
		n = *(add+i);		//获取偏移
		m = add_m + n; 	
		if(!ReadProcessMemory(pid,(void *)m,&add_m,4,0))
			return FALSE;	//读取失败
	}
	*dat = add_m + *(add+len-1);
	return TRUE;
}

void TrainerBase::writeMemory(DWORD m,DWORD n) //写入内存数据
{
	getID();
	WriteProcessMemory(pid,(LPVOID)m,&n,4,0);
}
void TrainerBase::writeMemory(DWORD m,DWORD n,DWORD size) //写入内存数据 +大小
{
	getID();
	WriteProcessMemory(pid,(LPVOID)m,&n,size,0);
}

void TrainerBase::virtualAE(LPVOID* address) //判断是否分配一块内存
{
	if(*address != NULL)	//指针不为空
	{       
		//这里一定要释放防止游戏重启时出错
		::VirtualFreeEx(pid, *address, 0, MEM_RELEASE);		//释放内存
	}
	*address = VirtualAllocEx(pid,NULL,256,MEM_COMMIT,PAGE_EXECUTE_READWRITE);
}

void TrainerBase::jmpWritProc(LPVOID n,LPVOID m) //写入地址,跳转的地址
{
	DWORD sizeRell;                        //实际写入的字节数 只为了校验
	DWORD add_w =(DWORD)n; //地址
	int ass_v =0xE9;           //内容               
	WriteProcessMemory(pid,(LPVOID)add_w,&ass_v,1,&sizeRell);
	add_w += 1;                                        //地址+1
	ass_v=int(m)- (add_w+4);        //内容
	WriteProcessMemory(pid,(LPVOID)add_w,&ass_v,4,&sizeRell);
}

//远程注入
void TrainerBase::writProcess(LPVOID callF)
{
	getID();
	LPVOID PowerCallBase_l;
	PowerCallBase_l=::VirtualAllocEx(pid,NULL,0x256,MEM_COMMIT,PAGE_EXECUTE_READWRITE);
	WriteProcessMemory(pid,PowerCallBase_l,callF,0x256,NULL);
	DWORD ttid = 0;
	CreateRemoteThread(pid,NULL,0,(LPTHREAD_START_ROUTINE)PowerCallBase_l,0,0,&ttid);
	Sleep(300);
	BOOL test;
	test = VirtualFreeEx(pid,PowerCallBase_l,0,MEM_RELEASE);
	if (test==false){MessageBox(NULL,"请开启游戏!!","提示:",NULL);}
}

///////////////////////////////////////////////////////////////////////////////////

// 调用AutoAssemble
bool TrainerBase::AutoAssemble(HANDLE hProcess, char * aa_script, int command) {
	typedef BOOL(FAR WINAPI *PROC1)(HANDLE, char*, int);
	PROC1 pAutoAssemble = (PROC1)GetProcAddress(hDLL, "AutoAssemble");
	bool isSuccess = (pAutoAssemble)(hProcess, aa_script, command);
	return isSuccess;
}

// 调用GetAddress
DWORD TrainerBase::GetAddress(HANDLE hProcess, char * address_line) {
	typedef DWORD(FAR WINAPI *PROC2)(HANDLE, char*);
	PROC2 pGetAddress = (PROC2)GetProcAddress(hDLL, "GetAddress");
	DWORD add = (pGetAddress)(hProcess, address_line);
	return add;
}

// 调用Ver
char* TrainerBase::Ver(void) {
	typedef char *(FAR WINAPI *PROC3)();
	PROC3 pVer = (PROC3)GetProcAddress(hDLL, "Ver");
	char *ver = (pVer)();
	return ver;
}

