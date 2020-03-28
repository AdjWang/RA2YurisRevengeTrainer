#include "stdafx.h"
#include "Psapi.h"
#include "tlhelp32.h"
#include "windows.h"
#include "trainerbase.h"
#pragma comment(lib,"Psapi.lib")


BOOL TrainerBase::GetProcessIDFromName(char *name,LPDWORD id)
{
    //���ϵͳ���վ�� (ͨ�׵Ľ�, ���ǵõ���ǰ�����н���)   
    HANDLE hSnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS,0) ;   
    PROCESSENTRY32 pInfo; //���ڱ��������Ϣ��һ�����ݽṹ   
    pInfo.dwSize = sizeof(pInfo);   
    //�ӿ����л�ȡ�����б�   
    Process32First(hSnapShot, &pInfo) ; //�ӵ�һ�����̿�ʼѭ��   
    do   
    {   
        //����� pszProcessName Ϊ��Ľ�������   
        if(_stricmp(pInfo.szExeFile, name) == 0)   
        {   
            *id = pInfo.th32ProcessID ;    //id ������Ҫ�Ľ���PID ��..   
			return TRUE;
		}   
    }while(Process32Next(hSnapShot, &pInfo) != FALSE);   
    return FALSE;	//û���ҵ�
} 

DWORD TrainerBase::getID()        //�õ����ڽ��̾��
{
//	HWND gameh;        //���ھ��
	DWORD id;        //����ID
//	DWORD gwtpi;

	//1 ȡ���ھ��
//	gameh = ::FindWindow(NULL,GameCaption);
	//2 ȡ����ID
//	gwtpi = ::GetWindowThreadProcessId(gameh,&id);
	if(!GetProcessIDFromName(GameCaption,&id))	//ʹ�ý�������ȡID
	{
		return 0;	//��ȡʧ��
	}
	//3 ȡ���ڽ��̾����Ȩ��
	pid = OpenProcess(PROCESS_ALL_ACCESS,false,id);
	return id;
}

BOOL TrainerBase::IsGameRunning(DWORD mode)		//��Ϸ������
{
	DWORD hHandle = getID();
	if (hHandle == 0) {
		if (mode == RUNNING_ALERT) {
			::MessageBox(NULL, "�뿪����Ϸ!!", "��ʾ:", NULL);
		}
		return FALSE;
	}
	return TRUE;
}

BOOL TrainerBase::readMemory(DWORD m,DWORD* dat) //��ȡ �ڴ�����
{
	DWORD add_m;
	getID();
	if(!ReadProcessMemory(pid,(void *)m,&add_m,4,0))
		return FALSE;	//��ȡʧ��
	*dat = add_m;
	return TRUE;
}

//const DWORD add[?]={�׵�ַ,ƫ��1,ƫ��2,...};
BOOL TrainerBase::readMemory(const DWORD* add,const DWORD len,DWORD* dat) //��ȡ ��ַ+ƫ�� ���ڴ�����
{
	DWORD add_m,n,i;
	DWORD m = *add;		//��ȡ�׵�ַ
	getID();
	if(!ReadProcessMemory(pid,(void *)m,&add_m,4,0))
		return FALSE;	//��ȡʧ��
	for(i=1;i<len;i++)
	{
		n = *(add+i);		//��ȡƫ��
		m = add_m + n; 	
		if(!ReadProcessMemory(pid,(void *)m,&add_m,4,0))
			return FALSE;	//��ȡʧ��
	}
	*dat = add_m;
	return TRUE;
}

//const DWORD add[?]={�׵�ַ,ƫ��1,ƫ��2,...};
BOOL TrainerBase::readAddress(const DWORD* add,const DWORD len,DWORD* dat) //��ȡ ��ַ+ƫ�� ���ڴ����ݵĵ�ַ
{
	DWORD add_m,n,i;
	DWORD m = *add;		//��ȡ�׵�ַ
	getID();
	if(!ReadProcessMemory(pid,(void *)m,&add_m,4,0))
		return FALSE;	//��ȡʧ��
	for(i=1;i<len-1;i++)
	{
		n = *(add+i);		//��ȡƫ��
		m = add_m + n; 	
		if(!ReadProcessMemory(pid,(void *)m,&add_m,4,0))
			return FALSE;	//��ȡʧ��
	}
	*dat = add_m + *(add+len-1);
	return TRUE;
}

void TrainerBase::writeMemory(DWORD m,DWORD n) //д���ڴ�����
{
	getID();
	WriteProcessMemory(pid,(LPVOID)m,&n,4,0);
}
void TrainerBase::writeMemory(DWORD m,DWORD n,DWORD size) //д���ڴ����� +��С
{
	getID();
	WriteProcessMemory(pid,(LPVOID)m,&n,size,0);
}

void TrainerBase::virtualAE(LPVOID* address) //�ж��Ƿ����һ���ڴ�
{
	if(*address != NULL)	//ָ�벻Ϊ��
	{       
		//����һ��Ҫ�ͷŷ�ֹ��Ϸ����ʱ����
		::VirtualFreeEx(pid, *address, 0, MEM_RELEASE);		//�ͷ��ڴ�
	}
	*address = VirtualAllocEx(pid,NULL,256,MEM_COMMIT,PAGE_EXECUTE_READWRITE);
}

void TrainerBase::jmpWritProc(LPVOID n,LPVOID m) //д���ַ,��ת�ĵ�ַ
{
	DWORD sizeRell;                        //ʵ��д����ֽ��� ֻΪ��У��
	DWORD add_w =(DWORD)n; //��ַ
	int ass_v =0xE9;           //����               
	WriteProcessMemory(pid,(LPVOID)add_w,&ass_v,1,&sizeRell);
	add_w += 1;                                        //��ַ+1
	ass_v=int(m)- (add_w+4);        //����
	WriteProcessMemory(pid,(LPVOID)add_w,&ass_v,4,&sizeRell);
}

//Զ��ע��
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
	if (test==false){MessageBox(NULL,"�뿪����Ϸ!!","��ʾ:",NULL);}
}

///////////////////////////////////////////////////////////////////////////////////

// ����AutoAssemble
bool TrainerBase::AutoAssemble(HANDLE hProcess, char * aa_script, int command) {
	typedef BOOL(FAR WINAPI *PROC1)(HANDLE, char*, int);
	PROC1 pAutoAssemble = (PROC1)GetProcAddress(hDLL, "AutoAssemble");
	bool isSuccess = (pAutoAssemble)(hProcess, aa_script, command);
	return isSuccess;
}

// ����GetAddress
DWORD TrainerBase::GetAddress(HANDLE hProcess, char * address_line) {
	typedef DWORD(FAR WINAPI *PROC2)(HANDLE, char*);
	PROC2 pGetAddress = (PROC2)GetProcAddress(hDLL, "GetAddress");
	DWORD add = (pGetAddress)(hProcess, address_line);
	return add;
}

// ����Ver
char* TrainerBase::Ver(void) {
	typedef char *(FAR WINAPI *PROC3)();
	PROC3 pVer = (PROC3)GetProcAddress(hDLL, "Ver");
	char *ver = (pVer)();
	return ver;
}

