#include "stdafx.h"
#include "trainerfunctions.h"

LPVOID PowerCallBase;        //����������ڴ�ָ��
LPVOID NoPowerCallBase;      //ͣ��������ڴ�ָ��
LPVOID PutCallBase;        //���⽨��������ڴ�ָ��
LPVOID PutWaterCallBase;    //ˮ�潨��������ڴ�ָ��
LPVOID AtraCallBase;		//�Զ�����������ڴ�ָ��
LPVOID AtraStrCallBase;		//�����Զ�����������ڴ�ָ��
LPVOID ReYuriCallBase;		//������������ڴ�ָ��
LPVOID PerYuriCallBase;		//���ÿ���������ڴ�ָ��
LPVOID MineAtkCallBase;		//������������ڴ�ָ��
LPVOID MineBdiCallBase;		//������������ڴ�ָ��
LPVOID MineUneCallBase;		//��ռ��������ڴ�ָ��
LPVOID BdImeCallBase;		//˲�佨��������ڴ�ָ��
LPVOID UnbSKCallBase;		//�޵�/��ɱ������ڴ�ָ��
LPVOID AntiChronoCallBase;	//����ʱ��������ڴ�ָ��1
LPVOID AntChonoDbCallBase;	//����ʱ��������ڴ�ָ��2

LPVOID UnRaCallBase;		//������������ڴ�ָ��
//�Ƽ�ȫ��
LPVOID AllBCallBase;		//ȫ����������ڴ�ָ��
LPVOID AllCCallBase;		//ȫ����������ڴ�ָ��
LPVOID AllSCallBase;		//ȫ����������ڴ�ָ��
LPVOID AllPCallBase;		//ȫ�ɻ�������ڴ�ָ��

LPVOID FstAtkCallBase;		//���ٹ���������ڴ�ָ��
LPVOID FstTurnBtCallBase;	//����ת����������ڴ�ָ��
LPVOID FstTurnRdCallBase;	//����ת��������ڴ�ָ��
LPVOID FstRldCallBase;		//������װ������ڴ�ָ��
LPVOID FulAmuCallBase;		//��ҩ����������ڴ�ָ��
LPVOID ARgeAtkCallBase;		//Զ�̴��������ڴ�ָ��
LPVOID ARgeAleCallBase;		//Զ�̾���������ڴ�ָ��
LPVOID InsChoMovCallBase;	//˲�䳬ʱ���ƶ�������ڴ�ָ��
LPVOID InsChoAtkCallBase;	//˲�䳬ʱ�չ���������ڴ�ָ��
LPVOID AntiSpyCallBase;		//����������ڴ�ָ��
LPVOID DisGAGAPCallBase;	//̱���ѷ������������ڴ�ָ��
LPVOID DisAllCallBase;		//̱���з����е�λ������ڴ�ָ��
//LPVOID SpeedSetCallBase;	//����ֱ���滻����Ҫ�����ڴ�
LPVOID SoldAllCallBase;		//�����������е�λ������ڴ�ָ��
//LPVOID ;					//����-���� ֱ���滻����Ҫ�����ڴ�
//LPVOID ;					//����-������ֱ���滻����Ҫ�����ڴ�
LPVOID FlashMoveCallBase;	//����Ь������ڴ�ָ��
LPVOID EliteBdACallBase;	//��������������ڴ�ָ��1
LPVOID EliteBdVCallBase;	//��������������ڴ�ָ��2
LPVOID EliteBdSCallBase;	//��������������ڴ�ָ��3


//////////////////////////////////////////////�Ĵ���
BOOL TrainerFunctions::rSpeed(DWORD* pSpeed) //��ȡ�ٶ�
{
	const DWORD address = 0x00A8EB60;
	if(!readMemory(address,pSpeed))
		return FALSE;
	return TRUE;
}

BOOL TrainerFunctions::wSpeed(DWORD Speed) //д���ٶ�
{
	const DWORD address = 0x00A8EB60;
	DWORD SpeedTest;
	if(!readMemory(address,&SpeedTest))
		return FALSE;
	writeMemory(address,Speed);
	return TRUE;
}

BOOL TrainerFunctions::rMymoney(DWORD* pMoney) //��ȡ��Ǯ
{
	const DWORD address[2] = {0x00A83D4C,0x30C};
	DWORD money;
	if(!readMemory(address,2,&money))
		return FALSE;
	*pMoney = money;
	return TRUE;
}

BOOL TrainerFunctions::wMymoney(DWORD money) //��Ǯ�޸�
{
	const DWORD address[2] = {0x00A83D4C,0x30C};
	DWORD add;
	if(!readAddress(address,2,&add))
		return FALSE;
	writeMemory(add,money); //ʵ�ʽ�Ǯ��ַ
	return TRUE;
}


BOOL TrainerFunctions::rBuildMaxm(DWORD* pMaxm) //��ȡ�����������
{
	const DWORD address[2] = {0x008871E0,0xF0};
	DWORD maxm;
	if(!readMemory(address,2,&maxm))
		return FALSE;
	*pMaxm = maxm+1;
	return TRUE;
}

BOOL TrainerFunctions::wBuildMaxm(DWORD maxm) //�����޸�
{
	const DWORD address[2] = {0x008871E0,0xF0};
	DWORD add;
	if(!readAddress(address,2,&add))
		return FALSE;
	writeMemory(add,maxm-1); //
	return TRUE;
}

BOOL TrainerFunctions::QuickBuild() //���ٽ���
{
	int j=0;
	DWORD dat;
	if(!readMemory(0x00A83D4C,&dat))
		return FALSE;
	for(int i=1;i<=5;(j+=4),i++)
	{
		writeMemory(dat+j+0x5378,15);
	}
	return TRUE;
}

BOOL TrainerFunctions::RadarOn() //�������״�
{
	const DWORD address[2] = {0x00A8B230,0x34A4};
	DWORD add;
	if(!readAddress(address,2,&add))
		return FALSE;
	writeMemory(add,0x1,0x1);
	return TRUE;
}

BOOL TrainerFunctions::SuperOn() //���޳���
{
	//00 NuclearMissile
	//01 IronCurtain
	//02 ForceShield
	//03 LightningStorm
	//04 PsychicDominator
	//05 Chronosphere
	//06 Reinforcements
	//07 SpyPlane
	//08 GeneticMutator
	//09 PsychicReveal
	//����һ��11������
	const DWORD add_pre[2] = {0x00A83D4C,0x258};
	DWORD t1,t2,dis;	//dis->0~54

	if(!readMemory(add_pre,2,&t1)){return FALSE;}
	for(DWORD i=0; i<12; i++)		//11��ѭ��
	{
		if(!readMemory(t1+i*4,&t2)){return FALSE;}
		if(!readMemory(t2+0x78,&dis)){return FALSE;}	//��ȡ�����ʾ��û����-1
		if((long)dis != -1)
			writeMemory(t2+0x6F,0x1,0x1);		//�޸ľ�����ʶ
	}
	return TRUE;
}

void TrainerFunctions::WinImme() //����ʤ��
{
	writeMemory(0x00A83D49,0x1,0x1);
}

BOOL TrainerFunctions::TobeGhost() //��Ϊ�������
{
	DWORD dat1,dat2;
	readMemory(0x00A8ECC8,&dat1);	//ѡ������
	if(dat1 != 1)		//����ѡ��һ������
		return FALSE;

	DWORD address[3] = {0x00A8ECBC,0,0};
	readMemory(address,3,&dat1);		//ѡ�е�λ�׵�ַ
	if(dat1 != 0x007E3EBC)		//����ѡ��һ������
		return FALSE;

	address[2] = 0x21C;		//��λ����
	readMemory(address,3,&dat1);		//ѡ�е�λ����
	readMemory(0x00A83D4C,&dat2);		//������ݻ�ַ
	if(dat1 != dat2)		//����ѡ����ҵ�λ
		return FALSE;
	//����������ʼת��
	DWORD PlayerID[2] = {0x00A83D4C,0x30};
	readMemory(PlayerID,2,&dat1);		//��ȡ��ҵ�ǰID
	if(dat1 != 0)		//ת�Ƶ�����ID
		dat1--;
	else
		dat1++;

	DWORD AimID[2] = {0x00A8022C,4*dat1};	//Ŀ����һ�ַ
	readMemory(AimID,2,&dat2);		//��ȡĿ��������ݻ�ַ
	readAddress(address,3,&dat1);		//ѡ�е�λ�����洢��ַ
	writeMemory(dat1,dat2);		//ת��

	//address[2] = 0x6C;		//��λѪ��
	//readAddress(address,3,&dat1);		//ѡ�е�λѪ���洢��ַ
	//writeMemory(dat1,0);		//����ѡ�еĽ�����

	//ɾ��ѡ�е�λ
	writProcess(DeleteThis_Assemble);

	return TRUE;	
}

//////////////////////////////////////////////////�߳�
//ȫ��ͼ��������
void TrainerFunctions::Map_Assemble()
{
	_asm
	{
		pushad
		mov eax,0x00A83D4C
		mov edx,[eax]
		mov ecx,0x0087F7E8
		push edx
		mov eax,0x00577D90
		call eax
		popad                      
	}
}
void TrainerFunctions::AllMap()
{
	//��ͼȫ��
	writProcess(Map_Assemble);
}

//����˵���������
void TrainerFunctions::NuclearBomb_Assemble()
{
	int eax_temp;       
	__asm
	{
		pushad
		mov ecx,0x00A83D4C	//����Ƿ��Ѿ��к˵����侮
		mov ecx,[ecx]
		add ecx,0x258
		mov ecx,[ecx]
		mov ecx,[ecx]
		add ecx,0x78
		mov ecx,[ecx]
		cmp ecx,0xFFFFFFFF	//���-1��ʾû�� 0~54��ʾ��
		jne exit1	//�о��˳���ֹ����

		mov ecx,0x14
		mov eax_temp,0x006CEEB0
		call eax_temp
		mov ebx,eax
		//�����˵�CD
		add ebx,0x98
		mov eax,[ebx]//������
		mov edx,0x00A83D4C
		mov edx,[edx]
		add edx,0x258
		mov ecx,[edx]
		push 0           //��ʼ״̬��1=�ȴ�
		push 0           //��Ӧ����
		mov ecx,[ecx+eax*4]
		push 1             //1=һ���� �˵�����1 ��������0
		mov eax_temp,0x006CB560
		call eax_temp
		//���Ӻ˵�����ѡ��
		add ebx,0x98
		mov eax,[ebx]//������
		mov ecx,0x0087F7E8
		push eax
		push 0x1F
		mov eax_temp,0x006A6300
		call eax_temp

		exit1:
		popad             
	}       
}

void TrainerFunctions::GetaNuclearBomb()
{
	//����˵�
	writProcess(NuclearBomb_Assemble);
}

//��λ�����������
void TrainerFunctions::UpChose_Assemble()
{
	__asm
	{
		pushad
		mov eax,0x00A8ECBC
		mov ebx,[eax]	//ѡ�е�λ�׵�ַ
		mov eax,0

		process:
		mov ecx,0x00A8ECC8
		cmp eax,[ecx]  //ѡ�е�λ����Ϊ���ж�
		jge brek//>=��ת
		mov ecx,[ebx+eax*4]  //��ǰ��λ��ַ
		mov edx,0x40000000		//Float 2.0
		add ecx,0x150
		mov [ecx],edx  //��ǰ��λ����
		add eax,1
		jmp process

		brek:
		popad
	}
}

void TrainerFunctions::UpChose()
{
	//��λ����
	writProcess(UpChose_Assemble);
}

//��λ�����������
void TrainerFunctions::SpeedUpChose_Assemble()
{
	__asm
	{
		pushad
		mov eax,0x00A8ECBC
		mov ebx,[eax]	//ѡ�е�λ�׵�ַ
		mov eax,0

		process:
		mov ecx,0x00A8ECC8
		cmp eax,[ecx]  //ѡ�е�λ����
		jge brek//>=��ת
		mov ecx,[ebx+eax*4]  //��ǰ��λ��ַ
		mov edx,0x007E3EBC
		cmp [ecx],edx	//��ǰ��λ����Ϊ����
		je brek			//����û�ٶȲ��ܸ�
		mov edx,0x007EB058
		cmp [ecx],edx	//��������2.0
		jne next1
		mov edx,0x40000000
		add ecx,0x584
		mov [ecx],edx
		je loopcontinue
		next1:
		mov edx,0x007F5C70
		cmp [ecx],edx	//����
		jne next2
		mov edx,0x40A00000
		add ecx,0x584
		mov [ecx],edx	//5.0
		je loopcontinue
		next2:
		//�ɻ������û��
		//mov [ecx+584],3FF00000//1.875
		loopcontinue:
		add eax,1
		jmp process

		brek:
		popad
	}
}

void TrainerFunctions::SpeedUpChose()
{
	//��λ����
	writProcess(SpeedUpChose_Assemble);
}

//ȫ���ҵ�-ѡ�е�λ �������
void TrainerFunctions::MineChose_Assemble()
{
	__asm
	{
		pushad
		mov eax,0x00A8ECC8	//ѡ�е�λ����
		mov eax,[eax]
		cmp eax,0		//�Ƿ�ѡ�е�λ
		je exit1
		push 0  //
		mov ebx,0x00A83D4C
		mov eax,[ebx]
		push eax
		mov ebx,0x00A8ECBC
		mov eax,[ebx]
		mov ecx,[eax]
		mov ebx,[ecx]
		add ebx,0x3D4
		mov ebx,[ebx]
		call ebx
		exit1:
		popad
	}
}

void TrainerFunctions::MineChose()
{
	//ȫ���ҵ�-ѡ�е�λ
	writProcess(MineChose_Assemble);
}

//ɾ��ѡ�е�λ �������
void TrainerFunctions::DeleteThis_Assemble()
{
	__asm
	{
		pushad
		mov eax,0x00A8ECC8
		mov eax,[eax]//ѡ�е�λ����
		cmp eax,0		//�Ƿ�ѡ�е�λ
		je exit1

		mov eax,0x00A8ECBC
		mov eax,[eax]
		mov ecx,[eax]
		mov eax,[ecx]
		add eax,0xF8
		mov ebx,[eax]
		call ebx

		exit1:
		popad
	}
}

void TrainerFunctions::DeleteThis()
{
	//ɾ��ѡ�е�λ
	writProcess(DeleteThis_Assemble);
}


////////////////////////////////////ע��
__declspec(naked) void TrainerFunctions::NoLoad_Assemble()
{
	__asm
	{
		push ecx
		mov ecx,00A83D4Ch
		mov ecx,[ecx]      //������ݵ�ַ
		cmp ecx,esi
		jnz originalcode       //���������ת
		mov [esi+000053A8h],0     //�޸ģ���
		jmp exit1

		originalcode:
		mov [esi+000053A8h],edx

		exit1:
		pop ecx
	}
}

void TrainerFunctions::NoLoad() //���޵���
{
	DWORD sizeX;
	getID();        //get process HANDLE
	//PowerCallBase = VirtualAllocEx(pid,NULL,0x256,MEM_COMMIT,PAGE_EXECUTE_READWRITE);
	virtualAE(&PowerCallBase);        //VirtualAllocEx
	jmpWritProc((LPVOID)0x00508D18,PowerCallBase); //jmp write address
	writeMemory((DWORD)PowerCallBase,(DWORD)NoLoad_Assemble,256);
	writeMemory((0x00508D18+5),0x90,1);		//0x90->nop
	WriteProcessMemory(pid,PowerCallBase,&NoLoad_Assemble,256,&sizeX);
	jmpWritProc((LPVOID)((int)PowerCallBase + 0x1C),(LPVOID)(0x00508D18+6));
}

void TrainerFunctions::FreeNoLoad() //�ر����޵���
{       
	getID();
	DWORD add_l = 0x00508D18;
	BYTE jmp_l[6] ={0x89,0x96,0xA8,0x53,00,00};
	DWORD writex = WriteProcessMemory(pid,(LPVOID)add_l,&jmp_l,sizeof(jmp_l),0);
	::VirtualFreeEx(pid,PowerCallBase,0,MEM_RELEASE);
	PowerCallBase=0;
}
/*�ɰ�ͣ�磬����ɵ���ֻ��糧�����������°����������
__declspec(naked) void TrainerFunctions::NoPower_Assemble()
{
	__asm
	{
		push ecx
		mov ecx,00A83D4Ch
		mov ecx,[ecx]      //������ݵ�ַ
		cmp ecx,esi
		je originalcode       //�������ת
		mov ecx,[esi+000053A4h]     //����
		inc ecx		//����+1
		mov edx,ecx		//������Զ�ȸ���С1 ������\(^o^)/~
		jmp exit1

		originalcode:
		mov edx,[esi+000053A8h]

		exit1:
		pop ecx
	}
}

void TrainerFunctions::NoPower() //ͣ��
{
	DWORD sizeX;
	getID();        //get process HANDLE
	//PowerCallBase = VirtualAllocEx(pid,NULL,0x256,MEM_COMMIT,PAGE_EXECUTE_READWRITE);
	virtualAE(&NoPowerCallBase);        //VirtualAllocEx
	jmpWritProc((LPVOID)0x00508D0E,NoPowerCallBase); //jmp write address
	writeMemory((DWORD)NoPowerCallBase,(DWORD)NoPower_Assemble,256);
	writeMemory((0x00508D0E + 5),0x90,1);		//0x90->nop
	WriteProcessMemory(pid,NoPowerCallBase,&NoPower_Assemble,256,&sizeX);
	jmpWritProc((LPVOID)((int)NoPowerCallBase + 0x1E),(LPVOID)(0x00508D0E + 6));
}

void TrainerFunctions::FreeNoPower() //�ر�ͣ��
{       
	getID();
	DWORD add_l = 0x00508D0E;
	BYTE jmp_l[6] ={0x8B,0x96,0xA8,0x53,00,00};
	DWORD writex = WriteProcessMemory(pid,(LPVOID)add_l,&jmp_l,sizeof(jmp_l),0);
	::VirtualFreeEx(pid,NoPowerCallBase,0,MEM_RELEASE);
	NoPowerCallBase=0;
}
*/
//���ᵼ��AIֻ��糧���⣬���ǳ����������ϵ���أ��Ժ�Ľ���
__declspec(naked) void TrainerFunctions::NoPower_Assemble()
{
	__asm
	{
		mov ebx,esi
		add ebx,21Ch
		mov ebx,[ebx]
		push eax
		mov eax,00A83D4Ch
		cmp [eax],ebx
		pop eax
//		je 00455643h
		nop					//ռλ
		nop
		nop
		nop
		nop
		nop
		cmp[esi + 0000067Ch],edi
	}
}

void TrainerFunctions::NoPower() //ͣ��
{
	DWORD sizeX;
	getID();        //get process HANDLE
					//PowerCallBase = VirtualAllocEx(pid,NULL,0x256,MEM_COMMIT,PAGE_EXECUTE_READWRITE);
	virtualAE(&NoPowerCallBase);        //VirtualAllocEx
	jmpWritProc((LPVOID)0x00455639, NoPowerCallBase); //jmp write address
	writeMemory((DWORD)NoPowerCallBase, (DWORD)NoPower_Assemble, 256);
	writeMemory((0x00455639 + 5), 0x90, 1);		//0x90->nop
	writeMemory((0x00455639 + 5 + 1), 0x90, 1);		//0x90->nop
	writeMemory((0x00455639 + 5 + 2), 0x90, 1);		//0x90->nop
	WriteProcessMemory(pid, NoPowerCallBase, &NoPower_Assemble, 256, &sizeX);

	//��д������
	BYTE reWrite[6] = { 0x0F,0x84 };		//je
	DWORD writadd = 0x00455643 - ((DWORD)NoPowerCallBase + 0x13) - 6;	//Ŀ��-��ַ-ָ���
	for (int i = 2; i<6; i++)
	{
		reWrite[i] = (BYTE)writadd;
		writadd >>= 8;
	}
	WriteProcessMemory(pid, (LPVOID)((int)NoPowerCallBase + 0x13), &reWrite, sizeof(reWrite), 0);

	jmpWritProc((LPVOID)((int)NoPowerCallBase + 0x1F), (LPVOID)(0x00455639 + 8));
}

void TrainerFunctions::FreeNoPower() //�ر�ͣ��
{
	getID();
	DWORD add_l = 0x00455639;
	BYTE jmp_l[8] = { 0x74,0x08,0x39,0xBE,0x7C,0x06,00,00 };
	DWORD writex = WriteProcessMemory(pid, (LPVOID)add_l, &jmp_l, sizeof(jmp_l), 0);
	::VirtualFreeEx(pid, NoPowerCallBase, 0, MEM_RELEASE);
	NoPowerCallBase = 0;
}


__declspec(naked) void TrainerFunctions::PutAsWill_Assemble()
{
	__asm
	{
		mov eax,1
		ret 0010h
	}
}

void TrainerFunctions::PutAsWill() //���⽨��
{
	DWORD sizeX;
	getID();        //get process HANDLE
	//PutCallBase = VirtualAllocEx(pid,NULL,0x256,MEM_COMMIT,PAGE_EXECUTE_READWRITE);
	virtualAE(&PutCallBase);        //VirtualAllocEx
	jmpWritProc((LPVOID)0x004A8EB0,PutCallBase); //jmp write address
	writeMemory((DWORD)PutCallBase,(DWORD)PutAsWill_Assemble,256);
	WriteProcessMemory(pid,PutCallBase,&PutAsWill_Assemble,256,&sizeX);
	jmpWritProc((LPVOID)((int)PutCallBase + 0x08),(LPVOID)(0x004A8EB0+5));
}

void TrainerFunctions::FreePutAsWill() //�ر����⽨��
{       
	getID();
	DWORD add_l = 0x004A8EB0;
	BYTE jmp_l[5] ={0xA1,0x4C,0x3D,0xA8,00};
	DWORD writex = WriteProcessMemory(pid,(LPVOID)add_l,&jmp_l,sizeof(jmp_l),0);
	::VirtualFreeEx(pid,PutCallBase,0,MEM_RELEASE);
	PutCallBase=0;
}
__declspec(naked) void TrainerFunctions::PutWaterAble_Assemble()
{
	__asm
	{
		mov eax,[esp+24h]
		push esi
		mov esi,00A83D4Ch
		cmp eax,[esi]
		pop esi
		jne originalcode
		mov ecx,03

		originalcode:
		cmp ecx,-01
//		jne 0047CA4Dh		//���ֵд���ԣ�ֱ����������д������
		nop					//ռλ
		nop
		nop
		nop
		nop
		nop
		//exit1:
	}
}

void TrainerFunctions::PutWaterAble() //ˮ�潨��
{
	DWORD sizeX;
	getID();        //get process HANDLE
	//PutCallBase = VirtualAllocEx(pid,NULL,0x256,MEM_COMMIT,PAGE_EXECUTE_READWRITE);
	virtualAE(&PutWaterCallBase);        //VirtualAllocEx
	jmpWritProc((LPVOID)0x0047C9D1,PutWaterCallBase); //jmp write address
	writeMemory((DWORD)PutWaterCallBase,(DWORD)PutWaterAble_Assemble,256);
	WriteProcessMemory(pid,PutWaterCallBase,&PutWaterAble_Assemble,256,&sizeX);

	//��д������
	BYTE reWrite[6] ={0x0F,0x85};		//jne
	DWORD writadd = 0x0047CA4D-((DWORD)PutWaterCallBase+0x17)-6;	//Ŀ��-��ַ-ָ���
	for(int i=2; i<6; i++)
	{
		reWrite[i] = (BYTE)writadd;
		writadd >>= 8;
	}
	WriteProcessMemory(pid,(LPVOID)((int)PutWaterCallBase+0x17),&reWrite,sizeof(reWrite),0);

	jmpWritProc((LPVOID)((int)PutWaterCallBase + 0x1D),(LPVOID)(0x0047C9D1+5));		//jmp 0047CA4Dh
}

void TrainerFunctions::FreePutWaterAble() //�ر�ˮ�潨��
{       
	getID();
	DWORD add_l = 0x0047C9D1;
	BYTE jmp_l[5] ={0x83,0XF9,0XFF,0X75,0X77};
	DWORD writex = WriteProcessMemory(pid,(LPVOID)add_l,&jmp_l,sizeof(jmp_l),0);
	::VirtualFreeEx(pid,PutWaterCallBase,0,MEM_RELEASE);
	PutWaterCallBase=0;
}


__declspec(naked) void TrainerFunctions::AutoRepair_Assemble()
{
	__asm
	{
		mov ecx,05
		cmp ecx,eax
//		jl 00450813h		//���ֵд���ԣ�ֱ����������д������
		nop					//ռλ
		nop
		nop
		nop
		nop
		nop
	}
}

void TrainerFunctions::AutoRepair() //�Զ�����
{
	DWORD sizeX;
	getID();        //get process HANDLE
	//PutCallBase = VirtualAllocEx(pid,NULL,0x256,MEM_COMMIT,PAGE_EXECUTE_READWRITE);
	virtualAE(&AtraCallBase);        //VirtualAllocEx
	jmpWritProc((LPVOID)0x00450651,AtraCallBase); //jmp write address
	writeMemory((0x00450651+5),0x90,1);		//0x90->nop
	writeMemory((0x00450651+5+1),0x90,1);		//0x90->nop
	writeMemory((0x00450651+5+2),0x90,1);		//0x90->nop
	writeMemory((DWORD)AtraCallBase,(DWORD)AutoRepair_Assemble,256);
	WriteProcessMemory(pid,AtraCallBase,&AutoRepair_Assemble,256,&sizeX);

	//��д������
	BYTE reWrite[6] ={0x0F,0x8C};		//jl
	DWORD writadd = 0x00450813-((DWORD)AtraCallBase+0x07)-6;	//Ŀ��-��ַ-ָ���
	for(int i=2; i<6; i++)
	{
		reWrite[i] = (BYTE)writadd;
		writadd >>= 8;
	}
	WriteProcessMemory(pid,(LPVOID)((int)AtraCallBase+0x07),&reWrite,sizeof(reWrite),0);

	jmpWritProc((LPVOID)((int)AtraCallBase + 0x0D),(LPVOID)(0x00450651+8));
}

void TrainerFunctions::FreeAutoRepair() //�ر��Զ�����
{       
	getID();
	DWORD add_l = 0x00450651;
	BYTE jmp_l[8] ={0x3B,0xC8,0x0F,0x8C,0xBA,0x01,00,00};
	DWORD writex = WriteProcessMemory(pid,(LPVOID)add_l,&jmp_l,sizeof(jmp_l),0);
	::VirtualFreeEx(pid,AtraCallBase,0,MEM_RELEASE);
	AtraCallBase=0;
}

__declspec(naked) void TrainerFunctions::AutoRepairStruc_Assemble()
{
	__asm
	{
		mov ecx,00A83D4Ch
		mov ecx,[ecx]
		mov edx,[esi+21Ch]
		cmp ecx,edx
		jne originalcode//��������ճ�
		mov cl,1//��Ҷ�����

		originalcode:
		test cl,cl
//		jne 00452652h		//���ֵд���ԣ�ֱ����������д������
		nop					//ռλ
		nop
		nop
		nop
		nop
		nop
		xor al,al
	}
}

void TrainerFunctions::AutoRepairStruc() //�������Զ�����
{
	DWORD sizeX;
	getID();        //get process HANDLE
	//PutCallBase = VirtualAllocEx(pid,NULL,0x256,MEM_COMMIT,PAGE_EXECUTE_READWRITE);
	virtualAE(&AtraStrCallBase);        //VirtualAllocEx
	jmpWritProc((LPVOID)0x0045264A,AtraStrCallBase); //jmp write address
	writeMemory((0x0045264A+5),0x90,1);		//0x90->nop
	writeMemory((DWORD)AtraStrCallBase,(DWORD)AutoRepairStruc_Assemble,256);
	WriteProcessMemory(pid,AtraStrCallBase,&AutoRepairStruc_Assemble,256,&sizeX);

	//��д������
	BYTE reWrite[6] ={0x0F,0x85};		//jne
	DWORD writadd = 0x00452652-((DWORD)AtraStrCallBase+0x15)-6;	//Ŀ��-��ַ-ָ���
	for(int i=2; i<6; i++)
	{
		reWrite[i] = (BYTE)writadd;
		writadd >>= 8;
	}
	WriteProcessMemory(pid,(LPVOID)((int)AtraStrCallBase+0x15),&reWrite,sizeof(reWrite),0);

	jmpWritProc((LPVOID)((int)AtraStrCallBase + 0x1D),(LPVOID)(0x0045264A+6));
}

void TrainerFunctions::FreeAutoRepairStruc() //�ر��������Զ�����
{       
	getID();
	DWORD add_l = 0x0045264A;
	BYTE jmp_l[6] ={0x84,0XC9,0X75,0X04,0X32,0XC0};
	DWORD writex = WriteProcessMemory(pid,(LPVOID)add_l,&jmp_l,sizeof(jmp_l),0);
	::VirtualFreeEx(pid,AtraStrCallBase,0,MEM_RELEASE);
	AtraStrCallBase=0;
}


__declspec(naked) void TrainerFunctions::RevengeYuri_Assemble()
{
	__asm
	{
		pushad
		mov ebx,00A83D4Ch
		cmp eax,[ebx]
		je originalcode//����ҾͲ���
		//�������bug: ���Ʒ���ִ�е�����ǰ�����ͻ����
		mov eax,0
//		mov ecx,ecx//��λ��ַ
		mov edx,00A8EC7Ch
		mov ebx,[edx]//���е�λ�����׵�ַ
		unitlist:
		mov edx,00A8EC88h
		cmp eax,[edx]//�������е�λ
		jge originalcode//>=û��or����
		cmp ecx,[ebx+eax*4]//����
		je cheat
		inc eax
		jmp unitlist

		cheat:
		popad
		mov ebx,00A83D4Ch
		mov eax,[ebx]
		push eax
		//mov ecx,esi//ecx�ǿ��Ʒ���ʶ
		mov ebp,[ecx]//��ȡ��λ����
		call dword ptr [ebp+000003D4h]
		mov ebx,00471DC2h
		jmp ebx//ֱ���˳�����������

		originalcode:
		popad
		mov eax,00A83D4Ch
		mov eax,[eax]
		push eax
		mov ecx,esi
		call dword ptr [ebp+000003D4h]
	}
}

void TrainerFunctions::RevengeYuri() //������
{
	DWORD sizeX;
	getID();        //get process HANDLE
	//PutCallBase = VirtualAllocEx(pid,NULL,0x256,MEM_COMMIT,PAGE_EXECUTE_READWRITE);
	virtualAE(&ReYuriCallBase);        //VirtualAllocEx
	jmpWritProc((LPVOID)0x00471DB5,ReYuriCallBase); //jmp write address
	writeMemory((0x00471DB5+5),0x90,1);		//0x90->nop
	writeMemory((0x00471DB5+5+1),0x90,1);		//0x90->nop
	writeMemory((0x00471DB5+5+2),0x90,1);		//0x90->nop
	writeMemory((0x00471DB5+5+3),0x90,1);		//0x90->nop
	writeMemory((DWORD)ReYuriCallBase,(DWORD)RevengeYuri_Assemble,256);
	WriteProcessMemory(pid,ReYuriCallBase,&RevengeYuri_Assemble,256,&sizeX);
	jmpWritProc((LPVOID)((int)ReYuriCallBase + 0x50),(LPVOID)(0x00471DB5+9));
}

void TrainerFunctions::FreeRevengeYuri() //�رշ�����
{       
	getID();
	DWORD add_l = 0x00471DB5;
	BYTE jmp_l[9] ={0x50,0x8B,0xCE,0xFF,0x95,0xD4,0x03,00,00};
	DWORD writex = WriteProcessMemory(pid,(LPVOID)add_l,&jmp_l,sizeof(jmp_l),0);
	::VirtualFreeEx(pid,ReYuriCallBase,0,MEM_RELEASE);
	ReYuriCallBase=0;
}


__declspec(naked) void TrainerFunctions::PermanentYuri_Assemble()
{
	__asm
	{
		mov ecx,00A83D4Ch
		mov eax,[ecx]
		push eax
		mov ecx,esi
		call dword ptr [edx+000003D4h]
	}
}

void TrainerFunctions::PermanentYuri() //���ÿ���
{
	DWORD sizeX;
	getID();        //get process HANDLE
	//PutCallBase = VirtualAllocEx(pid,NULL,0x256,MEM_COMMIT,PAGE_EXECUTE_READWRITE);
	virtualAE(&PerYuriCallBase);        //VirtualAllocEx
	jmpWritProc((LPVOID)0x004720D7,PerYuriCallBase); //jmp write address
	writeMemory((0x004720D7+5),0x90,1);		//0x90->nop
	writeMemory((0x004720D7+5+1),0x90,1);		//0x90->nop
	writeMemory((0x004720D7+5+2),0x90,1);		//0x90->nop
	writeMemory((0x004720D7+5+3),0x90,1);		//0x90->nop
	writeMemory((DWORD)PerYuriCallBase,(DWORD)PermanentYuri_Assemble,256);
	WriteProcessMemory(pid,PerYuriCallBase,&PermanentYuri_Assemble,256,&sizeX);
	jmpWritProc((LPVOID)((int)PerYuriCallBase + 0x10),(LPVOID)(0x004720D7+9));
}

void TrainerFunctions::FreePermanentYuri() //�ر����ÿ���
{       
	getID();
	DWORD add_l = 0x004720D7;
	BYTE jmp_l[9] ={0x50,0x8B,0xCE,0xFF,0x92,0xD4,0x03,00,00};
	DWORD writex = WriteProcessMemory(pid,(LPVOID)add_l,&jmp_l,sizeof(jmp_l),0);
	::VirtualFreeEx(pid,PerYuriCallBase,0,MEM_RELEASE);
	PerYuriCallBase=0;
}


__declspec(naked) void TrainerFunctions::MineAttack_Assemble()
{
	__asm
	{
		pushad
		mov eax,0
		mov ebx,[esi+0xB0]//��λ��ַ
		mov edx,00A8EC7Ch
		mov ecx,[edx]//���е�λ�����׵�ַ
		unitlist:
		mov edx,00A8EC88h
		cmp eax,[edx]//�������е�λ
		jge originalcode//>=û��or����
		cmp ebx,[ecx+eax*4]//����
		je cheat
		inc eax
		jmp unitlist

		cheat:
		push 0  
		mov edx,00A83D4Ch
		mov eax,[edx]
		push eax
		mov ecx,[esi+0xB0]
		mov ebx,[ecx]
		call [ebx+3D4h]
		jmp originalcode

		originalcode:
		popad
		mov eax,[ebp+08h]
		mov edx,[eax]
	}
}

void TrainerFunctions::MineAttack() //��������
{
	DWORD sizeX;
	getID();        //get process HANDLE
	//PutCallBase = VirtualAllocEx(pid,NULL,0x256,MEM_COMMIT,PAGE_EXECUTE_READWRITE);
	virtualAE(&MineAtkCallBase);        //VirtualAllocEx
	jmpWritProc((LPVOID)0x00469AA4,MineAtkCallBase); //jmp write address
	writeMemory((DWORD)MineAtkCallBase,(DWORD)MineAttack_Assemble,256);
	WriteProcessMemory(pid,MineAtkCallBase,&MineAttack_Assemble,256,&sizeX);
	jmpWritProc((LPVOID)((int)MineAtkCallBase + 0x44),(LPVOID)(0x00469AA4+5));
}

void TrainerFunctions::FreeMineAttack() //�رչ�������
{       
	getID();
	DWORD add_l = 0x00469AA4;
	BYTE jmp_l[5] ={0x8B,0x45,0x08,0x8B,0x10};
	DWORD writex = WriteProcessMemory(pid,(LPVOID)add_l,&jmp_l,sizeof(jmp_l),0);
	::VirtualFreeEx(pid,MineAtkCallBase,0,MEM_RELEASE);
	MineAtkCallBase=0;
}


__declspec(naked) void TrainerFunctions::MineBuildIn_Assemble()
{
	__asm
	{
		mov eax,00A83D4Ch
		mov eax,[eax]
		push eax
		call dword ptr [edx+000003D4h]
	}
}

void TrainerFunctions::MineBuildIn() //��������
{
	DWORD sizeX;
	getID();        //get process HANDLE
	//PutCallBase = VirtualAllocEx(pid,NULL,0x256,MEM_COMMIT,PAGE_EXECUTE_READWRITE);
	virtualAE(&MineBdiCallBase);        //VirtualAllocEx
	jmpWritProc((LPVOID)0x00458322,MineBdiCallBase); //jmp write address
	writeMemory((0x00458322+5),0x90,1);		//0x90->nop
	writeMemory((0x00458322+5+1),0x90,1);		//0x90->nop
	writeMemory((DWORD)MineBdiCallBase,(DWORD)MineBuildIn_Assemble,256);
	WriteProcessMemory(pid,MineBdiCallBase,&MineBuildIn_Assemble,256,&sizeX);
	jmpWritProc((LPVOID)((int)MineBdiCallBase + 0x0E),(LPVOID)(0x00458322+7));
}

void TrainerFunctions::FreeMineBuildIn() //�رս�������
{       
	getID();
	DWORD add_l = 0x00458322;
	BYTE jmp_l[7] ={0x50,0xFF,0x92,0xD4,0x03,00,00};
	DWORD writex = WriteProcessMemory(pid,(LPVOID)add_l,&jmp_l,sizeof(jmp_l),0);
	::VirtualFreeEx(pid,MineBdiCallBase,0,MEM_RELEASE);
	MineBdiCallBase=0;
}


__declspec(naked) void TrainerFunctions::MineUnEngineer_Assemble()
{
	__asm
	{
		mov eax,00A83D4Ch
		mov eax,[eax]
		push eax
		mov ecx,edi
		call dword ptr [edx+000003D4h]
	}
}

void TrainerFunctions::MineUnEngineer() //������ʦռ��
{
	DWORD sizeX;
	getID();        //get process HANDLE
	//PutCallBase = VirtualAllocEx(pid,NULL,0x256,MEM_COMMIT,PAGE_EXECUTE_READWRITE);
	virtualAE(&MineUneCallBase);        //VirtualAllocEx
	jmpWritProc((LPVOID)0x00519F7B,MineUneCallBase); //jmp write address
	writeMemory((0x00519F7B+5),0x90,1);		//0x90->nop
	writeMemory((0x00519F7B+5+1),0x90,1);		//0x90->nop
	writeMemory((0x00519F7B+5+2),0x90,1);		//0x90->nop
	writeMemory((0x00519F7B+5+3),0x90,1);		//0x90->nop
	writeMemory((DWORD)MineUneCallBase,(DWORD)MineUnEngineer_Assemble,256);
	WriteProcessMemory(pid,MineUneCallBase,&MineUnEngineer_Assemble,256,&sizeX);
	jmpWritProc((LPVOID)((int)MineUneCallBase + 0x10),(LPVOID)(0x00519F7B+9));
}

void TrainerFunctions::FreeMineUnEngineer() //�رշ�����ʦռ��
{       
	getID();
	DWORD add_l = 0x00519F7B;
	BYTE jmp_l[9] ={0x50,0x8B,0xCF,0xFF,0x92,0xD4,0x03,00,00};
	DWORD writex = WriteProcessMemory(pid,(LPVOID)add_l,&jmp_l,sizeof(jmp_l),0);
	::VirtualFreeEx(pid,MineUneCallBase,0,MEM_RELEASE);
	MineUneCallBase=0;
}


__declspec(naked) void TrainerFunctions::BuildImme_Assemble()
{
	__asm
	{
		push eax
		push ebx
		mov eax,00A83D4Ch
		mov eax,[eax]      //������ݵ�ַ
//		mov [eax+537Ch],1Eh    //��Ӫ����
//		mov [eax+5380h],14h    //ս����������
		mov ebx,dword ptr [eax+53ACh]//�ɻ�
		cmp ebx,esi
		jz cheat       //�����
		mov ebx,dword ptr [eax+53B8h]//��ֻ
		cmp ebx,esi
		jz cheat       //�����
		mov ebx,dword ptr [eax+53BCh]//����
		cmp ebx,esi
		jz cheat       //�����
		mov ebx,dword ptr [eax+53CCh]//����
		cmp ebx,esi
		jz cheat       //�����
		jmp originalcode

		cheat:
		mov [esi+24h],36h     //�޸ģ���
		jmp exit1

		originalcode:
		mov [esi+24h],edx

		exit1:
		pop ebx
		pop eax
		mov edx,[esp+10h]
	}
}

void TrainerFunctions::BuildImme() //˲�佨��
{
	DWORD sizeX;
	getID();        //get process HANDLE
	//PutCallBase = VirtualAllocEx(pid,NULL,0x256,MEM_COMMIT,PAGE_EXECUTE_READWRITE);
	virtualAE(&BdImeCallBase);        //VirtualAllocEx
	jmpWritProc((LPVOID)0x004C9B82,BdImeCallBase); //jmp write address
	writeMemory((0x004C9B82+5),0x90,1);		//0x90->nop
	writeMemory((0x004C9B82+5+1),0x90,1);		//0x90->nop
	writeMemory((DWORD)BdImeCallBase,(DWORD)BuildImme_Assemble,256);
	WriteProcessMemory(pid,BdImeCallBase,&BuildImme_Assemble,256,&sizeX);
	jmpWritProc((LPVOID)((int)BdImeCallBase + 0x42),(LPVOID)(0x004C9B82+7));
}

void TrainerFunctions::FreeBuildImme() //�ر�˲�佨��
{       
	getID();
	DWORD add_l = 0x004C9B82;
	BYTE jmp_l[7] ={0x89,0x56,0x24,0x8B,0x54,0x24,0x10};
	DWORD writex = WriteProcessMemory(pid,(LPVOID)add_l,&jmp_l,sizeof(jmp_l),0);
	::VirtualFreeEx(pid,BdImeCallBase,0,MEM_RELEASE);
	BdImeCallBase=0;
}


__declspec(naked) void TrainerFunctions::Unbeatable_Assemble()
{
	__asm
	{
		push ecx
		mov ecx,00A83D4Ch
		mov ecx,[ecx]			//������ݵ�ַ
		cmp ecx,[esi+21Ch]		//�ж����˺���λ�Ƿ��������
		pop ecx
		jnz originalcode       //��ת
		sub eax,0		//��Ҳ����˺�
		jmp exit1

		originalcode:
		sub eax,edx		//�з�����

		exit1:
		test eax,eax
		mov [esi+6Ch],eax
	}
}
__declspec(naked) void TrainerFunctions::SecKill_Assemble()
{
	__asm
	{
		push ecx
		mov ecx,00A83D4Ch
		mov ecx,[ecx]			//������ݵ�ַ
		cmp ecx,[esi+21Ch]		//�ж����˺���λ�Ƿ��������
		pop ecx
		jnz originalcode       //��ת
		sub eax,edx		//��Ҳ���
		jmp exit1

		originalcode:
		sub eax,eax		//�з�����ɱ

		exit1:
		test eax,eax
		mov [esi+6Ch],eax
	}
}
__declspec(naked) void TrainerFunctions::UnbeatableSecKill_Assemble()
{
	__asm
	{
		push ecx
		mov ecx,00A83D4Ch
		mov ecx,[ecx]			//������ݵ�ַ
		cmp ecx,[esi+21Ch]		//�ж����˺���λ�Ƿ��������
		pop ecx
		jnz originalcode       //��ת
		sub eax,0		//��Ҳ����˺�
		jmp exit1

		originalcode:
		sub eax,eax		//�з�����ɱ

		exit1:
		test eax,eax
		mov [esi+6Ch],eax
	}
}

void TrainerFunctions::UnbeatableSecKill_Pre() //�޵�/��ɱ�滻����
{
	getID();        //get process HANDLE
	//PutCallBase = VirtualAllocEx(pid,NULL,0x256,MEM_COMMIT,PAGE_EXECUTE_READWRITE);
	virtualAE(&UnbSKCallBase);        //VirtualAllocEx
	jmpWritProc((LPVOID)0x005F5509,UnbSKCallBase); //jmp write address
	writeMemory((0x005F5509+5),0x90,1);		//0x90->nop
	writeMemory((0x005F5509+5+1),0x90,1);		//0x90->nop
}
void TrainerFunctions::Unbeatable()//�޵�ʵ�ֺ���
{
	DWORD sizeX;
	writeMemory((DWORD)UnbSKCallBase,(DWORD)Unbeatable_Assemble,256);
	WriteProcessMemory(pid,UnbSKCallBase,&Unbeatable_Assemble,256,&sizeX);
	jmpWritProc((LPVOID)((int)UnbSKCallBase + 0x1D),(LPVOID)(0x005F5509+7));
}
void TrainerFunctions::SecKill()//��ɱʵ�ֺ���
{
	DWORD sizeX;
	writeMemory((DWORD)UnbSKCallBase,(DWORD)SecKill_Assemble,256);
	WriteProcessMemory(pid,UnbSKCallBase,&SecKill_Assemble,256,&sizeX);
	jmpWritProc((LPVOID)((int)UnbSKCallBase + 0x1C),(LPVOID)(0x005F5509+7));
}
void TrainerFunctions::UnbeatableSecKill()//�޵�+��ɱʵ�ֺ���
{
	DWORD sizeX;
	writeMemory((DWORD)UnbSKCallBase,(DWORD)UnbeatableSecKill_Assemble,256);
	WriteProcessMemory(pid,UnbSKCallBase,&UnbeatableSecKill_Assemble,256,&sizeX);
	jmpWritProc((LPVOID)((int)UnbSKCallBase + 0x1D),(LPVOID)(0x005F5509+7));
}

void TrainerFunctions::FreeUnbeatableSecKill() //�ر��޵�/�ر���ɱ
{       
	getID();
	DWORD add_l = 0x005F5509;
	BYTE jmp_l[7] ={0x2B,0xC2,0x85,0xC0,0x89,0x46,0x6C};
	DWORD writex = WriteProcessMemory(pid,(LPVOID)add_l,&jmp_l,sizeof(jmp_l),0);
	::VirtualFreeEx(pid,UnbSKCallBase,0,MEM_RELEASE);
	UnbSKCallBase = NULL;
}

__declspec(naked) void TrainerFunctions::AntiChrono_Assemble()
{
	__asm
	{
		push eax
		mov eax,[esi+24h]  //������ʱ�յ�λ
		mov eax,[eax+21Ch]
		push esi
		mov esi,00A83D4Ch
		cmp eax,[esi]
		pop esi
		jne exit1

//		originalcode:
		mov [esi+48h],ecx  //ʱ������ֵ

		exit1:
		pop eax
		cmp eax,ebx
	}
}
void TrainerFunctions::AntiChrono() //����ʱ��
{
	DWORD sizeX;
	getID();        //get process HANDLE
	//PutCallBase = VirtualAllocEx(pid,NULL,0x256,MEM_COMMIT,PAGE_EXECUTE_READWRITE);
	virtualAE(&AntiChronoCallBase);        //VirtualAllocEx
	jmpWritProc((LPVOID)0x0071A88A,AntiChronoCallBase); //jmp write address
	writeMemory((DWORD)AntiChronoCallBase,(DWORD)AntiChrono_Assemble,256);
	WriteProcessMemory(pid,AntiChronoCallBase,&AntiChrono_Assemble,256,&sizeX);
	jmpWritProc((LPVOID)((int)AntiChronoCallBase + 0x1B),(LPVOID)(0x0071A88A+5));
}
void TrainerFunctions::FreeAntiChrono() //�رշ���ʱ��
{       
	getID();
	DWORD add_l = 0x0071A88A;
	BYTE jmp_l[5] ={0x89,0x4E,0x48,0x3B,0xC3};
	DWORD writex = WriteProcessMemory(pid,(LPVOID)add_l,&jmp_l,sizeof(jmp_l),0);
	::VirtualFreeEx(pid,AntiChronoCallBase,0,MEM_RELEASE);
	AntiChronoCallBase=0;
}
__declspec(naked) void TrainerFunctions::AntiChronoDisbuild_Assemble()
{
	__asm
	{
		mov byte ptr [ecx+00000660h],01
		mov eax,[ecx+21Ch]
		push esi
		mov esi,00A83D4Ch
		cmp eax,[esi]
		pop esi
		je exit1
//		originalcode:
		mov byte ptr [ecx+00000660h],00

		exit1:
	}
}
void TrainerFunctions::AntiChronoDisbuild() //��ʱ�չ���������
{
	DWORD sizeX;
	getID();        //get process HANDLE
	//PutCallBase = VirtualAllocEx(pid,NULL,0x256,MEM_COMMIT,PAGE_EXECUTE_READWRITE);
	virtualAE(&AntChonoDbCallBase);        //VirtualAllocEx
	jmpWritProc((LPVOID)0x004521C0,AntChonoDbCallBase); //jmp write address
	writeMemory((0x004521C0+5),0x90,1);		//0x90->nop
	writeMemory((0x004521C0+5+1),0x90,1);		//0x90->nop
	writeMemory((DWORD)AntChonoDbCallBase,(DWORD)AntiChronoDisbuild_Assemble,256);
	WriteProcessMemory(pid,AntChonoDbCallBase,&AntiChronoDisbuild_Assemble,256,&sizeX);
	jmpWritProc((LPVOID)((int)AntChonoDbCallBase + 0x1F),(LPVOID)(0x004521C0+7));
}
void TrainerFunctions::FreeAntiChronoDisbuild() //�رճ�ʱ�չ���������
{       
	getID();
	DWORD add_l = 0x004521C0;
	BYTE jmp_l[7] ={0xC6,0x81,0x60,0x06,0x00,0x00,0x00};
	DWORD writex = WriteProcessMemory(pid,(LPVOID)add_l,&jmp_l,sizeof(jmp_l),0);
	::VirtualFreeEx(pid,AntChonoDbCallBase,0,MEM_RELEASE);
	AntChonoDbCallBase=0;
}
void TrainerFunctions::AntiChronoDisable() //��ʱ�չ��̽����������ɫ
{
	getID();        //get process HANDLE
	writeMemory((0x006A97B2  ),0x90,1);		//0x90->nop
	writeMemory((0x006A97B2+1),0x90,1);		//0x90->nop
}
void TrainerFunctions::FreeAntiChronoDisable() //�رճ�ʱ�չ��̽����������ɫ
{       
	getID();
	DWORD add_l = 0x006A97B2;
	BYTE jmp_l[2] ={0x74,0X47};
	DWORD writex = WriteProcessMemory(pid,(LPVOID)add_l,&jmp_l,sizeof(jmp_l),0);
}


__declspec(naked) void TrainerFunctions::UnRepair_Assemble()
{
	__asm
	{
		push eax
		mov eax,00A83D4Ch
		mov eax,[eax]
		cmp eax,[esi+21Ch]
		pop eax
		jz originalcode		//��Ҳ���
		sub ebx,ebp			//�з���Ѫ
		cmp ebx,0		 //����Ѫebx>0
		jge exit1
		mov ebx,0		//ûѪ�͹���
		jmp exit1

		originalcode:
		add ebx,ebp

		exit1:
		add edi,ebp
		mov [esi+6Ch],ebx
	}
}

void TrainerFunctions::UnRepair() //����
{
	DWORD sizeX;
	getID();        //get process HANDLE
	//PutCallBase = VirtualAllocEx(pid,NULL,0x256,MEM_COMMIT,PAGE_EXECUTE_READWRITE);
	virtualAE(&UnRaCallBase);        //VirtualAllocEx
	jmpWritProc((LPVOID)0x004508B4,UnRaCallBase); //jmp write address
	writeMemory((0x004508B4+5),0x90,1);		//0x90->nop
	writeMemory((0x004508B4+5+1),0x90,1);		//0x90->nop
	writeMemory((DWORD)UnRaCallBase,(DWORD)UnRepair_Assemble,256);
	WriteProcessMemory(pid,UnRaCallBase,&UnRepair_Assemble,256,&sizeX);
	jmpWritProc((LPVOID)((int)UnRaCallBase + 0x26),(LPVOID)(0x004508B4+7));
}

void TrainerFunctions::FreeUnRepair() //�رշ���
{       
	getID();
	DWORD add_l = 0x004508B4;
	BYTE jmp_l[7] ={0x01,0xEB,0x01,0xEF,0x89,0x5E,0x6C};
	DWORD writex = WriteProcessMemory(pid,(LPVOID)add_l,&jmp_l,sizeof(jmp_l),0);
	::VirtualFreeEx(pid,UnRaCallBase,0,MEM_RELEASE);
	UnRaCallBase=0;
}


void TrainerFunctions::UnRefreshCon() //��ɾ������ѡ��-����Ҫע�룬ֱ���滻
{
	getID();        //get process HANDLE
	jmpWritProc((LPVOID)0x006AA788,(LPVOID)0x006AAA68); //jmp 006AAA68
	writeMemory((0x006AA788+5),0x90,1);		//0x90->nop
}

void TrainerFunctions::FreeUnRefreshCon() //�رղ�ɾ������ѡ��
{       
	getID();
	DWORD add_l = 0x006AA788;
	BYTE jmp_l[6] ={0x0F,0x85,0xDA,0x02,00,00};
	DWORD writex = WriteProcessMemory(pid,(LPVOID)add_l,&jmp_l,sizeof(jmp_l),0);
}

__declspec(naked) void TrainerFunctions::AllBuild_Assemble()
{
	__asm
	{
		mov ecx,[eax+esi*4]
		mov eax,[ecx+634h]
		cmp eax,-1
//		jmp 0044575Fh	//������дjmp
	}
}

void TrainerFunctions::AllBuild() //����ȫ�Ƽ�
{
	DWORD sizeX;
	getID();        //get process HANDLE
	//PutCallBase = VirtualAllocEx(pid,NULL,0x256,MEM_COMMIT,PAGE_EXECUTE_READWRITE);
	virtualAE(&AllBCallBase);        //VirtualAllocEx
	jmpWritProc((LPVOID)0x0044574A,AllBCallBase); //jmp write address
	writeMemory((0x0044574A+5),0x90,1);		//0x90->nop
	writeMemory((0x0044574A+5+1),0x90,1);		//0x90->nop
	writeMemory((DWORD)AllBCallBase,(DWORD)AllBuild_Assemble,256);
	WriteProcessMemory(pid,AllBCallBase,&AllBuild_Assemble,256,&sizeX);
	jmpWritProc((LPVOID)((int)AllBCallBase + 0x0C),(LPVOID)0x0044575F);		//jmp 0044575F
}

void TrainerFunctions::FreeAllBuild() //�رս���ȫ�Ƽ�
{       
	getID();
	DWORD add_l = 0x0044574A;
	BYTE jmp_l[7] ={0x6A,0x01,0x6A,0x00,0x8B,0x0C,0xB0};
	DWORD writex = WriteProcessMemory(pid,(LPVOID)add_l,&jmp_l,sizeof(jmp_l),0);
	::VirtualFreeEx(pid,AllBCallBase,0,MEM_RELEASE);
	AllBCallBase=0;
}

__declspec(naked) void TrainerFunctions::AllCarBoat_Assemble()
{
	__asm
	{
		mov eax,[edx+esi*4]
		mov eax,[eax+634h]
		cmp eax,-1
//		jmp 004457A4h	//������дjmp
	}
}

void TrainerFunctions::AllCarBoat() //����ȫ�Ƽ�
{
	DWORD sizeX;
	getID();        //get process HANDLE
	//PutCallBase = VirtualAllocEx(pid,NULL,0x256,MEM_COMMIT,PAGE_EXECUTE_READWRITE);
	virtualAE(&AllCCallBase);        //VirtualAllocEx
	jmpWritProc((LPVOID)0x00445795,AllCCallBase); //jmp write address
	writeMemory((0x00445795+5),0x90,1);		//0x90->nop
	writeMemory((0x00445795+5+1),0x90,1);		//0x90->nop
	writeMemory((DWORD)AllCCallBase,(DWORD)AllCarBoat_Assemble,256);
	WriteProcessMemory(pid,AllCCallBase,&AllCarBoat_Assemble,256,&sizeX);
	jmpWritProc((LPVOID)((int)AllCCallBase + 0x0C),(LPVOID)0x004457A4);		//jmp 004457A4
}

void TrainerFunctions::FreeAllCarBoat() //�رճ���ȫ�Ƽ�
{       
	getID();
	DWORD add_l = 0x00445795;
	BYTE jmp_l[7] ={0x6A,0x01,0x6A,0x00,0x8B,0x04,0xB2};
	DWORD writex = WriteProcessMemory(pid,(LPVOID)add_l,&jmp_l,sizeof(jmp_l),0);
	::VirtualFreeEx(pid,AllCCallBase,0,MEM_RELEASE);
	AllCCallBase=0;
}

__declspec(naked) void TrainerFunctions::AllSoldier_Assemble()
{
	__asm
	{
		mov ecx,[ecx+esi*4]
		mov eax,[ecx+634h]
		cmp eax,-1
//		jmp 004457E5h	//������дjmp
	}
}

void TrainerFunctions::AllSoldier() //����ȫ�Ƽ�
{
	DWORD sizeX;
	getID();        //get process HANDLE
	//PutCallBase = VirtualAllocEx(pid,NULL,0x256,MEM_COMMIT,PAGE_EXECUTE_READWRITE);
	virtualAE(&AllSCallBase);        //VirtualAllocEx
	jmpWritProc((LPVOID)0x004457D0,AllSCallBase); //jmp write address
	writeMemory((0x004457D0+5),0x90,1);		//0x90->nop
	writeMemory((0x004457D0+5+1),0x90,1);		//0x90->nop
	writeMemory((DWORD)AllSCallBase,(DWORD)AllSoldier_Assemble,256);
	WriteProcessMemory(pid,AllSCallBase,&AllSoldier_Assemble,256,&sizeX);
	jmpWritProc((LPVOID)((int)AllSCallBase + 0x0C),(LPVOID)0x004457E5);		//jmp 004457E5
}

void TrainerFunctions::FreeAllSoldier() //�رղ���ȫ�Ƽ�
{       
	getID();
	DWORD add_l = 0x004457D0;
	BYTE jmp_l[7] ={0x6A,0x01,0x6A,0x00,0x8B,0x14,0xB1};
	DWORD writex = WriteProcessMemory(pid,(LPVOID)add_l,&jmp_l,sizeof(jmp_l),0);
	::VirtualFreeEx(pid,AllSCallBase,0,MEM_RELEASE);
	AllSCallBase=0;
}

__declspec(naked) void TrainerFunctions::AllPlane_Assemble()
{
	__asm
	{
		mov ecx,[eax+esi*4]
		mov eax,[ecx+634h]
		cmp eax,-1
//		jmp 00445825h
	}
}

void TrainerFunctions::AllPlane() //�ɻ�ȫ�Ƽ�
{
	DWORD sizeX;
	getID();        //get process HANDLE
	//PutCallBase = VirtualAllocEx(pid,NULL,0x256,MEM_COMMIT,PAGE_EXECUTE_READWRITE);
	virtualAE(&AllPCallBase);        //VirtualAllocEx
	jmpWritProc((LPVOID)0x00445810,AllPCallBase); //jmp write address
	writeMemory((0x00445810+5),0x90,1);		//0x90->nop
	writeMemory((0x00445810+5+1),0x90,1);		//0x90->nop
	writeMemory((DWORD)AllPCallBase,(DWORD)AllPlane_Assemble,256);
	WriteProcessMemory(pid,AllPCallBase,&AllPlane_Assemble,256,&sizeX);
	jmpWritProc((LPVOID)((int)AllPCallBase + 0x0C),(LPVOID)0x00445825);
}

void TrainerFunctions::FreeAllPlane() //�رշɻ�ȫ�Ƽ�
{       
	getID();
	DWORD add_l = 0x00445810;
	BYTE jmp_l[7] ={0x6A,0x01,0x6A,0x00,0x8B,0x0C,0xB0};
	DWORD writex = WriteProcessMemory(pid,(LPVOID)add_l,&jmp_l,sizeof(jmp_l),0);
	::VirtualFreeEx(pid,AllPCallBase,0,MEM_RELEASE);
	AllPCallBase=0;
}


__declspec(naked) void TrainerFunctions::FastAttack_Assemble()
{
	__asm
	{
		mov eax,[esi+21Ch]
		push ebx
		mov ebx,00A83D4Ch
		cmp eax,[ebx]
		pop ebx
		jne originalcode
		mov eax,006FC981h
		jmp eax      //����ʱ����㣬ǿ����һ�ι���

		originalcode:
		mov eax,[esi+2F4h]
	}
}

void TrainerFunctions::FastAttack() //���ٹ���
{
	DWORD sizeX;
	getID();        //get process HANDLE
	//PutCallBase = VirtualAllocEx(pid,NULL,0x256,MEM_COMMIT,PAGE_EXECUTE_READWRITE);
	virtualAE(&FstAtkCallBase);        //VirtualAllocEx
	jmpWritProc((LPVOID)0x006FC955,FstAtkCallBase); //jmp write address
	writeMemory((0x006FC955+5),0x90,1);		//0x90->nop
	writeMemory((DWORD)FstAtkCallBase,(DWORD)FastAttack_Assemble,256);
	WriteProcessMemory(pid,FstAtkCallBase,&FastAttack_Assemble,256,&sizeX);
	jmpWritProc((LPVOID)((int)FstAtkCallBase + 0x1E),(LPVOID)(0x006FC955+6));
}

void TrainerFunctions::FreeFastAttack() //�رռ��ٹ���
{       
	getID();
	DWORD add_l = 0x006FC955;
	BYTE jmp_l[6] ={0x8B,0x86,0xF4,0x02,00,00};
	DWORD writex = WriteProcessMemory(pid,(LPVOID)add_l,&jmp_l,sizeof(jmp_l),0);
	::VirtualFreeEx(pid,FstAtkCallBase,0,MEM_RELEASE);
	FstAtkCallBase=0;
}


__declspec(naked) void TrainerFunctions::FastTurnBattery_Assemble()
{
	__asm
	{
		pushad
		mov eax,00A8EC7Ch
		mov ebx,[eax]//���е�λ�����׵�ַ
		mov eax,0
		unitlist:
		mov edx,00A8EC88h
		cmp eax,[edx]//�������е�λ
		jge exit1//>=û��or����
		cmp ebp,[ebx+eax*4]//����
		je cheat
		inc eax
		jmp unitlist

		cheat:
		mov edx,[ebp+21Ch]
		mov eax,00A83D4Ch
		cmp edx,[eax]
		jne exit1
		mov [ecx+08h],0

		exit1:
		popad
		mov edx,[ecx+10h]
		mov ecx,[ecx+08h]
	}
}

void TrainerFunctions::FastTurnBattery() //����ת����
{
	DWORD sizeX;
	getID();        //get process HANDLE
	//PutCallBase = VirtualAllocEx(pid,NULL,0x256,MEM_COMMIT,PAGE_EXECUTE_READWRITE);
	virtualAE(&FstTurnBtCallBase);        //VirtualAllocEx
	jmpWritProc((LPVOID)0x004C941E,FstTurnBtCallBase); //jmp write address
	writeMemory((0x004C941E+5),0x90,1);		//0x90->nop
	writeMemory((DWORD)FstTurnBtCallBase,(DWORD)FastTurnBattery_Assemble,256);
	WriteProcessMemory(pid,FstTurnBtCallBase,&FastTurnBattery_Assemble,256,&sizeX);
	jmpWritProc((LPVOID)((int)FstTurnBtCallBase + 0x38),(LPVOID)(0x004C941E+6));
}

void TrainerFunctions::FreeFastTurnBattery() //�رռ���ת����
{       
	getID();
	DWORD add_l = 0x004C941E;
	BYTE jmp_l[6] ={0x8B,0x51,0x10,0x8B,0x49,0x08};
	DWORD writex = WriteProcessMemory(pid,(LPVOID)add_l,&jmp_l,sizeof(jmp_l),0);
	::VirtualFreeEx(pid,FstTurnBtCallBase,0,MEM_RELEASE);
	FstTurnBtCallBase=0;
}


__declspec(naked) void TrainerFunctions::FastTurnRound_Assemble()
{
	__asm
	{
		lea esi,[ecx-388h]
		mov edi,[esi+21Ch]
		mov esi,00A83D4Ch
		cmp edi,[esi]
//		je 004C92EBh		//���ֵд���ԣ�ֱ����������д������
		nop					//ռλ
		nop
		nop
		nop
		nop
		nop
		test bp,bp

//		originalcode:
//		jle 004C92EBh		//���ֵд���ԣ�ֱ����������д������
		nop					//ռλ
		nop
		nop
		nop
		nop
		nop
		mov ax,dx
	}
}

void TrainerFunctions::FastTurnRound() //����ת��
{
	DWORD sizeX;
	getID();        //get process HANDLE
	//PutCallBase = VirtualAllocEx(pid,NULL,0x256,MEM_COMMIT,PAGE_EXECUTE_READWRITE);
	virtualAE(&FstTurnRdCallBase);        //VirtualAllocEx
	jmpWritProc((LPVOID)0x004C92BF,FstTurnRdCallBase); //jmp write address
	writeMemory((DWORD)FstTurnRdCallBase,(DWORD)FastTurnRound_Assemble,256);
	WriteProcessMemory(pid,FstTurnRdCallBase,&FastTurnRound_Assemble,256,&sizeX);

	//��д������
	BYTE reWrite[6] = { 0x0F,0x84 };		//je
	DWORD writadd = 0x004C92EB - ((DWORD)FstTurnRdCallBase + 0x13) - 6;	//Ŀ��-��ַ-ָ���
	for (int i = 2; i<6; i++)
	{
		reWrite[i] = (BYTE)writadd;
		writadd >>= 8;
	}
	WriteProcessMemory(pid, (LPVOID)((int)FstTurnRdCallBase + 0x13), &reWrite, sizeof(reWrite), 0);

	*((LPWORD)reWrite) = 0x8E0F;		//jle(ע��x86��С��ģʽ)
	writadd = 0x004C92EB - ((DWORD)FstTurnRdCallBase + 0x1C) - 6;	//Ŀ��-��ַ-ָ���
	for (int i = 2; i<6; i++)
	{
		reWrite[i] = (BYTE)writadd;
		writadd >>= 8;
	}
	WriteProcessMemory(pid, (LPVOID)((int)FstTurnRdCallBase + 0x1C), &reWrite, sizeof(reWrite), 0);

	jmpWritProc((LPVOID)((int)FstTurnRdCallBase + 0x25),(LPVOID)(0x004C92BF+5));
}

void TrainerFunctions::FreeFastTurnRound() //�رռ���ת��
{       
	getID();
	DWORD add_l = 0x004C92BF;
	BYTE jmp_l[5] ={0x7E,0x2A,0x66,0x8B,0xC2};
	DWORD writex = WriteProcessMemory(pid,(LPVOID)add_l,&jmp_l,sizeof(jmp_l),0);
	::VirtualFreeEx(pid,FstTurnRdCallBase,0,MEM_RELEASE);
	FstTurnRdCallBase=0;
}


__declspec(naked) void TrainerFunctions::FastReload_Assemble()
{
	__asm
	{
		mov ecx,00A83D4Ch
		mov ecx,[ecx]
		cmp ecx,[esi+21Ch]
		jne originalcode
		mov eax,05h
		jmp exit1

		originalcode:
		mov eax,[eax+00000D60h]//��ҩ�������

		exit1:
	}
}

void TrainerFunctions::FastReload() //������װ
{
	DWORD sizeX;
	getID();        //get process HANDLE
	//PutCallBase = VirtualAllocEx(pid,NULL,0x256,MEM_COMMIT,PAGE_EXECUTE_READWRITE);
	virtualAE(&FstRldCallBase);        //VirtualAllocEx
	jmpWritProc((LPVOID)0x006F3FC8,FstRldCallBase); //jmp write address
	writeMemory((0x006F3FC8+5),0x90,1);		//0x90->nop
	writeMemory((DWORD)FstRldCallBase,(DWORD)FastReload_Assemble,256);
	WriteProcessMemory(pid,FstRldCallBase,&FastReload_Assemble,256,&sizeX);
	jmpWritProc((LPVOID)((int)FstRldCallBase + 0x1C),(LPVOID)(0x006F3FC8+6));
}

void TrainerFunctions::FreeFastReload() //�ر�������װ
{       
	getID();
	DWORD add_l = 0x006F3FC8;
	BYTE jmp_l[6] ={0x8B,0x80,0x60,0x0D,00,00};
	DWORD writex = WriteProcessMemory(pid,(LPVOID)add_l,&jmp_l,sizeof(jmp_l),0);
	::VirtualFreeEx(pid,FstRldCallBase,0,MEM_RELEASE);
	FstRldCallBase=0;
}


__declspec(naked) void TrainerFunctions::FullAmmunition_Assemble()
{
	__asm
	{
		mov ecx,00A83D4Ch
		mov ecx,[ecx]
		cmp ecx,[esi+21Ch]
		jne originalcode
		mov eax,14h		//20
		jmp exit1

		originalcode:
		mov eax,[eax+00000D5Ch]		//��ҩ��

		exit1:
	}
}

void TrainerFunctions::FullAmmunition() //��ҩ����
{
	DWORD sizeX;
	getID();        //get process HANDLE
	//PutCallBase = VirtualAllocEx(pid,NULL,0x256,MEM_COMMIT,PAGE_EXECUTE_READWRITE);
	virtualAE(&FulAmuCallBase);        //VirtualAllocEx
	jmpWritProc((LPVOID)0x006F3FD9,FulAmuCallBase); //jmp write address
	writeMemory((0x006F3FD9+5),0x90,1);		//0x90->nop
	writeMemory((DWORD)FulAmuCallBase,(DWORD)FullAmmunition_Assemble,256);
	WriteProcessMemory(pid,FulAmuCallBase,&FullAmmunition_Assemble,256,&sizeX);
	jmpWritProc((LPVOID)((int)FulAmuCallBase + 0x1C),(LPVOID)(0x006F3FD9+6));
}

void TrainerFunctions::FreeFullAmmunition() //�رյ�ҩ����
{       
	getID();
	DWORD add_l = 0x006F3FD9;
	BYTE jmp_l[6] ={0x8B,0x80,0x5C,0x0D,00,00};
	DWORD writex = WriteProcessMemory(pid,(LPVOID)add_l,&jmp_l,sizeof(jmp_l),0);
	::VirtualFreeEx(pid,FulAmuCallBase,0,MEM_RELEASE);
	FulAmuCallBase=0;
}


__declspec(naked) void TrainerFunctions::AllRangeAttack_Assemble()
{
	__asm
	{
		push ecx
		mov ecx,00A83D4Ch
		mov ecx,[ecx]
		cmp ecx,[esi+21Ch]
		pop ecx
		jne originalcode
		mov edi,0xF900		//������ΧF9 249
		jmp exit1

		originalcode:
		mov edi,[ebx+000000B4h]

		exit1:
	}
}

void TrainerFunctions::AllRangeAttack() //Զ�̴��
{
	DWORD sizeX;
	getID();        //get process HANDLE
	//PutCallBase = VirtualAllocEx(pid,NULL,0x256,MEM_COMMIT,PAGE_EXECUTE_READWRITE);
	virtualAE(&ARgeAtkCallBase);        //VirtualAllocEx
	jmpWritProc((LPVOID)0x006F7248,ARgeAtkCallBase); //jmp write address
	writeMemory((0x006F7248+5),0x90,1);		//0x90->nop
	writeMemory((DWORD)ARgeAtkCallBase,(DWORD)AllRangeAttack_Assemble,256);
	WriteProcessMemory(pid,ARgeAtkCallBase,&AllRangeAttack_Assemble,256,&sizeX);
	jmpWritProc((LPVOID)((int)ARgeAtkCallBase + 0x1E),(LPVOID)(0x006F7248+6));
}

void TrainerFunctions::FreeAllRangeAttack() //�ر�Զ�̴��
{       
	getID();
	DWORD add_l = 0x006F7248;
	BYTE jmp_l[6] ={0x8B,0xBB,0xB4,00,00,00};
	DWORD writex = WriteProcessMemory(pid,(LPVOID)add_l,&jmp_l,sizeof(jmp_l),0);
	::VirtualFreeEx(pid,ARgeAtkCallBase,0,MEM_RELEASE);
	ARgeAtkCallBase=0;
}


__declspec(naked) void TrainerFunctions::AllRangeAlert_Assemble()
{
	__asm
	{
		mov ecx,00A83D4Ch
		mov ecx,[ecx]
		cmp ecx,[esi+21Ch]
		jne originalcode
		mov ebx,0xF900		//���䷶ΧF9 249
		jmp exit1

		originalcode:
		mov ebx,[eax+000000B4h]

		exit1:
	}
}

void TrainerFunctions::AllRangeAlert() //Զ�̾���
{
	DWORD sizeX;
	getID();        //get process HANDLE
	//PutCallBase = VirtualAllocEx(pid,NULL,0x256,MEM_COMMIT,PAGE_EXECUTE_READWRITE);
	virtualAE(&ARgeAleCallBase);        //VirtualAllocEx
	jmpWritProc((LPVOID)0x007012DF,ARgeAleCallBase); //jmp write address
	writeMemory((0x007012DF+5),0x90,1);		//0x90->nop
	writeMemory((DWORD)ARgeAleCallBase,(DWORD)AllRangeAlert_Assemble,256);
	WriteProcessMemory(pid,ARgeAleCallBase,&AllRangeAlert_Assemble,256,&sizeX);
	jmpWritProc((LPVOID)((int)ARgeAleCallBase + 0x1C),(LPVOID)(0x007012DF+6));
}

void TrainerFunctions::FreeAllRangeAlert() //�ر�Զ�̾���
{       
	getID();
	DWORD add_l = 0x007012DF;
	BYTE jmp_l[6] ={0x8B,0x98,0xB4,00,00,00};
	DWORD writex = WriteProcessMemory(pid,(LPVOID)add_l,&jmp_l,sizeof(jmp_l),0);
	::VirtualFreeEx(pid,ARgeAleCallBase,0,MEM_RELEASE);
	ARgeAleCallBase=0;
}


__declspec(naked) void TrainerFunctions::InstantChronoMove_Assemble()
{
	__asm
	{
		mov edx,[ecx+21Ch]
		push esi
		mov esi,00A83D4Ch
		cmp edx,[esi]
		pop esi
		jne originalcode
		mov byte ptr [ecx+00000271h],00
		jmp exit1

		originalcode:
		mov byte ptr [ecx+00000271h],01

		exit1:
	}
}

void TrainerFunctions::InstantChronoMove() //˲�䳬ʱ���ƶ�
{
	DWORD sizeX;
	getID();        //get process HANDLE
	//PutCallBase = VirtualAllocEx(pid,NULL,0x256,MEM_COMMIT,PAGE_EXECUTE_READWRITE);
	virtualAE(&InsChoMovCallBase);        //VirtualAllocEx
	jmpWritProc((LPVOID)0x00719579,InsChoMovCallBase); //jmp write address
	writeMemory((0x00719579+5),0x90,1);		//0x90->nop
	writeMemory((0x00719579+6),0x90,1);		//0x90->nop
	writeMemory((DWORD)InsChoMovCallBase,(DWORD)InstantChronoMove_Assemble,256);
	WriteProcessMemory(pid,InsChoMovCallBase,&InstantChronoMove_Assemble,256,&sizeX);
	jmpWritProc((LPVOID)((int)InsChoMovCallBase + 0x21),(LPVOID)(0x00719579+6));
}

void TrainerFunctions::FreeInstantChronoMove() //˲�䳬ʱ���ƶ�
{       
	getID();
	DWORD add_l = 0x00719579;
	BYTE jmp_l[7] ={0xC6,0x81,0x71,0x02,0x00,0x00,0x01};
	DWORD writex = WriteProcessMemory(pid,(LPVOID)add_l,&jmp_l,sizeof(jmp_l),0);
	::VirtualFreeEx(pid,InsChoMovCallBase,0,MEM_RELEASE);
	InsChoMovCallBase=0;
}

__declspec(naked) void TrainerFunctions::InstantChronoAttack_Assemble()
{
	__asm
	{
		push eax
		push ebx
		mov eax,[esi+24h]//����������λָ��
		mov eax,[eax+21Ch]
		mov ebx,00A83D4Ch
		cmp eax,[ebx]
		pop ebx
		pop eax
		jne originalcode
		mov [esi+48h],0
		jmp exit1

		originalcode:
		mov [esi+48h],eax

		exit1:
//		je 0071B064h		//���ֵд���ԣ�ֱ����������д������
		nop					//ռλ
		nop
		nop
		nop
		nop
		nop
	}
}

void TrainerFunctions::InstantChronoAttack() //˲�䳬ʱ�չ���
{
	DWORD sizeX;
	getID();        //get process HANDLE
	//PutCallBase = VirtualAllocEx(pid,NULL,0x256,MEM_COMMIT,PAGE_EXECUTE_READWRITE);
	virtualAE(&InsChoAtkCallBase);        //VirtualAllocEx
	jmpWritProc((LPVOID)0x0071AFB9,InsChoAtkCallBase); //jmp write address
	writeMemory((0x0071AFB9+5),0x90,1);		//0x90->nop
	writeMemory((0x0071AFB9+5+1),0x90,1);		//0x90->nop
	writeMemory((0x0071AFB9+5+2),0x90,1);		//0x90->nop
	writeMemory((0x0071AFB9+5+3),0x90,1);		//0x90->nop
	writeMemory((DWORD)InsChoAtkCallBase,(DWORD)InstantChronoAttack_Assemble,256);
	WriteProcessMemory(pid,InsChoAtkCallBase,&InstantChronoAttack_Assemble,256,&sizeX);

	//��д������
	BYTE reWrite[6] = { 0x0F,0x84 };		//je
	DWORD writadd = 0x0071B064 - ((DWORD)InsChoAtkCallBase + 0x1F) - 6;	//Ŀ��-��ַ-ָ���
	for (int i = 2; i<6; i++)
	{
		reWrite[i] = (BYTE)writadd;
		writadd >>= 8;
	}
	WriteProcessMemory(pid, (LPVOID)((int)InsChoAtkCallBase + 0x1F), &reWrite, sizeof(reWrite), 0);

	jmpWritProc((LPVOID)((int)InsChoAtkCallBase + 0x25),(LPVOID)(0x0071AFB9+9));
}

void TrainerFunctions::FreeInstantChronoAttack() //˲�䳬ʱ�չ���
{       
	getID();
	DWORD add_l = 0x0071AFB9;
	BYTE jmp_l[9] ={0x89,0x46,0x48,0x0F,0x84,0xA2,00,00,00};
	DWORD writex = WriteProcessMemory(pid,(LPVOID)add_l,&jmp_l,sizeof(jmp_l),0);
	::VirtualFreeEx(pid,InsChoAtkCallBase,0,MEM_RELEASE);
	InsChoAtkCallBase=0;
}


__declspec(naked) void TrainerFunctions::AntiSpy_Assemble()
{
	__asm
	{
		push edi
		mov edi,00A83D4Ch
		cmp eax,[edi]
		pop edi
		je originalcode

		mov ebx,[edi+21Ch]//�������ָ��
		mov [edi+21Ch],eax//��ʱת���з�
		push edi
		mov edi,00A83D4Ch
		mov eax,[edi]//���ָ��Ϊ���
		pop edi
		push eax
//		call 004571E0h		//�޷���ȷд�룬ֱ����������д������
		nop
		nop
		nop
		nop
		nop
		mov [edi+21Ch],ebx//������ԭ
		jmp exit1

		originalcode:
		push eax
//		call 004571E0h		//�޷���ȷд�룬ֱ����������д������
		nop
		nop
		nop
		nop
		nop

		exit1:
	}
}

void TrainerFunctions::AntiSpy() //����
{
	DWORD sizeX;
	getID();        //get process HANDLE
	//PutCallBase = VirtualAllocEx(pid,NULL,0x256,MEM_COMMIT,PAGE_EXECUTE_READWRITE);
	virtualAE(&AntiSpyCallBase);        //VirtualAllocEx
	jmpWritProc((LPVOID)0x0051A00A,AntiSpyCallBase); //jmp write address
	writeMemory((0x0051A00A+5),0x90,1);		//0x90->nop
	writeMemory((DWORD)AntiSpyCallBase,(DWORD)AntiSpy_Assemble,256);
	WriteProcessMemory(pid,AntiSpyCallBase,&AntiSpy_Assemble,256,&sizeX);

	//��д������
	BYTE reWrite[5] = { 0xE8 };		//call
	DWORD writadd = 0x004571E0 - ((DWORD)AntiSpyCallBase + 0x21) - 5;	//Ŀ��-��ַ-ָ���
	for (int i = 1; i<5; i++)
	{
		reWrite[i] = (BYTE)writadd;
		writadd >>= 8;
	}
	WriteProcessMemory(pid, (LPVOID)((int)AntiSpyCallBase + 0x21), &reWrite, sizeof(reWrite), 0);

//	BYTE reWrite[5] = { 0xE8 };		//call
	writadd = 0x004571E0 - ((DWORD)AntiSpyCallBase + 0x2F) - 5;	//Ŀ��-��ַ-ָ���
	for (int i = 1; i<5; i++)
	{
		reWrite[i] = (BYTE)writadd;
		writadd >>= 8;
	}
	WriteProcessMemory(pid, (LPVOID)((int)AntiSpyCallBase + 0x2F), &reWrite, sizeof(reWrite), 0);

	jmpWritProc((LPVOID)((int)AntiSpyCallBase + 0x34),(LPVOID)(0x0051A00A+6));
}

void TrainerFunctions::FreeAntiSpy() //�رռ���
{       
	getID();
	DWORD add_l = 0x0051A00A;
	BYTE jmp_l[6] ={0x50,0xE8,0xD0,0xD1,0xF3,0xFF};
	DWORD writex = WriteProcessMemory(pid,(LPVOID)add_l,&jmp_l,sizeof(jmp_l),0);
	::VirtualFreeEx(pid,AntiSpyCallBase,0,MEM_RELEASE);
	AntiSpyCallBase=0;
}


__declspec(naked) void TrainerFunctions::DisableGAGAP_Assemble()
{
	__asm
	{
		mov eax,[esi+21Ch]
		push esi
		mov esi,00A83D4Ch
		cmp eax,[esi]
		pop esi
		je originalcode   //�����
		mov eax,[esi]
		cmp eax,007E3EBCh
		jne originalcode  //���ǽ���
		mov eax,[esi+520h]
		mov eax,[eax+0DF8h]
		cmp eax,3Fh
		jne originalcode  //�����ѷ������
		mov eax,1
//		jmp 006FAF1Ch		//�������ֱ����������д������
		nop					//jmpռλ
		nop
		nop
		nop
		nop

		originalcode:
		mov eax,[esi+00000504h]
//		exit1:
	}
}

void TrainerFunctions::DisableGAGAP() //̱���ѷ������
{
	DWORD sizeX;
	getID();        //get process HANDLE
	//PutCallBase = VirtualAllocEx(pid,NULL,0x256,MEM_COMMIT,PAGE_EXECUTE_READWRITE);
	virtualAE(&DisGAGAPCallBase);        //VirtualAllocEx
	jmpWritProc((LPVOID)0x006FAF0D,DisGAGAPCallBase); //jmp write address
	writeMemory((0x006FAF0D+5),0x90,1);		//0x90->nop
	writeMemory((DWORD)DisGAGAPCallBase,(DWORD)DisableGAGAP_Assemble,256);
	WriteProcessMemory(pid,DisGAGAPCallBase,&DisableGAGAP_Assemble,256,&sizeX);
	jmpWritProc((LPVOID)((int)DisGAGAPCallBase + 0x30),(LPVOID)(0x006FAF1C));	//jmp 006FAF1Ch
	jmpWritProc((LPVOID)((int)DisGAGAPCallBase + 0x3B),(LPVOID)(0x006FAF0D+6));
}

void TrainerFunctions::FreeDisableGAGAP() //�ر�̱���ѷ������
{       
	getID();
	DWORD add_l = 0x006FAF0D;
	BYTE jmp_l[6] ={0x8B,0x86,0x04,0x05,0x00,0x00};
	DWORD writex = WriteProcessMemory(pid,(LPVOID)add_l,&jmp_l,sizeof(jmp_l),0);
	::VirtualFreeEx(pid,DisGAGAPCallBase,0,MEM_RELEASE);
	DisGAGAPCallBase=0;
}


__declspec(naked) void TrainerFunctions::DisableAll_Assemble()
{
	__asm
	{
		mov eax,[esi+21Ch]
		push esi
		mov esi,00A83D4Ch
		cmp eax,[esi]
		pop esi
		je originalcode
		mov eax,1
//		jmp 006FAF1Ch		//�������ֱ����������д������
		nop					//jmpռλ
		nop
		nop
		nop
		nop

		originalcode:
		mov eax,[esi+504h]
		test eax,eax
//		jng 006FAFFDh		//���ֵд���ԣ�ֱ����������д������
		nop					//ռλ
		nop
		nop
		nop
		nop
		nop
//		exit1:
	}
}

void TrainerFunctions::DisableAll() //̱���з����е�λ
{
	DWORD sizeX;
	getID();        //get process HANDLE
	//PutCallBase = VirtualAllocEx(pid,NULL,0x256,MEM_COMMIT,PAGE_EXECUTE_READWRITE);
	virtualAE(&DisAllCallBase);        //VirtualAllocEx
	jmpWritProc((LPVOID)0x006FAF15,DisAllCallBase); //jmp write address
	writeMemory((0x006FAF15+5),0x90,1);		//0x90->nop
	writeMemory((DWORD)DisAllCallBase,(DWORD)DisableAll_Assemble,256);
	WriteProcessMemory(pid,DisAllCallBase,&DisableAll_Assemble,256,&sizeX);
	jmpWritProc((LPVOID)((int)DisAllCallBase + 0x16),(LPVOID)(0x006FAF1C));	//jmp 006FAF1Ch

	//��д������
	BYTE reWrite[6] = { 0x0F,0x8E };		//jng
	DWORD writadd = 0x006FAFFD - ((DWORD)DisAllCallBase + 0x23) - 6;	//Ŀ��-��ַ-ָ���
	for (int i = 2; i<6; i++)
	{
		reWrite[i] = (BYTE)writadd;
		writadd >>= 8;
	}
	WriteProcessMemory(pid, (LPVOID)((int)DisAllCallBase + 0x23), &reWrite, sizeof(reWrite), 0);

	jmpWritProc((LPVOID)((int)DisAllCallBase + 0x29),(LPVOID)(0x006FAF15+6));
}

void TrainerFunctions::FreeDisableAll() //�ر�̱���з����е�λ
{       
	getID();
	DWORD add_l = 0x006FAF15;
	BYTE jmp_l[6] ={0x0F,0x8E,0xE2,0x00,0x00,0x00};
	DWORD writex = WriteProcessMemory(pid,(LPVOID)add_l,&jmp_l,sizeof(jmp_l),0);
	::VirtualFreeEx(pid,DisAllCallBase,0,MEM_RELEASE);
	DisAllCallBase=0;
}


void TrainerFunctions::SpeedSet() //����
{
	getID();        //get process HANDLE
	//PutCallBase = VirtualAllocEx(pid,NULL,0x256,MEM_COMMIT,PAGE_EXECUTE_READWRITE);
	jmpWritProc((LPVOID)0x0055D76E,(LPVOID)0x0055D79E); //jmp write address
	writeMemory((0x0055D76E+5),0x90,1);		//0x90->nop
}

void TrainerFunctions::FreeSpeedSet() //����
{       
	getID();
	DWORD add_l = 0x0055D76E;
	BYTE jmp_l[6] ={0x75,0x2E,0x8D,0x4C,0x24,0x14};
	DWORD writex = WriteProcessMemory(pid,(LPVOID)add_l,&jmp_l,sizeof(jmp_l),0);
}


__declspec(naked) void TrainerFunctions::EnableSoldAll_Cursor_Assemble()
{
	__asm
	{
		mov edx,[esi]
		mov ecx,esi
		call dword ptr [edx+2Ch]
		cmp eax,24h
		je originalcode
		//enable value is 0C disable is 0E
		mov eax,0Ch
//		jmp 00692A20h		//�������ֱ����������д������
		nop					//jmpռλ
		nop
		nop
		nop
		nop

		originalcode:
		mov eax,[esi]
		mov ecx,esi
		call dword ptr [eax+3Ch]
		//exit1:
	}
}

void TrainerFunctions::EnableSoldAll_Cursor() //��������������е�λ
{
	DWORD sizeX;
	getID();        //get process HANDLE
	//PutCallBase = VirtualAllocEx(pid,NULL,0x256,MEM_COMMIT,PAGE_EXECUTE_READWRITE);
	virtualAE(&SoldAllCallBase);        //VirtualAllocEx
	jmpWritProc((LPVOID)0x006929D1,SoldAllCallBase); //jmp write address
	writeMemory((0x006929D1+5),0x90,1);		//0x90->nop
	writeMemory((0x006929D1+6),0x90,1);		//0x90->nop
	writeMemory((DWORD)SoldAllCallBase,(DWORD)EnableSoldAll_Cursor_Assemble,256);
	WriteProcessMemory(pid,SoldAllCallBase,&EnableSoldAll_Cursor_Assemble,256,&sizeX);
	jmpWritProc((LPVOID)((int)SoldAllCallBase + 0x11),(LPVOID)(0x00692A20));	//jmp 00692A20h
	jmpWritProc((LPVOID)((int)SoldAllCallBase + 0x1D),(LPVOID)(0x006929D1+7));
}

void TrainerFunctions::FreeEnableSoldAll_Cursor() //�ر���������������е�λ
{       
	getID();
	DWORD add_l = 0x006929D1;
	BYTE jmp_l[7] ={0x8B,0x06,0x8B,0xCE,0xFF,0x50,0x3C};
	DWORD writex = WriteProcessMemory(pid,(LPVOID)add_l,&jmp_l,sizeof(jmp_l),0);
	::VirtualFreeEx(pid,SoldAllCallBase,0,MEM_RELEASE);
	SoldAllCallBase=0;
}
void TrainerFunctions::EnableSoldAll_Belong() //��������������е�λ-��λ����
{
	getID();        //get process HANDLE
	//PutCallBase = VirtualAllocEx(pid,NULL,0x256,MEM_COMMIT,PAGE_EXECUTE_READWRITE);
	jmpWritProc((LPVOID)0x004C6F48,(LPVOID)0x004C6F9C); //jmp write address
	writeMemory((0x004C6F48+5),0x90,1);		//0x90->nop
}

void TrainerFunctions::FreeEnableSoldAll_Belong() //
{       
	getID();
	DWORD add_l = 0x004C6F48;
	BYTE jmp_l[6] ={0x0F,0x85,0xBB,0x11,0x00,0x00};
	DWORD writex = WriteProcessMemory(pid,(LPVOID)add_l,&jmp_l,sizeof(jmp_l),0);
}
void TrainerFunctions::EnableSoldAll_Builder() //��������������е�λ-��λ������
{
	getID();        //get process HANDLE
	//PutCallBase = VirtualAllocEx(pid,NULL,0x256,MEM_COMMIT,PAGE_EXECUTE_READWRITE);
	writeMemory((0x0044711B  ),0x90,1);		//0x90->nop
	writeMemory((0x0044711B+1),0x90,1);		//0x90->nop
	writeMemory((0x0044711B+2),0x90,1);		//0x90->nop
	writeMemory((0x0044711B+3),0x90,1);		//0x90->nop
	writeMemory((0x0044711B+4),0x90,1);		//0x90->nop
	writeMemory((0x0044711B+5),0x90,1);		//0x90->nop
}

void TrainerFunctions::FreeEnableSoldAll_Builder() //
{       
	getID();
	DWORD add_l = 0x0044711B;
	BYTE jmp_l[6] ={0x0F,0x84,0xA4,0x00,0x00,0x00};
	DWORD writex = WriteProcessMemory(pid,(LPVOID)add_l,&jmp_l,sizeof(jmp_l),0);
}


__declspec(naked) void TrainerFunctions::SoldierFlashMove_Assemble()
{
	__asm
	{
		pushad
		mov ecx,[esi+8h]
		mov ecx,[ecx+21Ch]
		push esi
		mov esi,00A83D4Ch
		cmp ecx,[esi]
		pop esi
		jne originalcode

		mov ecx,eax

		mov edx,[esi+24h]
		test edx,edx
		je originalcode
		mov edx,[esi+28h]
		test edx,edx
		je originalcode

		lea eax,[esi+24h]
		push eax
		mov edx,[ecx]
		call [edx+1B4h]

		originalcode:
		popad
		mov byte ptr [esi+31h],00
		mov eax,[esi]
		//exit1:
	}
}

void TrainerFunctions::SoldierFlashMove() //����Ь
{
	DWORD sizeX;
	getID();        //get process HANDLE
	//PutCallBase = VirtualAllocEx(pid,NULL,0x256,MEM_COMMIT,PAGE_EXECUTE_READWRITE);
	virtualAE(&FlashMoveCallBase);        //VirtualAllocEx
	jmpWritProc((LPVOID)0x0075AC93,FlashMoveCallBase); //jmp write address
	writeMemory((0x0075AC93+5),0x90,1);		//0x90->nop
	writeMemory((DWORD)FlashMoveCallBase,(DWORD)SoldierFlashMove_Assemble,256);
	WriteProcessMemory(pid,FlashMoveCallBase,&SoldierFlashMove_Assemble,256,&sizeX);
	jmpWritProc((LPVOID)((int)FlashMoveCallBase + 0x38),(LPVOID)(0x0075AC93+6));
}

void TrainerFunctions::FreeSoldierFlashMove() //�رջ���Ь
{       
	getID();
	DWORD add_l = 0x0075AC93;
	BYTE jmp_l[6] ={0xC6,0x46,0x31,0x00,0x8B,0x06};
	DWORD writex = WriteProcessMemory(pid,(LPVOID)add_l,&jmp_l,sizeof(jmp_l),0);
	::VirtualFreeEx(pid,FlashMoveCallBase,0,MEM_RELEASE);
	FlashMoveCallBase=0;
}


__declspec(naked) void TrainerFunctions::EliteBuild_Aircraft_Assemble()
{
	__asm
	{
		push eax
		mov eax,[esi+0000021Ch]
		push esi
		mov esi,00A83D4Ch
		cmp eax,[esi]
		pop esi
		pop eax
		jne originalcode
		mov dword ptr [esi+150h],40000000h

		originalcode:
		mov edi,[esi+000006C4h]
	}
}
void TrainerFunctions::EliteBuild_Aircraft() //�������3��-�ɻ�
{
	DWORD sizeX;
	getID();        //get process HANDLE
	//PutCallBase = VirtualAllocEx(pid,NULL,0x256,MEM_COMMIT,PAGE_EXECUTE_READWRITE);
	virtualAE(&EliteBdACallBase);        //VirtualAllocEx
	jmpWritProc((LPVOID)0x00413F8A,EliteBdACallBase); //jmp write address
	writeMemory((0x00413F8A+5),0x90,1);		//0x90->nop
	writeMemory((DWORD)EliteBdACallBase,(DWORD)EliteBuild_Aircraft_Assemble,256);
	WriteProcessMemory(pid,EliteBdACallBase,&EliteBuild_Aircraft_Assemble,256,&sizeX);
	jmpWritProc((LPVOID)((int)EliteBdACallBase + 0x23),(LPVOID)(0x00413F8A+6));
}
void TrainerFunctions::FreeEliteBuild_Aircraft() //�ر��������3��-�ɻ�
{       
	getID();
	DWORD add_l = 0x00413F8A;
	BYTE jmp_l[6] ={0x8B,0xBE,0xC4,0x06,0x00,0x00};
	DWORD writex = WriteProcessMemory(pid,(LPVOID)add_l,&jmp_l,sizeof(jmp_l),0);
	::VirtualFreeEx(pid,EliteBdACallBase,0,MEM_RELEASE);
	EliteBdACallBase=0;
}
void TrainerFunctions::EliteBuild_Vehicle() //�������3��-���� ������ɻ��Ļ�������ͬ
{
	DWORD sizeX;
	getID();        //get process HANDLE
	//PutCallBase = VirtualAllocEx(pid,NULL,0x256,MEM_COMMIT,PAGE_EXECUTE_READWRITE);
	virtualAE(&EliteBdVCallBase);        //VirtualAllocEx
	jmpWritProc((LPVOID)0x007355F6,EliteBdVCallBase); //jmp write address
	writeMemory((0x007355F6+5),0x90,1);		//0x90->nop
	writeMemory((DWORD)EliteBdVCallBase,(DWORD)EliteBuild_Aircraft_Assemble,256);
	WriteProcessMemory(pid,EliteBdVCallBase,&EliteBuild_Aircraft_Assemble,256,&sizeX);
	jmpWritProc((LPVOID)((int)EliteBdVCallBase + 0x23),(LPVOID)(0x007355F6+6));
}
void TrainerFunctions::FreeEliteBuild_Vehicle() //�ر��������3��-����
{       
	getID();
	DWORD add_l = 0x007355F6;
	BYTE jmp_l[6] ={0x8B,0xBE,0xC4,0x06,0x00,0x00};
	DWORD writex = WriteProcessMemory(pid,(LPVOID)add_l,&jmp_l,sizeof(jmp_l),0);
	::VirtualFreeEx(pid,EliteBdVCallBase,0,MEM_RELEASE);
	EliteBdVCallBase=0;
}
__declspec(naked) void TrainerFunctions::EliteBuild_Soldier_Assemble()
{
	__asm
	{
		mov ecx,[esi+0000021Ch]
		push esi
		mov esi,00A83D4Ch
		cmp ecx,[esi]
		pop esi
		jne originalcode
		mov dword ptr [esi+150h],40000000h

		originalcode:
		mov ecx,[esi+0000021Ch]
	}
}
void TrainerFunctions::EliteBuild_Soldier() //�������3��-����
{
	DWORD sizeX;
	getID();        //get process HANDLE
	//PutCallBase = VirtualAllocEx(pid,NULL,0x256,MEM_COMMIT,PAGE_EXECUTE_READWRITE);
	virtualAE(&EliteBdSCallBase);        //VirtualAllocEx
	jmpWritProc((LPVOID)0x00517CC9,EliteBdSCallBase); //jmp write address
	writeMemory((0x00517CC9+5),0x90,1);		//0x90->nop
	writeMemory((DWORD)EliteBdSCallBase,(DWORD)EliteBuild_Soldier_Assemble,256);
	WriteProcessMemory(pid,EliteBdSCallBase,&EliteBuild_Soldier_Assemble,256,&sizeX);
	jmpWritProc((LPVOID)((int)EliteBdSCallBase + 0x21),(LPVOID)(0x00517CC9+6));
}
void TrainerFunctions::FreeEliteBuild_Soldier() //�ر��������3��-����
{       
	getID();
	DWORD add_l = 0x00517CC9;
	BYTE jmp_l[6] ={0x8B,0x8E,0x1C,0x02,0x00,0x00};
	DWORD writex = WriteProcessMemory(pid,(LPVOID)add_l,&jmp_l,sizeof(jmp_l),0);
	::VirtualFreeEx(pid,EliteBdSCallBase,0,MEM_RELEASE);
	EliteBdSCallBase=0;
}


////////////////////////////////////


