#include "stdafx.h"
#include "trainerfunctions.h"

LPVOID PowerCallBase;        //电力申请的内存指针
LPVOID NoPowerCallBase;      //停电申请的内存指针
LPVOID PutCallBase;        //随意建筑申请的内存指针
LPVOID PutWaterCallBase;    //水面建筑申请的内存指针
LPVOID AtraCallBase;		//自动修理申请的内存指针
LPVOID AtraStrCallBase;		//中立自动修理申请的内存指针
LPVOID ReYuriCallBase;		//反尤里申请的内存指针
LPVOID PerYuriCallBase;		//永久控制申请的内存指针
LPVOID MineAtkCallBase;		//反攻击申请的内存指针
LPVOID MineBdiCallBase;		//反进房申请的内存指针
LPVOID MineUneCallBase;		//反占领申请的内存指针
LPVOID BdImeCallBase;		//瞬间建造申请的内存指针
LPVOID UnbSKCallBase;		//无敌/秒杀申请的内存指针
LPVOID AntiChronoCallBase;	//反超时空申请的内存指针1
LPVOID AntChonoDbCallBase;	//反超时空申请的内存指针2

LPVOID UnRaCallBase;		//反修理申请的内存指针
//科技全开
LPVOID AllBCallBase;		//全建筑申请的内存指针
LPVOID AllCCallBase;		//全车船申请的内存指针
LPVOID AllSCallBase;		//全步兵申请的内存指针
LPVOID AllPCallBase;		//全飞机申请的内存指针

LPVOID FstAtkCallBase;		//极速攻击申请的内存指针
LPVOID FstTurnBtCallBase;	//极速转炮塔申请的内存指针
LPVOID FstTurnRdCallBase;	//极速转身申请的内存指针
LPVOID FstRldCallBase;		//闪电重装申请的内存指针
LPVOID FulAmuCallBase;		//弹药充足申请的内存指针
LPVOID ARgeAtkCallBase;		//远程打击申请的内存指针
LPVOID ARgeAleCallBase;		//远程警戒申请的内存指针
LPVOID InsChoMovCallBase;	//瞬间超时空移动申请的内存指针
LPVOID InsChoAtkCallBase;	//瞬间超时空攻击申请的内存指针
LPVOID AntiSpyCallBase;		//间间谍申请的内存指针
LPVOID DisGAGAPCallBase;	//瘫痪裂缝产生器申请的内存指针
LPVOID DisAllCallBase;		//瘫痪敌方所有单位申请的内存指针
//LPVOID SpeedSetCallBase;	//调速直接替换不需要申请内存
LPVOID SoldAllCallBase;		//允许售卖所有单位申请的内存指针
//LPVOID ;					//售卖-所属 直接替换不需要申请内存
//LPVOID ;					//售卖-建造者直接替换不需要申请内存
LPVOID FlashMoveCallBase;	//滑板鞋申请的内存指针
LPVOID EliteBdACallBase;	//三级建造申请的内存指针1
LPVOID EliteBdVCallBase;	//三级建造申请的内存指针2
LPVOID EliteBdSCallBase;	//三级建造申请的内存指针3


//////////////////////////////////////////////寄存器
BOOL TrainerFunctions::rSpeed(DWORD* pSpeed) //读取速度
{
	const DWORD address = 0x00A8EB60;
	if(!readMemory(address,pSpeed))
		return FALSE;
	return TRUE;
}

BOOL TrainerFunctions::wSpeed(DWORD Speed) //写入速度
{
	const DWORD address = 0x00A8EB60;
	DWORD SpeedTest;
	if(!readMemory(address,&SpeedTest))
		return FALSE;
	writeMemory(address,Speed);
	return TRUE;
}

BOOL TrainerFunctions::rMymoney(DWORD* pMoney) //读取金钱
{
	const DWORD address[2] = {0x00A83D4C,0x30C};
	DWORD money;
	if(!readMemory(address,2,&money))
		return FALSE;
	*pMoney = money;
	return TRUE;
}

BOOL TrainerFunctions::wMymoney(DWORD money) //金钱修改
{
	const DWORD address[2] = {0x00A83D4C,0x30C};
	DWORD add;
	if(!readAddress(address,2,&add))
		return FALSE;
	writeMemory(add,money); //实际金钱地址
	return TRUE;
}


BOOL TrainerFunctions::rBuildMaxm(DWORD* pMaxm) //读取建造队列上限
{
	const DWORD address[2] = {0x008871E0,0xF0};
	DWORD maxm;
	if(!readMemory(address,2,&maxm))
		return FALSE;
	*pMaxm = maxm+1;
	return TRUE;
}

BOOL TrainerFunctions::wBuildMaxm(DWORD maxm) //上限修改
{
	const DWORD address[2] = {0x008871E0,0xF0};
	DWORD add;
	if(!readAddress(address,2,&add))
		return FALSE;
	writeMemory(add,maxm-1); //
	return TRUE;
}

BOOL TrainerFunctions::QuickBuild() //快速建造
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

BOOL TrainerFunctions::RadarOn() //总是有雷达
{
	const DWORD address[2] = {0x00A8B230,0x34A4};
	DWORD add;
	if(!readAddress(address,2,&add))
		return FALSE;
	writeMemory(add,0x1,0x1);
	return TRUE;
}

BOOL TrainerFunctions::SuperOn() //无限超武
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
	//好像一共11个……
	const DWORD add_pre[2] = {0x00A83D4C,0x258};
	DWORD t1,t2,dis;	//dis->0~54

	if(!readMemory(add_pre,2,&t1)){return FALSE;}
	for(DWORD i=0; i<12; i++)		//11个循环
	{
		if(!readMemory(t1+i*4,&t2)){return FALSE;}
		if(!readMemory(t2+0x78,&dis)){return FALSE;}	//读取面板显示，没有是-1
		if((long)dis != -1)
			writeMemory(t2+0x6F,0x1,0x1);		//修改就绪标识
	}
	return TRUE;
}

void TrainerFunctions::WinImme() //立即胜利
{
	writeMemory(0x00A83D49,0x1,0x1);
}

BOOL TrainerFunctions::TobeGhost() //成为幽灵玩家
{
	DWORD dat1,dat2;
	readMemory(0x00A8ECC8,&dat1);	//选中数量
	if(dat1 != 1)		//必须选择一个建筑
		return FALSE;

	DWORD address[3] = {0x00A8ECBC,0,0};
	readMemory(address,3,&dat1);		//选中单位首地址
	if(dat1 != 0x007E3EBC)		//必须选择一个建筑
		return FALSE;

	address[2] = 0x21C;		//单位所属
	readMemory(address,3,&dat1);		//选中单位所属
	readMemory(0x00A83D4C,&dat2);		//玩家数据基址
	if(dat1 != dat2)		//必须选择玩家单位
		return FALSE;
	//满足条件后开始转移
	DWORD PlayerID[2] = {0x00A83D4C,0x30};
	readMemory(PlayerID,2,&dat1);		//获取玩家当前ID
	if(dat1 != 0)		//转移到其他ID
		dat1--;
	else
		dat1++;

	DWORD AimID[2] = {0x00A8022C,4*dat1};	//目标玩家基址
	readMemory(AimID,2,&dat2);		//获取目标玩家数据基址
	readAddress(address,3,&dat1);		//选中单位所属存储地址
	writeMemory(dat1,dat2);		//转移

	//address[2] = 0x6C;		//单位血量
	//readAddress(address,3,&dat1);		//选中单位血量存储地址
	//writeMemory(dat1,0);		//销毁选中的建筑物

	//删除选中单位
	writProcess(DeleteThis_Assemble);

	return TRUE;	
}

//////////////////////////////////////////////////线程
//全地图内联代码
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
	//地图全开
	writProcess(Map_Assemble);
}

//额外核弹内联代码
void TrainerFunctions::NuclearBomb_Assemble()
{
	int eax_temp;       
	__asm
	{
		pushad
		mov ecx,0x00A83D4C	//检查是否已经有核弹发射井
		mov ecx,[ecx]
		add ecx,0x258
		mov ecx,[ecx]
		mov ecx,[ecx]
		add ecx,0x78
		mov ecx,[ecx]
		cmp ecx,0xFFFFFFFF	//面板-1表示没有 0~54表示有
		jne exit1	//有就退出防止出错

		mov ecx,0x14
		mov eax_temp,0x006CEEB0
		call eax_temp
		mov ebx,eax
		//调整核弹CD
		add ebx,0x98
		mov eax,[ebx]//超武编号
		mov edx,0x00A83D4C
		mov edx,[edx]
		add edx,0x258
		mov ecx,[edx]
		push 0           //初始状态，1=等待
		push 0           //对应建筑
		mov ecx,[ecx+eax*4]
		push 1             //1=一次性 核弹必须1 其他可以0
		mov eax_temp,0x006CB560
		call eax_temp
		//增加核弹攻击选项
		add ebx,0x98
		mov eax,[ebx]//超武编号
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
	//额外核弹
	writProcess(NuclearBomb_Assemble);
}

//单位升级内联汇编
void TrainerFunctions::UpChose_Assemble()
{
	__asm
	{
		pushad
		mov eax,0x00A8ECBC
		mov ebx,[eax]	//选中单位首地址
		mov eax,0

		process:
		mov ecx,0x00A8ECC8
		cmp eax,[ecx]  //选中单位数量为零判断
		jge brek//>=跳转
		mov ecx,[ebx+eax*4]  //当前单位地址
		mov edx,0x40000000		//Float 2.0
		add ecx,0x150
		mov [ecx],edx  //当前单位经验
		add eax,1
		jmp process

		brek:
		popad
	}
}

void TrainerFunctions::UpChose()
{
	//单位升级
	writProcess(UpChose_Assemble);
}

//单位加速内联汇编
void TrainerFunctions::SpeedUpChose_Assemble()
{
	__asm
	{
		pushad
		mov eax,0x00A8ECBC
		mov ebx,[eax]	//选中单位首地址
		mov eax,0

		process:
		mov ecx,0x00A8ECC8
		cmp eax,[ecx]  //选中单位数量
		jge brek//>=跳转
		mov ecx,[ebx+eax*4]  //当前单位地址
		mov edx,0x007E3EBC
		cmp [ecx],edx	//当前单位类型为建筑
		je brek			//建筑没速度不能改
		mov edx,0x007EB058
		cmp [ecx],edx	//步兵最大改2.0
		jne next1
		mov edx,0x40000000
		add ecx,0x584
		mov [ecx],edx
		je loopcontinue
		next1:
		mov edx,0x007F5C70
		cmp [ecx],edx	//车船
		jne next2
		mov edx,0x40A00000
		add ecx,0x584
		mov [ecx],edx	//5.0
		je loopcontinue
		next2:
		//飞机改这个没用
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
	//单位加速
	writProcess(SpeedUpChose_Assemble);
}

//全是我的-选中单位 内联汇编
void TrainerFunctions::MineChose_Assemble()
{
	__asm
	{
		pushad
		mov eax,0x00A8ECC8	//选中单位数量
		mov eax,[eax]
		cmp eax,0		//是否选中单位
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
	//全是我的-选中单位
	writProcess(MineChose_Assemble);
}

//删除选中单位 内联汇编
void TrainerFunctions::DeleteThis_Assemble()
{
	__asm
	{
		pushad
		mov eax,0x00A8ECC8
		mov eax,[eax]//选中单位数量
		cmp eax,0		//是否选中单位
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
	//删除选中单位
	writProcess(DeleteThis_Assemble);
}


////////////////////////////////////注入
__declspec(naked) void TrainerFunctions::NoLoad_Assemble()
{
	__asm
	{
		push ecx
		mov ecx,00A83D4Ch
		mov ecx,[ecx]      //玩家数据地址
		cmp ecx,esi
		jnz originalcode       //不是玩家跳转
		mov [esi+000053A8h],0     //修改！！
		jmp exit1

		originalcode:
		mov [esi+000053A8h],edx

		exit1:
		pop ecx
	}
}

void TrainerFunctions::NoLoad() //无限电力
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

void TrainerFunctions::FreeNoLoad() //关闭无限电力
{       
	getID();
	DWORD add_l = 0x00508D18;
	BYTE jmp_l[6] ={0x89,0x96,0xA8,0x53,00,00};
	DWORD writex = WriteProcessMemory(pid,(LPVOID)add_l,&jmp_l,sizeof(jmp_l),0);
	::VirtualFreeEx(pid,PowerCallBase,0,MEM_RELEASE);
	PowerCallBase=0;
}
/*旧版停电，会造成电脑只造电厂不攻击，用新版解决这个问题
__declspec(naked) void TrainerFunctions::NoPower_Assemble()
{
	__asm
	{
		push ecx
		mov ecx,00A83D4Ch
		mov ecx,[ecx]      //玩家数据地址
		cmp ecx,esi
		je originalcode       //是玩家跳转
		mov ecx,[esi+000053A4h]     //负载
		inc ecx		//负载+1
		mov edx,ecx		//电力永远比负载小1 气不气\(^o^)/~
		jmp exit1

		originalcode:
		mov edx,[esi+000053A8h]

		exit1:
		pop ecx
	}
}

void TrainerFunctions::NoPower() //停电
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

void TrainerFunctions::FreeNoPower() //关闭停电
{       
	getID();
	DWORD add_l = 0x00508D0E;
	BYTE jmp_l[6] ={0x8B,0x96,0xA8,0x53,00,00};
	DWORD writex = WriteProcessMemory(pid,(LPVOID)add_l,&jmp_l,sizeof(jmp_l),0);
	::VirtualFreeEx(pid,NoPowerCallBase,0,MEM_RELEASE);
	NoPowerCallBase=0;
}
*/
//不会导致AI只造电厂问题，但是超级武器不断电￣へ￣以后改进吧
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
		nop					//占位
		nop
		nop
		nop
		nop
		nop
		cmp[esi + 0000067Ch],edi
	}
}

void TrainerFunctions::NoPower() //停电
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

	//重写机器码
	BYTE reWrite[6] = { 0x0F,0x84 };		//je
	DWORD writadd = 0x00455643 - ((DWORD)NoPowerCallBase + 0x13) - 6;	//目标-地址-指令长度
	for (int i = 2; i<6; i++)
	{
		reWrite[i] = (BYTE)writadd;
		writadd >>= 8;
	}
	WriteProcessMemory(pid, (LPVOID)((int)NoPowerCallBase + 0x13), &reWrite, sizeof(reWrite), 0);

	jmpWritProc((LPVOID)((int)NoPowerCallBase + 0x1F), (LPVOID)(0x00455639 + 8));
}

void TrainerFunctions::FreeNoPower() //关闭停电
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

void TrainerFunctions::PutAsWill() //随意建筑
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

void TrainerFunctions::FreePutAsWill() //关闭随意建筑
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
//		jne 0047CA4Dh		//这个值写不对，直接在下面重写机器码
		nop					//占位
		nop
		nop
		nop
		nop
		nop
		//exit1:
	}
}

void TrainerFunctions::PutWaterAble() //水面建筑
{
	DWORD sizeX;
	getID();        //get process HANDLE
	//PutCallBase = VirtualAllocEx(pid,NULL,0x256,MEM_COMMIT,PAGE_EXECUTE_READWRITE);
	virtualAE(&PutWaterCallBase);        //VirtualAllocEx
	jmpWritProc((LPVOID)0x0047C9D1,PutWaterCallBase); //jmp write address
	writeMemory((DWORD)PutWaterCallBase,(DWORD)PutWaterAble_Assemble,256);
	WriteProcessMemory(pid,PutWaterCallBase,&PutWaterAble_Assemble,256,&sizeX);

	//重写机器码
	BYTE reWrite[6] ={0x0F,0x85};		//jne
	DWORD writadd = 0x0047CA4D-((DWORD)PutWaterCallBase+0x17)-6;	//目标-地址-指令长度
	for(int i=2; i<6; i++)
	{
		reWrite[i] = (BYTE)writadd;
		writadd >>= 8;
	}
	WriteProcessMemory(pid,(LPVOID)((int)PutWaterCallBase+0x17),&reWrite,sizeof(reWrite),0);

	jmpWritProc((LPVOID)((int)PutWaterCallBase + 0x1D),(LPVOID)(0x0047C9D1+5));		//jmp 0047CA4Dh
}

void TrainerFunctions::FreePutWaterAble() //关闭水面建筑
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
//		jl 00450813h		//这个值写不对，直接在下面重写机器码
		nop					//占位
		nop
		nop
		nop
		nop
		nop
	}
}

void TrainerFunctions::AutoRepair() //自动修理
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

	//重写机器码
	BYTE reWrite[6] ={0x0F,0x8C};		//jl
	DWORD writadd = 0x00450813-((DWORD)AtraCallBase+0x07)-6;	//目标-地址-指令长度
	for(int i=2; i<6; i++)
	{
		reWrite[i] = (BYTE)writadd;
		writadd >>= 8;
	}
	WriteProcessMemory(pid,(LPVOID)((int)AtraCallBase+0x07),&reWrite,sizeof(reWrite),0);

	jmpWritProc((LPVOID)((int)AtraCallBase + 0x0D),(LPVOID)(0x00450651+8));
}

void TrainerFunctions::FreeAutoRepair() //关闭自动修理
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
		jne originalcode//不是玩家照常
		mov cl,1//玩家都能修

		originalcode:
		test cl,cl
//		jne 00452652h		//这个值写不对，直接在下面重写机器码
		nop					//占位
		nop
		nop
		nop
		nop
		nop
		xor al,al
	}
}

void TrainerFunctions::AutoRepairStruc() //中立可自动修理
{
	DWORD sizeX;
	getID();        //get process HANDLE
	//PutCallBase = VirtualAllocEx(pid,NULL,0x256,MEM_COMMIT,PAGE_EXECUTE_READWRITE);
	virtualAE(&AtraStrCallBase);        //VirtualAllocEx
	jmpWritProc((LPVOID)0x0045264A,AtraStrCallBase); //jmp write address
	writeMemory((0x0045264A+5),0x90,1);		//0x90->nop
	writeMemory((DWORD)AtraStrCallBase,(DWORD)AutoRepairStruc_Assemble,256);
	WriteProcessMemory(pid,AtraStrCallBase,&AutoRepairStruc_Assemble,256,&sizeX);

	//重写机器码
	BYTE reWrite[6] ={0x0F,0x85};		//jne
	DWORD writadd = 0x00452652-((DWORD)AtraStrCallBase+0x15)-6;	//目标-地址-指令长度
	for(int i=2; i<6; i++)
	{
		reWrite[i] = (BYTE)writadd;
		writadd >>= 8;
	}
	WriteProcessMemory(pid,(LPVOID)((int)AtraStrCallBase+0x15),&reWrite,sizeof(reWrite),0);

	jmpWritProc((LPVOID)((int)AtraStrCallBase + 0x1D),(LPVOID)(0x0045264A+6));
}

void TrainerFunctions::FreeAutoRepairStruc() //关闭中立可自动修理
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
		je originalcode//是玩家就不变
		//解决怀疑bug: 控制方在执行到这里前阵亡就会出错
		mov eax,0
//		mov ecx,ecx//单位地址
		mov edx,00A8EC7Ch
		mov ebx,[edx]//所有单位数组首地址
		unitlist:
		mov edx,00A8EC88h
		cmp eax,[edx]//遍历所有单位
		jge originalcode//>=没有or阵亡
		cmp ecx,[ebx+eax*4]//存在
		je cheat
		inc eax
		jmp unitlist

		cheat:
		popad
		mov ebx,00A83D4Ch
		mov eax,[ebx]
		push eax
		//mov ecx,esi//ecx是控制方标识
		mov ebp,[ecx]//获取单位类型
		call dword ptr [ebp+000003D4h]
		mov ebx,00471DC2h
		jmp ebx//直接退出不触发动画

		originalcode:
		popad
		mov eax,00A83D4Ch
		mov eax,[eax]
		push eax
		mov ecx,esi
		call dword ptr [ebp+000003D4h]
	}
}

void TrainerFunctions::RevengeYuri() //反控制
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

void TrainerFunctions::FreeRevengeYuri() //关闭反控制
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

void TrainerFunctions::PermanentYuri() //永久控制
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

void TrainerFunctions::FreePermanentYuri() //关闭永久控制
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
		mov ebx,[esi+0xB0]//单位地址
		mov edx,00A8EC7Ch
		mov ecx,[edx]//所有单位数组首地址
		unitlist:
		mov edx,00A8EC88h
		cmp eax,[edx]//遍历所有单位
		jge originalcode//>=没有or阵亡
		cmp ebx,[ecx+eax*4]//存在
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

void TrainerFunctions::MineAttack() //攻击反控
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

void TrainerFunctions::FreeMineAttack() //关闭攻击反控
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

void TrainerFunctions::MineBuildIn() //进房反控
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

void TrainerFunctions::FreeMineBuildIn() //关闭进房反控
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

void TrainerFunctions::MineUnEngineer() //反工程师占领
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

void TrainerFunctions::FreeMineUnEngineer() //关闭反工程师占领
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
		mov eax,[eax]      //玩家数据地址
//		mov [eax+537Ch],1Eh    //兵营跳过
//		mov [eax+5380h],14h    //战车工场跳过
		mov ebx,dword ptr [eax+53ACh]//飞机
		cmp ebx,esi
		jz cheat       //是玩家
		mov ebx,dword ptr [eax+53B8h]//船只
		cmp ebx,esi
		jz cheat       //是玩家
		mov ebx,dword ptr [eax+53BCh]//建筑
		cmp ebx,esi
		jz cheat       //是玩家
		mov ebx,dword ptr [eax+53CCh]//武器
		cmp ebx,esi
		jz cheat       //是玩家
		jmp originalcode

		cheat:
		mov [esi+24h],36h     //修改！！
		jmp exit1

		originalcode:
		mov [esi+24h],edx

		exit1:
		pop ebx
		pop eax
		mov edx,[esp+10h]
	}
}

void TrainerFunctions::BuildImme() //瞬间建造
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

void TrainerFunctions::FreeBuildImme() //关闭瞬间建造
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
		mov ecx,[ecx]			//玩家数据地址
		cmp ecx,[esi+21Ch]		//判断受伤害单位是否属于玩家
		pop ecx
		jnz originalcode       //跳转
		sub eax,0		//玩家不受伤害
		jmp exit1

		originalcode:
		sub eax,edx		//敌方不变

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
		mov ecx,[ecx]			//玩家数据地址
		cmp ecx,[esi+21Ch]		//判断受伤害单位是否属于玩家
		pop ecx
		jnz originalcode       //跳转
		sub eax,edx		//玩家不变
		jmp exit1

		originalcode:
		sub eax,eax		//敌方被秒杀

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
		mov ecx,[ecx]			//玩家数据地址
		cmp ecx,[esi+21Ch]		//判断受伤害单位是否属于玩家
		pop ecx
		jnz originalcode       //跳转
		sub eax,0		//玩家不受伤害
		jmp exit1

		originalcode:
		sub eax,eax		//敌方被秒杀

		exit1:
		test eax,eax
		mov [esi+6Ch],eax
	}
}

void TrainerFunctions::UnbeatableSecKill_Pre() //无敌/秒杀替换函数
{
	getID();        //get process HANDLE
	//PutCallBase = VirtualAllocEx(pid,NULL,0x256,MEM_COMMIT,PAGE_EXECUTE_READWRITE);
	virtualAE(&UnbSKCallBase);        //VirtualAllocEx
	jmpWritProc((LPVOID)0x005F5509,UnbSKCallBase); //jmp write address
	writeMemory((0x005F5509+5),0x90,1);		//0x90->nop
	writeMemory((0x005F5509+5+1),0x90,1);		//0x90->nop
}
void TrainerFunctions::Unbeatable()//无敌实现函数
{
	DWORD sizeX;
	writeMemory((DWORD)UnbSKCallBase,(DWORD)Unbeatable_Assemble,256);
	WriteProcessMemory(pid,UnbSKCallBase,&Unbeatable_Assemble,256,&sizeX);
	jmpWritProc((LPVOID)((int)UnbSKCallBase + 0x1D),(LPVOID)(0x005F5509+7));
}
void TrainerFunctions::SecKill()//秒杀实现函数
{
	DWORD sizeX;
	writeMemory((DWORD)UnbSKCallBase,(DWORD)SecKill_Assemble,256);
	WriteProcessMemory(pid,UnbSKCallBase,&SecKill_Assemble,256,&sizeX);
	jmpWritProc((LPVOID)((int)UnbSKCallBase + 0x1C),(LPVOID)(0x005F5509+7));
}
void TrainerFunctions::UnbeatableSecKill()//无敌+秒杀实现函数
{
	DWORD sizeX;
	writeMemory((DWORD)UnbSKCallBase,(DWORD)UnbeatableSecKill_Assemble,256);
	WriteProcessMemory(pid,UnbSKCallBase,&UnbeatableSecKill_Assemble,256,&sizeX);
	jmpWritProc((LPVOID)((int)UnbSKCallBase + 0x1D),(LPVOID)(0x005F5509+7));
}

void TrainerFunctions::FreeUnbeatableSecKill() //关闭无敌/关闭秒杀
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
		mov eax,[esi+24h]  //发动超时空单位
		mov eax,[eax+21Ch]
		push esi
		mov esi,00A83D4Ch
		cmp eax,[esi]
		pop esi
		jne exit1

//		originalcode:
		mov [esi+48h],ecx  //时间计算后赋值

		exit1:
		pop eax
		cmp eax,ebx
	}
}
void TrainerFunctions::AntiChrono() //反超时空
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
void TrainerFunctions::FreeAntiChrono() //关闭反超时空
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
void TrainerFunctions::AntiChronoDisbuild() //超时空过程允许建造
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
void TrainerFunctions::FreeAntiChronoDisbuild() //关闭超时空过程允许建造
{       
	getID();
	DWORD add_l = 0x004521C0;
	BYTE jmp_l[7] ={0xC6,0x81,0x60,0x06,0x00,0x00,0x00};
	DWORD writex = WriteProcessMemory(pid,(LPVOID)add_l,&jmp_l,sizeof(jmp_l),0);
	::VirtualFreeEx(pid,AntChonoDbCallBase,0,MEM_RELEASE);
	AntChonoDbCallBase=0;
}
void TrainerFunctions::AntiChronoDisable() //超时空过程建造栏不变灰色
{
	getID();        //get process HANDLE
	writeMemory((0x006A97B2  ),0x90,1);		//0x90->nop
	writeMemory((0x006A97B2+1),0x90,1);		//0x90->nop
}
void TrainerFunctions::FreeAntiChronoDisable() //关闭超时空过程建造栏不变灰色
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
		jz originalcode		//玩家不变
		sub ebx,ebp			//敌方减血
		cmp ebx,0		 //还有血ebx>0
		jge exit1
		mov ebx,0		//没血就归零
		jmp exit1

		originalcode:
		add ebx,ebp

		exit1:
		add edi,ebp
		mov [esi+6Ch],ebx
	}
}

void TrainerFunctions::UnRepair() //反修
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

void TrainerFunctions::FreeUnRepair() //关闭反修
{       
	getID();
	DWORD add_l = 0x004508B4;
	BYTE jmp_l[7] ={0x01,0xEB,0x01,0xEF,0x89,0x5E,0x6C};
	DWORD writex = WriteProcessMemory(pid,(LPVOID)add_l,&jmp_l,sizeof(jmp_l),0);
	::VirtualFreeEx(pid,UnRaCallBase,0,MEM_RELEASE);
	UnRaCallBase=0;
}


void TrainerFunctions::UnRefreshCon() //不删除建造选项-不需要注入，直接替换
{
	getID();        //get process HANDLE
	jmpWritProc((LPVOID)0x006AA788,(LPVOID)0x006AAA68); //jmp 006AAA68
	writeMemory((0x006AA788+5),0x90,1);		//0x90->nop
}

void TrainerFunctions::FreeUnRefreshCon() //关闭不删除建造选项
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
//		jmp 0044575Fh	//在下面写jmp
	}
}

void TrainerFunctions::AllBuild() //建筑全科技
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

void TrainerFunctions::FreeAllBuild() //关闭建筑全科技
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
//		jmp 004457A4h	//在下面写jmp
	}
}

void TrainerFunctions::AllCarBoat() //车船全科技
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

void TrainerFunctions::FreeAllCarBoat() //关闭车船全科技
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
//		jmp 004457E5h	//在下面写jmp
	}
}

void TrainerFunctions::AllSoldier() //步兵全科技
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

void TrainerFunctions::FreeAllSoldier() //关闭步兵全科技
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

void TrainerFunctions::AllPlane() //飞机全科技
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

void TrainerFunctions::FreeAllPlane() //关闭飞机全科技
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
		jmp eax      //跳过时间计算，强制下一次攻击

		originalcode:
		mov eax,[esi+2F4h]
	}
}

void TrainerFunctions::FastAttack() //极速攻击
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

void TrainerFunctions::FreeFastAttack() //关闭极速攻击
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
		mov ebx,[eax]//所有单位数组首地址
		mov eax,0
		unitlist:
		mov edx,00A8EC88h
		cmp eax,[edx]//遍历所有单位
		jge exit1//>=没有or阵亡
		cmp ebp,[ebx+eax*4]//存在
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

void TrainerFunctions::FastTurnBattery() //极速转炮塔
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

void TrainerFunctions::FreeFastTurnBattery() //关闭极速转炮塔
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
//		je 004C92EBh		//这个值写不对，直接在下面重写机器码
		nop					//占位
		nop
		nop
		nop
		nop
		nop
		test bp,bp

//		originalcode:
//		jle 004C92EBh		//这个值写不对，直接在下面重写机器码
		nop					//占位
		nop
		nop
		nop
		nop
		nop
		mov ax,dx
	}
}

void TrainerFunctions::FastTurnRound() //极速转身
{
	DWORD sizeX;
	getID();        //get process HANDLE
	//PutCallBase = VirtualAllocEx(pid,NULL,0x256,MEM_COMMIT,PAGE_EXECUTE_READWRITE);
	virtualAE(&FstTurnRdCallBase);        //VirtualAllocEx
	jmpWritProc((LPVOID)0x004C92BF,FstTurnRdCallBase); //jmp write address
	writeMemory((DWORD)FstTurnRdCallBase,(DWORD)FastTurnRound_Assemble,256);
	WriteProcessMemory(pid,FstTurnRdCallBase,&FastTurnRound_Assemble,256,&sizeX);

	//重写机器码
	BYTE reWrite[6] = { 0x0F,0x84 };		//je
	DWORD writadd = 0x004C92EB - ((DWORD)FstTurnRdCallBase + 0x13) - 6;	//目标-地址-指令长度
	for (int i = 2; i<6; i++)
	{
		reWrite[i] = (BYTE)writadd;
		writadd >>= 8;
	}
	WriteProcessMemory(pid, (LPVOID)((int)FstTurnRdCallBase + 0x13), &reWrite, sizeof(reWrite), 0);

	*((LPWORD)reWrite) = 0x8E0F;		//jle(注意x86是小端模式)
	writadd = 0x004C92EB - ((DWORD)FstTurnRdCallBase + 0x1C) - 6;	//目标-地址-指令长度
	for (int i = 2; i<6; i++)
	{
		reWrite[i] = (BYTE)writadd;
		writadd >>= 8;
	}
	WriteProcessMemory(pid, (LPVOID)((int)FstTurnRdCallBase + 0x1C), &reWrite, sizeof(reWrite), 0);

	jmpWritProc((LPVOID)((int)FstTurnRdCallBase + 0x25),(LPVOID)(0x004C92BF+5));
}

void TrainerFunctions::FreeFastTurnRound() //关闭极速转身
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
		mov eax,[eax+00000D60h]//弹药重生间隔

		exit1:
	}
}

void TrainerFunctions::FastReload() //闪电重装
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

void TrainerFunctions::FreeFastReload() //关闭闪电重装
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
		mov eax,[eax+00000D5Ch]		//弹药量

		exit1:
	}
}

void TrainerFunctions::FullAmmunition() //弹药充足
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

void TrainerFunctions::FreeFullAmmunition() //关闭弹药充足
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
		mov edi,0xF900		//攻击范围F9 249
		jmp exit1

		originalcode:
		mov edi,[ebx+000000B4h]

		exit1:
	}
}

void TrainerFunctions::AllRangeAttack() //远程打击
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

void TrainerFunctions::FreeAllRangeAttack() //关闭远程打击
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
		mov ebx,0xF900		//警戒范围F9 249
		jmp exit1

		originalcode:
		mov ebx,[eax+000000B4h]

		exit1:
	}
}

void TrainerFunctions::AllRangeAlert() //远程警戒
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

void TrainerFunctions::FreeAllRangeAlert() //关闭远程警戒
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

void TrainerFunctions::InstantChronoMove() //瞬间超时空移动
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

void TrainerFunctions::FreeInstantChronoMove() //瞬间超时空移动
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
		mov eax,[esi+24h]//发动攻击单位指针
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
//		je 0071B064h		//这个值写不对，直接在下面重写机器码
		nop					//占位
		nop
		nop
		nop
		nop
		nop
	}
}

void TrainerFunctions::InstantChronoAttack() //瞬间超时空攻击
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

	//重写机器码
	BYTE reWrite[6] = { 0x0F,0x84 };		//je
	DWORD writadd = 0x0071B064 - ((DWORD)InsChoAtkCallBase + 0x1F) - 6;	//目标-地址-指令长度
	for (int i = 2; i<6; i++)
	{
		reWrite[i] = (BYTE)writadd;
		writadd >>= 8;
	}
	WriteProcessMemory(pid, (LPVOID)((int)InsChoAtkCallBase + 0x1F), &reWrite, sizeof(reWrite), 0);

	jmpWritProc((LPVOID)((int)InsChoAtkCallBase + 0x25),(LPVOID)(0x0071AFB9+9));
}

void TrainerFunctions::FreeInstantChronoAttack() //瞬间超时空攻击
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

		mov ebx,[edi+21Ch]//保存玩家指针
		mov [edi+21Ch],eax//暂时转给敌方
		push edi
		mov edi,00A83D4Ch
		mov eax,[edi]//间谍指针为玩家
		pop edi
		push eax
//		call 004571E0h		//无法正确写入，直接在下面重写机器码
		nop
		nop
		nop
		nop
		nop
		mov [edi+21Ch],ebx//所属还原
		jmp exit1

		originalcode:
		push eax
//		call 004571E0h		//无法正确写入，直接在下面重写机器码
		nop
		nop
		nop
		nop
		nop

		exit1:
	}
}

void TrainerFunctions::AntiSpy() //间间谍
{
	DWORD sizeX;
	getID();        //get process HANDLE
	//PutCallBase = VirtualAllocEx(pid,NULL,0x256,MEM_COMMIT,PAGE_EXECUTE_READWRITE);
	virtualAE(&AntiSpyCallBase);        //VirtualAllocEx
	jmpWritProc((LPVOID)0x0051A00A,AntiSpyCallBase); //jmp write address
	writeMemory((0x0051A00A+5),0x90,1);		//0x90->nop
	writeMemory((DWORD)AntiSpyCallBase,(DWORD)AntiSpy_Assemble,256);
	WriteProcessMemory(pid,AntiSpyCallBase,&AntiSpy_Assemble,256,&sizeX);

	//重写机器码
	BYTE reWrite[5] = { 0xE8 };		//call
	DWORD writadd = 0x004571E0 - ((DWORD)AntiSpyCallBase + 0x21) - 5;	//目标-地址-指令长度
	for (int i = 1; i<5; i++)
	{
		reWrite[i] = (BYTE)writadd;
		writadd >>= 8;
	}
	WriteProcessMemory(pid, (LPVOID)((int)AntiSpyCallBase + 0x21), &reWrite, sizeof(reWrite), 0);

//	BYTE reWrite[5] = { 0xE8 };		//call
	writadd = 0x004571E0 - ((DWORD)AntiSpyCallBase + 0x2F) - 5;	//目标-地址-指令长度
	for (int i = 1; i<5; i++)
	{
		reWrite[i] = (BYTE)writadd;
		writadd >>= 8;
	}
	WriteProcessMemory(pid, (LPVOID)((int)AntiSpyCallBase + 0x2F), &reWrite, sizeof(reWrite), 0);

	jmpWritProc((LPVOID)((int)AntiSpyCallBase + 0x34),(LPVOID)(0x0051A00A+6));
}

void TrainerFunctions::FreeAntiSpy() //关闭间间谍
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
		je originalcode   //是玩家
		mov eax,[esi]
		cmp eax,007E3EBCh
		jne originalcode  //不是建筑
		mov eax,[esi+520h]
		mov eax,[eax+0DF8h]
		cmp eax,3Fh
		jne originalcode  //不是裂缝产生器
		mov eax,1
//		jmp 006FAF1Ch		//编译出错，直接在下面重写机器码
		nop					//jmp占位
		nop
		nop
		nop
		nop

		originalcode:
		mov eax,[esi+00000504h]
//		exit1:
	}
}

void TrainerFunctions::DisableGAGAP() //瘫痪裂缝产生器
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

void TrainerFunctions::FreeDisableGAGAP() //关闭瘫痪裂缝产生器
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
//		jmp 006FAF1Ch		//编译出错，直接在下面重写机器码
		nop					//jmp占位
		nop
		nop
		nop
		nop

		originalcode:
		mov eax,[esi+504h]
		test eax,eax
//		jng 006FAFFDh		//这个值写不对，直接在下面重写机器码
		nop					//占位
		nop
		nop
		nop
		nop
		nop
//		exit1:
	}
}

void TrainerFunctions::DisableAll() //瘫痪敌方所有单位
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

	//重写机器码
	BYTE reWrite[6] = { 0x0F,0x8E };		//jng
	DWORD writadd = 0x006FAFFD - ((DWORD)DisAllCallBase + 0x23) - 6;	//目标-地址-指令长度
	for (int i = 2; i<6; i++)
	{
		reWrite[i] = (BYTE)writadd;
		writadd >>= 8;
	}
	WriteProcessMemory(pid, (LPVOID)((int)DisAllCallBase + 0x23), &reWrite, sizeof(reWrite), 0);

	jmpWritProc((LPVOID)((int)DisAllCallBase + 0x29),(LPVOID)(0x006FAF15+6));
}

void TrainerFunctions::FreeDisableAll() //关闭瘫痪敌方所有单位
{       
	getID();
	DWORD add_l = 0x006FAF15;
	BYTE jmp_l[6] ={0x0F,0x8E,0xE2,0x00,0x00,0x00};
	DWORD writex = WriteProcessMemory(pid,(LPVOID)add_l,&jmp_l,sizeof(jmp_l),0);
	::VirtualFreeEx(pid,DisAllCallBase,0,MEM_RELEASE);
	DisAllCallBase=0;
}


void TrainerFunctions::SpeedSet() //调速
{
	getID();        //get process HANDLE
	//PutCallBase = VirtualAllocEx(pid,NULL,0x256,MEM_COMMIT,PAGE_EXECUTE_READWRITE);
	jmpWritProc((LPVOID)0x0055D76E,(LPVOID)0x0055D79E); //jmp write address
	writeMemory((0x0055D76E+5),0x90,1);		//0x90->nop
}

void TrainerFunctions::FreeSpeedSet() //调速
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
//		jmp 00692A20h		//编译出错，直接在下面重写机器码
		nop					//jmp占位
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

void TrainerFunctions::EnableSoldAll_Cursor() //允许玩家售卖所有单位
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

void TrainerFunctions::FreeEnableSoldAll_Cursor() //关闭允许玩家售卖所有单位
{       
	getID();
	DWORD add_l = 0x006929D1;
	BYTE jmp_l[7] ={0x8B,0x06,0x8B,0xCE,0xFF,0x50,0x3C};
	DWORD writex = WriteProcessMemory(pid,(LPVOID)add_l,&jmp_l,sizeof(jmp_l),0);
	::VirtualFreeEx(pid,SoldAllCallBase,0,MEM_RELEASE);
	SoldAllCallBase=0;
}
void TrainerFunctions::EnableSoldAll_Belong() //允许玩家售卖所有单位-单位所属
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
void TrainerFunctions::EnableSoldAll_Builder() //允许玩家售卖所有单位-单位建造者
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

void TrainerFunctions::SoldierFlashMove() //滑板鞋
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

void TrainerFunctions::FreeSoldierFlashMove() //关闭滑板鞋
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
void TrainerFunctions::EliteBuild_Aircraft() //造出来就3级-飞机
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
void TrainerFunctions::FreeEliteBuild_Aircraft() //关闭造出来就3级-飞机
{       
	getID();
	DWORD add_l = 0x00413F8A;
	BYTE jmp_l[6] ={0x8B,0xBE,0xC4,0x06,0x00,0x00};
	DWORD writex = WriteProcessMemory(pid,(LPVOID)add_l,&jmp_l,sizeof(jmp_l),0);
	::VirtualFreeEx(pid,EliteBdACallBase,0,MEM_RELEASE);
	EliteBdACallBase=0;
}
void TrainerFunctions::EliteBuild_Vehicle() //造出来就3级-车船 车船与飞机的汇编代码相同
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
void TrainerFunctions::FreeEliteBuild_Vehicle() //关闭造出来就3级-车船
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
void TrainerFunctions::EliteBuild_Soldier() //造出来就3级-步兵
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
void TrainerFunctions::FreeEliteBuild_Soldier() //关闭造出来就3级-步兵
{       
	getID();
	DWORD add_l = 0x00517CC9;
	BYTE jmp_l[6] ={0x8B,0x8E,0x1C,0x02,0x00,0x00};
	DWORD writex = WriteProcessMemory(pid,(LPVOID)add_l,&jmp_l,sizeof(jmp_l),0);
	::VirtualFreeEx(pid,EliteBdSCallBase,0,MEM_RELEASE);
	EliteBdSCallBase=0;
}


////////////////////////////////////


