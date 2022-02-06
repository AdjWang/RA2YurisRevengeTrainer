#include "stdafx.h"
#include "trainerfunctions.h"

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
char* NoLoad_Assemble = "\
[ENABLE]\n\
//code from here to '[DISABLE]' will be used to enable the cheat\n\
alloc(newmem,2048)\n\
label(returnhere)\n\
label(originalcode)\n\
label(exit)\n\
newmem: //this is allocated memory, you have read,write,execute access\n\
//place your code here\n\
push ecx\n\
mov ecx,[00A83D4C]      //玩家数据地址\n\
//mov ecx,dword ptr [ecx]\n\
cmp ecx,esi\n\
pop ecx\n\
jnz originalcode       //跳转\n\
//mov [esi+000053A8],edx     //修改！！\n\
jmp exit\n\
originalcode:\n\
mov [esi+000053A8],edx\n\
exit:\n\
jmp returnhere\n\
\"gamemd.exe\"+108D18:\n\
jmp newmem\n\
nop\n\
returnhere:\n\
[DISABLE]\n\
//code from here till the end of the code will be used to disable the cheat\n\
dealloc(newmem)\n\
\"gamemd.exe\"+108D18:\n\
mov [esi+000053A8],edx\n\
//Alt: db 89 96 A8 53 00 00\n\
";
bool TrainerFunctions::NoLoad(int command) //无限电力
{
	return AutoAssemble(pid, NoLoad_Assemble, command);
}

//不会导致AI只造电厂问题，但是超级武器不断电￣へ￣以后改进吧
//char* NoPower_Assemble = "\";
//{
//	__asm
//	{
//		mov ebx,esi
//		add ebx,21Ch
//		mov ebx,[ebx]
//		push eax
//		mov eax,00A83D4Ch
//		cmp [eax],ebx
//		pop eax
////		je 00455643h
//		nop					//占位
//		nop
//		nop
//		nop
//		nop
//		nop
//		cmp[esi + 0000067Ch],edi
//	}
//}
void TrainerFunctions::NoPower() //停电
{
	// 开发中，暂不使用
}


//Str 004ABC7B  004FB372  00739502
//je add-004FB236 一切障碍物还有??
//CALL 004A9480
char* PutAsWill_Assemble = "\
[ENABLE]\n\
//code from here to '[DISABLE]' will be used to enable the cheat\n\
alloc(newmem,256)\n\
label(returnhere)\n\
label(originalcode)\n\
label(exit)\n\
newmem: //this is allocated memory, you have read,write,execute access\n\
//place your code here\n\
mov eax,01\n\
jmp 004A9063\n\
jmp exit\n\
originalcode:\n\
mov eax,[00A83D4C]\n\
exit:\n\
jmp returnhere\n\
\"gamemd.exe\"+A8EB0:\n\
jmp newmem\n\
returnhere:\n\
[DISABLE]\n\
//code from here till the end of the code will be used to disable the cheat\n\
dealloc(newmem)\n\
\"gamemd.exe\"+A8EB0:\n\
mov eax,[00A83D4C]\n\
//Alt: db A1 4C 3D A8 00\n\
";
char* PutWaterAble_Assemble = "\
[ENABLE]\n\
//code from here to '[DISABLE]' will be used to enable the cheat\n\
alloc(newmem,256)\n\
label(returnhere)\n\
label(originalcode)\n\
label(exit)\n\
newmem: //this is allocated memory, you have read,write,execute access\n\
//place your code here\n\
mov eax,[esp+24]\n\
cmp eax,[00A83D4C]\n\
jne originalcode\n\
mov ecx,03\n\
originalcode:\n\
cmp ecx,-01\n\
jne 0047CA4D\n\
exit:\n\
jmp returnhere\n\
\"gamemd.exe\"+7C9D1:\n\
jmp newmem\n\
returnhere:\n\
[DISABLE]\n\
//code from here till the end of the code will be used to disable the cheat\n\
dealloc(newmem)\n\
\"gamemd.exe\"+7C9D1:\n\
cmp ecx,-01\n\
jne 0047CA4D\n\
//Alt: db 83 F9 FF 75 77\n\
";
bool TrainerFunctions::PutAsWill(int command) //随意建筑
{
	//bool b1, b2;
	//b1 = AutoAssemble(pid, PutAsWill_Assemble, command);
	//b2 = AutoAssemble(pid, PutWaterAble_Assemble, command);
	//return b1 && b2;
	AutoAssemble(pid, PutAsWill_Assemble, command);
	AutoAssemble(pid, PutWaterAble_Assemble, command);
	return true;
}


char* AutoRepair_Assemble = "\
[ENABLE]\n\
//code from here to '[DISABLE]' will be used to enable the cheat\n\
alloc(newmem,2048)\n\
label(returnhere)\n\
label(originalcode)\n\
label(exit)\n\
newmem: //this is allocated memory, you have read,write,execute access\n\
//place your code here\n\
mov ecx,05\n\
originalcode:\n\
cmp ecx,eax\n\
jl 00450813\n\
exit:\n\
jmp returnhere\n\
\"gamemd.exe\"+50651:\n\
jmp newmem\n\
nop\n\
nop\n\
nop\n\
returnhere:\n\
[DISABLE]\n\
//code from here till the end of the code will be used to disable the cheat\n\
dealloc(newmem)\n\
\"gamemd.exe\"+50651:\n\
cmp ecx,eax\n\
jl 00450813\n\
//Alt: db 3B C8 0F 8C BA 01 00 00\n\
";
char* AutoRepairNeutral_Assemble = "\
[ENABLE]\n\
//code from here to '[DISABLE]' will be used to enable the cheat\n\
alloc(newmem,256)\n\
label(returnhere)\n\
label(originalcode)\n\
label(exit)\n\
newmem: //this is allocated memory, you have read,write,execute access\n\
//place your code here\n\
mov ecx,[00A83D4C]\n\
mov edx,[esi+21C]\n\
cmp ecx,edx\n\
jne originalcode//不是玩家照常\n\
mov cl,1//玩家都能??\n\
originalcode:\n\
test cl,cl\n\
jne 00452652\n\
xor al,al\n\
exit:\n\
jmp returnhere\n\
\"gamemd.exe\"+5264A:\n\
jmp newmem\n\
nop\n\
returnhere:\n\
[DISABLE]\n\
//code from here till the end of the code will be used to disable the cheat\n\
dealloc(newmem)\n\
\"gamemd.exe\"+5264A:\n\
test cl,cl\n\
jne 00452652\n\
xor al,al\n\
//Alt: db 84 C9 75 04 32 C0\n\
";

bool TrainerFunctions::AutoRepair(int command) //自动修理
{
	bool b1, b2;
	b1 = AutoAssemble(pid, AutoRepair_Assemble, command);
	b2 = AutoAssemble(pid, AutoRepairNeutral_Assemble, command);
	return b1 && b2;
}

char* RevengeYuri_Assemble = "\
[ENABLE]\n\
//code from here to '[DISABLE]' will be used to enable the cheat\n\
alloc(newmem, 2048)\n\
label(unitlist)\n\
label(cheat)\n\
label(returnhere)\n\
label(originalcode)\n\
label(exit)\n\
newmem: //this is allocated memory, you have read,write,execute access\n\
//place your code here\n\
pushad\n\
cmp eax,[00A83D4C]\n\
je originalcode//是玩家就不变\n\
//解决怀疑bug: 控制方在执行到这里前阵亡就会出错\n\
mov eax,0\n\
//mov ecx,ecx//单位地址\n\
mov ebx,[00A8EC7C]//所有单位数组首地址\n\
unitlist:\n\
cmp eax,[00A8EC88]//遍历所有单位\n\
jge originalcode//>=没有or阵亡\n\
cmp ecx,[ebx+eax*4]//存在\n\
je cheat\n\
inc eax\n\
jmp unitlist\n\
cheat:\n\
popad\n\
mov eax,[00A83D4C]\n\
push eax\n\
//mov ecx,esi//ecx是控制方标识\n\
mov ebp,[ecx]//获取单位类型\n\
call dword ptr [ebp+000003D4]\n\
jmp 00471DC2//直接退出不触发动画\n\
originalcode:\n\
popad\n\
mov eax,[00A83D4C]\n\
push eax\n\
mov ecx,esi\n\
call dword ptr [ebp+000003D4]\n\
exit:\n\
jmp returnhere\n\
\"gamemd.exe\"+71DB5:\n\
jmp newmem\n\
nop\n\
nop\n\
nop\n\
nop\n\
returnhere:\n\
[DISABLE]\n\
//code from here till the end of the code will be used to disable the cheat\n\
dealloc(newmem)\n\
\"gamemd.exe\"+71DB5:\n\
push eax\n\
mov ecx,esi\n\
call dword ptr[ebp+000003D4]\n\
//Alt: db 50 8B CE FF 95 D4 03 00 00\n\
";
char* PermanentYuri_Assemble = "\
[ENABLE]\n\
//code from here to '[DISABLE]' will be used to enable the cheat\n\
alloc(newmem,2048)\n\
label(returnhere)\n\
label(originalcode)\n\
label(exit)\n\
newmem: //this is allocated memory, you have read,write,execute access\n\
//place your code here\n\
mov eax,[00A83D4C]\n\
originalcode:\n\
push eax\n\
mov ecx,esi\n\
call dword ptr [edx+000003D4]\n\
exit:\n\
jmp returnhere\n\
\"gamemd.exe\"+720D7:\n\
jmp newmem\n\
nop\n\
nop\n\
nop\n\
nop\n\
returnhere:\n\
[DISABLE]\n\
//code from here till the end of the code will be used to disable the cheat\n\
dealloc(newmem)\n\
\"gamemd.exe\"+720D7:\n\
push eax\n\
mov ecx,esi\n\
call dword ptr [edx+000003D4]\n\
//Alt: db 50 8B CE FF 92 D4 03 00 00\n\
";
bool TrainerFunctions::RevengeYuri(int command) //反控制
{
	bool b1, b2;
	b1 = AutoAssemble(pid, RevengeYuri_Assemble, command);
	b2 = AutoAssemble(pid, PermanentYuri_Assemble, command);
	return b1 && b2;
}

char* MineAttack_Assemble = "\
[ENABLE]\n\
//code from here to '[DISABLE]' will be used to enable the cheat\n\
alloc(newmem,2048)\n\
label(unitlist)\n\
label(cheat)\n\
label(returnhere)\n\
label(originalcode)\n\
label(exit)\n\
newmem: //this is allocated memory, you have read,write,execute access\n\
//place your code here\n\
pushad\n\
mov eax,0\n\
mov ebx,[esi+B0]//单位地址\n\
mov ecx,[00A8EC7C]//所有单位数组首地址\n\
unitlist:\n\
cmp eax,[00A8EC88]//遍历所有单??\n\
jge originalcode//>=没有or阵亡\n\
cmp ebx,[ecx+eax*4]//存在\n\
je cheat\n\
inc eax\n\
jmp unitlist\n\
cheat:\n\
push 0  //提示??\n\
mov eax,[00A83D4C]\n\
push eax\n\
mov ecx,[esi+B0]\n\
mov ebx,[ecx]\n\
call [ebx+3D4]\n\
jmp originalcode\n\
originalcode:\n\
popad\n\
mov eax,[ebp+08]\n\
mov edx,[eax]\n\
exit:\n\
jmp returnhere\n\
\"gamemd.exe\"+69AA4:\n\
jmp newmem\n\
returnhere:\n\
[DISABLE]\n\
//code from here till the end of the code will be used to disable the cheat\n\
dealloc(newmem)\n\
\"gamemd.exe\"+69AA4:\n\
mov eax,[ebp+08]\n\
mov edx,[eax]\n\
//Alt: db 8B 45 08 8B 10\n\
";
bool TrainerFunctions::MineAttack(int command) //攻击反控
{
	return AutoAssemble(pid, MineAttack_Assemble, command);
}

char* MineBuildIn_Assemble = "\
[ENABLE]\n\
//code from here to '[DISABLE]' will be used to enable the cheat\n\
alloc(newmem,2048)\n\
label(returnhere)\n\
label(originalcode)\n\
label(exit)\n\
newmem: //this is allocated memory, you have read,write,execute access\n\
//place your code here\n\
mov eax,[00A83D4C]\n\
originalcode:\n\
push eax\n\
call dword ptr [edx+000003D4]\n\
exit:\n\
jmp returnhere\n\
\"gamemd.exe\"+58322:\n\
jmp newmem\n\
nop\n\
nop\n\
returnhere:\n\
[DISABLE]\n\
//code from here till the end of the code will be used to disable the cheat\n\
dealloc(newmem)\n\
\"gamemd.exe\"+58322:\n\
push eax\n\
call dword ptr [edx+000003D4]\n\
//Alt: db 50 FF 92 D4 03 00 00\n\
";

bool TrainerFunctions::MineBuildIn(int command) //进房反控
{
	return AutoAssemble(pid, MineBuildIn_Assemble, command);
}

char* MineUnEngineer_Assemble = "\
[ENABLE]\n\
//code from here to '[DISABLE]' will be used to enable the cheat\n\
alloc(newmem,2048)\n\
label(returnhere)\n\
label(originalcode)\n\
label(exit)\n\
newmem: //this is allocated memory, you have read,write,execute access\n\
//place your code here\n\
mov eax,[00A83D4C]\n\
originalcode:\n\
push eax\n\
mov ecx,edi\n\
call dword ptr [edx+000003D4]\n\
exit:\n\
jmp returnhere\n\
\"gamemd.exe\"+119F7B:\n\
jmp newmem\n\
nop\n\
nop\n\
nop\n\
nop\n\
returnhere:\n\
[DISABLE]\n\
//code from here till the end of the code will be used to disable the cheat\n\
dealloc(newmem)\n\
\"gamemd.exe\"+119F7B:\n\
push eax\n\
mov ecx,edi\n\
call dword ptr [edx+000003D4]\n\
//Alt: db 50 8B CF FF 92 D4 03 00 00\n\
";
bool TrainerFunctions::MineUnEngineer(int command) //反工程师占领
{
	return AutoAssemble(pid, MineUnEngineer_Assemble, command);
}

char* BuildImme_Assemble = "\
[ENABLE]\n\
//code from here to '[DISABLE]' will be used to enable the cheat\n\
alloc(newmem,2048)\n\
label(returnhere)\n\
label(cheat)\n\
label(originalcode)\n\
label(exit)\n\
newmem: //this is allocated memory, you have read,write,execute access\n\
//place your code here\n\
push eax\n\
push ebx\n\
mov eax,[00A83D4C]      //玩家数据地址\n\
mov ebx,dword ptr [eax+53AC]//飞机\n\
cmp ebx,esi\n\
jz cheat       //是玩??\n\
mov ebx,dword ptr [eax+53B0]//步兵\n\
cmp ebx,esi\n\
jz cheat       //是玩??\n\
mov ebx,dword ptr [eax+53B4]//战车\n\
cmp ebx,esi\n\
jz cheat       //是玩??\n\
mov ebx,dword ptr [eax+53B8]//船只\n\
cmp ebx,esi\n\
jz cheat       //是玩??\n\
mov ebx,dword ptr [eax+53BC]//建筑\n\
cmp ebx,esi\n\
jz cheat       //是玩??\n\
mov ebx,dword ptr [eax+53CC]//武器\n\
cmp ebx,esi\n\
jz cheat       //是玩??\n\
jmp originalcode\n\
cheat:\n\
mov [esi+24],36     //修改！！\n\
jmp exit\n\
originalcode:\n\
mov [esi+24],edx\n\
exit:\n\
pop ebx\n\
pop eax\n\
mov edx,[esp+10]\n\
jmp returnhere\n\
\"gamemd.exe\"+C9B82:\n\
jmp newmem\n\
nop\n\
nop\n\
returnhere:\n\
[DISABLE]\n\
//code from here till the end of the code will be used to disable the cheat\n\
dealloc(newmem)\n\
\"gamemd.exe\"+C9B82:\n\
mov [esi+24],edx\n\
mov edx,[esp+10]\n\
//Alt: db 89 56 24 8B 54 24 10\n\
";
bool TrainerFunctions::BuildImme(int command) //瞬间建造
{
	return AutoAssemble(pid, BuildImme_Assemble, command);
}

char* Unbeatable_Assemble = "\
[ENABLE]\n\
//code from here to '[DISABLE]' will be used to enable the cheat\n\
alloc(newmem,2048)\n\
label(returnhere)\n\
label(originalcode)\n\
label(exit)\n\
newmem: //this is allocated memory, you have read,write,execute access\n\
//place your code here\n\
push ecx\n\
mov ecx,[00A83D4C]      //玩家数据地址\n\
//mov ecx,dword ptr [ecx+0]\n\
//判断受伤害单位是否属于玩??\n\
//esi+21c指向玩家数据基址\n\
cmp ecx,[esi+21C]\n\
pop ecx\n\
jnz originalcode       //跳转\n\
sub eax,0//is player\n\
jmp exit\n\
originalcode:\n\
sub eax,edx\n\
exit:\n\
test eax,eax\n\
mov [esi+6C],eax\n\
jmp returnhere\n\
\"gamemd.exe\"+1F5509:  //伤害计算地址\n\
jmp newmem\n\
nop\n\
nop\n\
returnhere:\n\
[DISABLE]\n\
//code from here till the end of the code will be used to disable the cheat\n\
dealloc(newmem)\n\
\"gamemd.exe\"+1F5509:\n\
sub eax,edx\n\
test eax,eax\n\
mov [esi+6C],eax\n\
//Alt: db 2B C2 85 C0 89 46 6C\n\
";
bool TrainerFunctions::Unbeatable(int command)//无敌实现函数
{
	return AutoAssemble(pid, Unbeatable_Assemble, command);
}

char* AntiChrono_Assemble = "\
[ENABLE]\n\
//code from here to '[DISABLE]' will be used to enable the cheat\n\
alloc(newmem,128)\n\
label(returnhere)\n\
label(originalcode)\n\
label(exit)\n\
newmem: //this is allocated memory, you have read,write,execute access\n\
//place your code here\n\
push eax\n\
mov eax,[esi+24]  //发动超时空单??\n\
mov eax,[eax+21C]\n\
cmp eax,[00A83D4C]\n\
jne exit\n\
originalcode:\n\
mov [esi+48],ecx  //时间计算后赋??\n\
exit:\n\
pop eax\n\
cmp eax,ebx\n\
jmp returnhere\n\
\"gamemd.exe\"+31A88A:\n\
jmp newmem\n\
returnhere:\n\
[DISABLE]\n\
//code from here till the end of the code will be used to disable the cheat\n\
dealloc(newmem)\n\
\"gamemd.exe\"+31A88A:\n\
mov [esi+48],ecx\n\
cmp eax,ebx\n\
//Alt: db 89 4E 48 3B C3\n\
";
char* AntiChronoDisbuild_Assemble = "\
[ENABLE]\n\
//code from here to '[DISABLE]' will be used to enable the cheat\n\
alloc(newmem,128)\n\
label(returnhere)\n\
label(originalcode)\n\
label(exit)\n\
newmem: //this is allocated memory, you have read,write,execute access\n\
//place your code here\n\
mov byte ptr [ecx+00000660],01\n\
mov eax,[ecx+21C]\n\
cmp eax,[00A83D4C]\n\
je exit\n\
originalcode:\n\
mov byte ptr [ecx+00000660],00\n\
exit:\n\
jmp returnhere\n\
\"gamemd.exe\"+521C0:\n\
jmp newmem\n\
nop\n\
nop\n\
returnhere:\n\
[DISABLE]\n\
//code from here till the end of the code will be used to disable the cheat\n\
dealloc(newmem)\n\
\"gamemd.exe\"+521C0:\n\
mov byte ptr [ecx+00000660],00\n\
//Alt: db C6 81 60 06 00 00 00\n\
";
bool TrainerFunctions::AntiChrono(int command) //反超时空攻击
{
	bool b1, b2;
	b1 = AutoAssemble(pid, AntiChrono_Assemble, command);			//超时空冷却无限长
	b2 = AutoAssemble(pid, AntiChronoDisbuild_Assemble, command);	//超时空过程允许建造
	return b1 && b2;
}

bool TrainerFunctions::AntiChronoDisable(int command) //超时空过程建造栏不变灰色
{
	if (command == 1) {
		writeMemory((0x006A97B2  ),0x90,1);		//0x90->nop
		writeMemory((0x006A97B2+1),0x90,1);		//0x90->nop
	}
	else {
		DWORD add_l = 0x006A97B2;
		BYTE jmp_l[2] ={0x74,0X47};
		WriteProcessMemory(pid,(LPVOID)add_l,&jmp_l,sizeof(jmp_l),0);
	}
	return true;	// 容我粗糙一下...
}


char* UnRepair_Assemble = "\
[ENABLE]\n\
//code from here to '[DISABLE]' will be used to enable the cheat\n\
alloc(newmem,2048)\n\
label(returnhere)\n\
label(originalcode)\n\
label(exit)\n\
newmem: //this is allocated memory, you have read,write,execute access\n\
//place your code here\n\
push eax\n\
mov eax,[00A83D4C]\n\
cmp eax,[esi+21C]\n\
pop eax\n\
jz originalcode\n\
sub ebx,ebp\n\
cmp ebx,0 //ebx>0 ?\n\
//jge //>=0\n\
jge exit\n\
mov ebx,0\n\
jmp exit\n\
originalcode:\n\
add ebx,ebp\n\
exit:\n\
add edi,ebp\n\
mov [esi+6C],ebx\n\
jmp returnhere\n\
\"gamemd.exe\"+508B4:\n\
jmp newmem\n\
nop\n\
nop\n\
returnhere:\n\
[DISABLE]\n\
//code from here till the end of the code will be used to disable the cheat\n\
dealloc(newmem)\n\
\"gamemd.exe\"+508B4:\n\
add ebx,ebp\n\
add edi,ebp\n\
mov [esi+6C],ebx\n\
//Alt: db 01 EB 01 EF 89 5E 6C\n\
";
bool TrainerFunctions::UnRepair(int command) //反修
{
	return AutoAssemble(pid, UnRepair_Assemble, command);
}

bool TrainerFunctions::InvadeMode(int command) //不删除建造选项-不需要注入，直接替换
{
	if (command == 1) {
		jmpWritProc((LPVOID)0x006F85DD,(LPVOID)0x006F8604);
	}
	else {
		DWORD add_l = 0x006F85DD;
		BYTE jmp_l[5] = { 0x83,0x38,0x00,0x74,0x0E };
		WriteProcessMemory(pid, (LPVOID)add_l, &jmp_l, sizeof(jmp_l), 0);
	}
	return true;	// 容我粗糙一下...
}

char* UnlockTech_Assemble = "\
[ENABLE]\n\
//code from here to '[DISABLE]' will be used to enable the cheat\n\
alloc(newmem,64)\n\
label(returnhere)\n\
label(originalcode)\n\
label(exit)\n\
newmem: //this is allocated memory, you have read,write,execute access\n\
//place your code here\n\
jmp 004F7B96\n\
originalcode:\n\
mov ecx,[00A8B238]\n\
exit:\n\
jmp returnhere\n\
\"gamemd.exe\"+F7B6C:\n\
jmp newmem\n\
nop\n\
returnhere:\n\
[DISABLE]\n\
//code from here till the end of the code will be used to disable the cheat\n\
dealloc(newmem)\n\
\"gamemd.exe\"+F7B6C:\n\
mov ecx,[00A8B238]\n\
//Alt: db 8B 0D 38 B2 A8 00\n\
";
bool TrainerFunctions::UnlockTech(int command) //飞机全科技
{
	return AutoAssemble(pid, UnlockTech_Assemble, command);
}

char* FastAttack_Assemble = "\
[ENABLE]\n\
//code from here to '[DISABLE]' will be used to enable the cheat\n\
alloc(newmem,256)\n\
label(returnhere)\n\
label(originalcode)\n\
label(exit)\n\
newmem: //this is allocated memory, you have read,write,execute access\n\
//place your code here\n\
mov eax,[esi+21C]\n\
cmp eax,[00A83D4C]\n\
jne originalcode\n\
jmp 006FC981      //强制下一次攻??\n\
originalcode:\n\
mov eax,[esi+000002F4]\n\
exit:\n\
jmp returnhere\n\
\"gamemd.exe\"+2FC955:\n\
jmp newmem\n\
nop\n\
returnhere:\n\
[DISABLE]\n\
//code from here till the end of the code will be used to disable the cheat\n\
dealloc(newmem)\n\
\"gamemd.exe\"+2FC955:\n\
mov eax,[esi+000002F4]\n\
//Alt: db 8B 86 F4 02 00 00\n\
";
bool TrainerFunctions::FastAttack(int command) //极速攻击
{
	return AutoAssemble(pid, FastAttack_Assemble, command);
}

char* FastTurnBattery_Assemble = "\
[ENABLE]\n\
//code from here to '[DISABLE]' will be used to enable the cheat\n\
alloc(newmem,256)\n\
label(returnhere)\n\
label(unitlist)\n\
label(cheat)\n\
label(originalcode)\n\
label(exit)\n\
newmem: //this is allocated memory, you have read,write,execute access\n\
//place your code here\n\
pushad\n\
mov eax,0\n\
mov ebx,[00A8EC7C]//所有单位数组首地址\n\
unitlist:\n\
cmp eax,[00A8EC88]//遍历所有单??\n\
jge originalcode//>=没有or阵亡\n\
cmp ebp,[ebx+eax*4]//存在\n\
je cheat\n\
inc eax\n\
jmp unitlist\n\
cheat:\n\
mov edx,[ebp+21C]\n\
cmp edx,[00A83D4C]\n\
jne exit\n\
mov [ecx+08],0\n\
originalcode:\n\
exit:\n\
popad\n\
mov edx,[ecx+10]\n\
mov ecx,[ecx+08]\n\
jmp returnhere\n\
\"gamemd.exe\"+C941E:\n\
jmp newmem\n\
nop\n\
returnhere:\n\
[DISABLE]\n\
//code from here till the end of the code will be used to disable the cheat\n\
dealloc(newmem)\n\
\"gamemd.exe\"+C941E:\n\
mov edx,[ecx+10]\n\
mov ecx,[ecx+08]\n\
//Alt: db 8B 51 10 8B 49 08\n\
";
bool TrainerFunctions::FastTurnBattery(int command) //极速转炮塔
{
	return AutoAssemble(pid, FastTurnBattery_Assemble, command);
}

char* FastTurnRound_Assemble = "\
[ENABLE]\n\
//code from here to '[DISABLE]' will be used to enable the cheat\n\
alloc(newmem,256)\n\
label(returnhere)\n\
label(originalcode)\n\
label(exit)\n\
newmem: //this is allocated memory, you have read,write,execute access\n\
//place your code here\n\
lea esi,[ecx-388]\n\
mov edi,[esi+21C]\n\
cmp edi,[00A83D4C]\n\
je 004C92EB\n\
test bp,bp\n\
originalcode:\n\
jle 004C92EB\n\
mov ax,dx\n\
exit:\n\
jmp returnhere\n\
\"gamemd.exe\"+C92BF:\n\
jmp newmem\n\
returnhere:\n\
[DISABLE]\n\
//code from here till the end of the code will be used to disable the cheat\n\
dealloc(newmem)\n\
\"gamemd.exe\"+C92BF:\n\
jle 004C92EB\n\
mov ax,dx\n\
//Alt: db 7E 2A 66 8B C2\n\
";
bool TrainerFunctions::FastTurnRound(int command) //极速转身
{
	return AutoAssemble(pid, FastTurnRound_Assemble, command);
}

char* FastReload_Assemble = "\
[ENABLE]\n\
//code from here to '[DISABLE]' will be used to enable the cheat\n\
alloc(newmem,256)\n\
label(returnhere)\n\
label(originalcode)\n\
label(exit)\n\
newmem: //this is allocated memory, you have read,write,execute access\n\
//place your code here\n\
mov ecx,[00A83D4C]\n\
cmp ecx,[esi+21C]\n\
jne originalcode\n\
mov eax,0A\n\
jmp exit\n\
originalcode:\n\
mov eax,[eax+00000D60]//弹药重生间隔\n\
exit:\n\
jmp returnhere\n\
\"gamemd.exe\"+2F3FC8:\n\
jmp newmem\n\
nop\n\
returnhere:\n\
[DISABLE]\n\
//code from here till the end of the code will be used to disable the cheat\n\
dealloc(newmem)\n\
\"gamemd.exe\"+2F3FC8:\n\
mov eax,[eax+00000D60]\n\
//Alt: db 8B 80 60 0D 00 00\n\
";
bool TrainerFunctions::FastReload(int command) //闪电重装
{
	return AutoAssemble(pid, FastReload_Assemble, command);
}

char* FullAmmunition_Assemble = "\
[ENABLE]\n\
//code from here to '[DISABLE]' will be used to enable the cheat\n\
alloc(newmem,2048)\n\
label(returnhere)\n\
label(originalcode)\n\
label(exit)\n\
newmem: //this is allocated memory, you have read,write,execute access\n\
//place your code here\n\
mov ecx,[00A83D4C]\n\
cmp ecx,[esi+21C]\n\
jne originalcode\n\
mov eax,0A\n\
jmp exit\n\
originalcode:\n\
mov eax,[eax+00000D5C]//弹药??\n\
exit:\n\
jmp returnhere\n\
\"gamemd.exe\"+2F3FD9:\n\
jmp newmem\n\
nop\n\
returnhere:\n\
[DISABLE]\n\
//code from here till the end of the code will be used to disable the cheat\n\
dealloc(newmem)\n\
\"gamemd.exe\"+2F3FD9:\n\
mov eax,[eax+00000D5C]\n\
//Alt: db 8B 80 5C 0D 00 00\n\
";
bool TrainerFunctions::FullAmmunition(int command) //弹药充足
{
	return AutoAssemble(pid, FullAmmunition_Assemble, command);
}

char* AllRangeAttack_Assemble = "\
[ENABLE]\n\
//code from here to '[DISABLE]' will be used to enable the cheat\n\
alloc(newmem,256)\n\
label(returnhere)\n\
label(originalcode)\n\
label(exit)\n\
newmem: //this is allocated memory, you have read,write,execute access\n\
//place your code here\n\
push ecx\n\
mov ecx,[00A83D4C]\n\
cmp ecx,[esi+21C]\n\
pop ecx\n\
jne originalcode\n\
mov edi,F900\n\
jmp exit\n\
originalcode:\n\
mov edi,[ebx+000000B4]\n\
exit:\n\
jmp returnhere\n\
\"gamemd.exe\"+2F7248:\n\
jmp newmem\n\
nop\n\
returnhere:\n\
[DISABLE]\n\
//code from here till the end of the code will be used to disable the cheat\n\
dealloc(newmem)\n\
\"gamemd.exe\"+2F7248:\n\
mov edi,[ebx+000000B4]\n\
//Alt: db 8B BB B4 00 00 00\n\
";
bool TrainerFunctions::AllRangeAttack(int command) //远程打击
{
	return AutoAssemble(pid, AllRangeAttack_Assemble, command);
}

char* AllRangeAlert_Assemble = "\
[ENABLE]\n\
//code from here to '[DISABLE]' will be used to enable the cheat\n\
alloc(newmem,256)\n\
label(returnhere)\n\
label(originalcode)\n\
label(exit)\n\
newmem: //this is allocated memory, you have read,write,execute access\n\
//place your code here\n\
mov ecx,[00A83D4C]\n\
cmp ecx,[esi+21C]\n\
jne originalcode\n\
mov ebx,F900//警戒范围F9 249\n\
jmp exit\n\
originalcode:\n\
mov ebx,[eax+000000B4]\n\
exit:\n\
jmp returnhere\n\
\"gamemd.exe\"+3012DF:\n\
jmp newmem\n\
nop\n\
returnhere:\n\
[DISABLE]\n\
//code from here till the end of the code will be used to disable the cheat\n\
dealloc(newmem)\n\
\"gamemd.exe\"+3012DF:\n\
mov ebx,[eax+000000B4]\n\
//Alt: db 8B 98 B4 00 00 00\n\
";
bool TrainerFunctions::AllRangeAlert(int command) //远程警戒
{
	return AutoAssemble(pid, AllRangeAlert_Assemble, command);
}

char* InstantChronoMove_Assemble = "\
[ENABLE]\n\
//code from here to '[DISABLE]' will be used to enable the cheat\n\
alloc(newmem,128)\n\
label(returnhere)\n\
label(originalcode)\n\
label(exit)\n\
newmem: //this is allocated memory, you have read,write,execute access\n\
//place your code here\n\
mov edx,[ecx+21C]\n\
cmp edx,[00A83D4C]\n\
jne originalcode\n\
mov byte ptr [ecx+00000271],00\n\
jmp exit\n\
originalcode:\n\
mov byte ptr [ecx+00000271],01\n\
exit:\n\
jmp returnhere\n\
\"gamemd.exe\"+319579:\n\
jmp newmem\n\
nop\n\
nop\n\
returnhere:\n\
[DISABLE]\n\
//code from here till the end of the code will be used to disable the cheat\n\
dealloc(newmem)\n\
\"gamemd.exe\"+319579:\n\
mov byte ptr [ecx+00000271],01\n\
//Alt: db C6 81 71 02 00 00 01\n\
";
bool TrainerFunctions::InstantChronoMove(int command) //瞬间超时空移动
{
	return AutoAssemble(pid, InstantChronoMove_Assemble, command);
}

char* InstantChronoAttack_Assemble = "\
[ENABLE]\n\
//code from here to '[DISABLE]' will be used to enable the cheat\n\
alloc(newmem,256)\n\
label(returnhere)\n\
label(originalcode)\n\
label(exit)\n\
newmem: //this is allocated memory, you have read,write,execute access\n\
//place your code here\n\
push eax\n\
mov eax,[esi+24]//发动攻击单位指针\n\
mov eax,[eax+21C]\n\
cmp [00A83D4C],eax\n\
pop eax\n\
jne originalcode\n\
mov [esi+48],0\n\
jmp exit\n\
originalcode:\n\
mov [esi+48],eax\n\
exit:\n\
je 0071B064\n\
jmp returnhere\n\
\"gamemd.exe\"+31AFB9:\n\
jmp newmem\n\
nop\n\
nop\n\
nop\n\
nop\n\
returnhere:\n\
[DISABLE]\n\
//code from here till the end of the code will be used to disable the cheat\n\
dealloc(newmem)\n\
\"gamemd.exe\"+31AFB9:\n\
mov [esi+48],eax\n\
je 0071B064\n\
//Alt: db 89 46 48 0F 84 A2 00 00 00\n\
";
bool TrainerFunctions::InstantChronoAttack(int command) //瞬间超时空攻击
{
	return AutoAssemble(pid, InstantChronoAttack_Assemble, command);
}

char* AntiSpy_Assemble = "\
[ENABLE]\n\
//code from here to '[DISABLE]' will be used to enable the cheat\n\
alloc(newmem,256)\n\
label(returnhere)\n\
label(originalcode)\n\
label(exit)\n\
newmem: //this is allocated memory, you have read,write,execute access\n\
//place your code here\n\
cmp eax,[00A83D4C]\n\
je originalcode\n\
mov ebx,[edi+21C]//保存玩家指针\n\
mov [edi+21C],eax//暂时转给敌方\n\
mov eax,[00A83D4C]//间谍指针为玩??\n\
push eax\n\
call 004571E0\n\
mov [edi+21C],ebx//所属回??\n\
jmp exit\n\
originalcode:\n\
push eax\n\
call 004571E0\n\
exit:\n\
jmp returnhere\n\
\"gamemd.exe\"+11A00A:\n\
jmp newmem\n\
nop\n\
returnhere:\n\
[DISABLE]\n\
//code from here till the end of the code will be used to disable the cheat\n\
dealloc(newmem)\n\
\"gamemd.exe\"+11A00A:\n\
push eax\n\
call 004571E0\n\
//Alt: db 50 E8 D0 D1 F3 FF\n\
";
bool TrainerFunctions::AntiSpy(int command) //间间谍
{
	return AutoAssemble(pid, AntiSpy_Assemble, command);
}


char* DisableGAGAP_Assemble = "\
[ENABLE]\n\
//code from here to '[DISABLE]' will be used to enable the cheat\n\
alloc(newmem,256)\n\
label(returnhere)\n\
label(originalcode)\n\
label(exit)\n\
newmem: //this is allocated memory, you have read,write,execute access\n\
//place your code here\n\
mov eax,[esi+21C]\n\
cmp eax,[00A83D4C]\n\
je originalcode   //是玩??\n\
mov eax,[esi]\n\
cmp eax,007E3EBC\n\
jne originalcode  //不是建筑\n\
mov eax,[esi+520]\n\
mov eax,[eax+DF8]\n\
cmp eax,3F\n\
jne originalcode  //不是裂缝产生??\n\
mov eax,1\n\
jmp 006FAF1C\n\
originalcode:\n\
mov eax,[esi+00000504]\n\
exit:\n\
jmp returnhere\n\
\"gamemd.exe\"+2FAF0D:\n\
jmp newmem\n\
nop\n\
returnhere:\n\
[DISABLE]\n\
//code from here till the end of the code will be used to disable the cheat\n\
dealloc(newmem)\n\
\"gamemd.exe\"+2FAF0D:\n\
mov eax,[esi+00000504]\n\
//Alt: db 8B 86 04 05 00 00\n\
";
bool TrainerFunctions::DisableGAGAP(int command) //瘫痪裂缝产生器
{
	return AutoAssemble(pid, DisableGAGAP_Assemble, command);
}

char* DisableAll_Assemble = "\
[ENABLE]\n\
//code from here to '[DISABLE]' will be used to enable the cheat\n\
alloc(newmem,256)\n\
label(returnhere)\n\
label(originalcode)\n\
label(exit)\n\
newmem: //this is allocated memory, you have read,write,execute access\n\
//place your code here\n\
mov eax,[esi+21C]\n\
cmp eax,[00A83D4C]\n\
je originalcode\n\
mov eax,1\n\
jmp 006FAF1C\n\
originalcode:\n\
mov eax,[esi+504]\n\
test eax,eax\n\
jng 006FAFFD\n\
exit:\n\
jmp returnhere\n\
\"gamemd.exe\"+2FAF15:\n\
jmp newmem\n\
nop\n\
returnhere:\n\
[DISABLE]\n\
//code from here till the end of the code will be used to disable the cheat\n\
dealloc(newmem)\n\
\"gamemd.exe\"+2FAF15:\n\
jng 006FAFFD\n\
//Alt: db 0F 8E E2 00 00 00\n\
";
bool TrainerFunctions::DisableAll(int command) //瘫痪敌方所有单位
{
	return AutoAssemble(pid, DisableAll_Assemble, command);
}

bool TrainerFunctions::SpeedSet(int command) //调速
{
	if (command == 1) {
		jmpWritProc((LPVOID)0x0055D76E, (LPVOID)0x0055D79E); //jmp write address
		writeMemory((0x0055D76E + 5), 0x90, 1);		//0x90->nop
	}
	else {
		DWORD add_l = 0x0055D76E;
		BYTE jmp_l[6] = { 0x75,0x2E,0x8D,0x4C,0x24,0x14 };
		WriteProcessMemory(pid, (LPVOID)add_l, &jmp_l, sizeof(jmp_l), 0);
	}
	return true;	// 容我粗糙一下...
}

char* EnableSoldAll_Cursor_Assemble = "\
[ENABLE]\n\
//code from here to '[DISABLE]' will be used to enable the cheat\n\
alloc(newmem,256)\n\
label(returnhere)\n\
label(originalcode)\n\
label(exit)\n\
newmem: //this is allocated memory, you have read,write,execute access\n\
//place your code here\n\
//jmp 006929FC\n\
//type judge\n\
mov edx,[esi]\n\
mov ecx,esi\n\
call dword ptr [edx+2C]\n\
cmp eax,24\n\
je originalcode\n\
//enable value is 0C disable is 0E\n\
mov eax,0C\n\
jmp 00692A20\n\
originalcode:\n\
mov eax,[esi]\n\
mov ecx,esi\n\
call dword ptr [eax+3C]\n\
exit:\n\
jmp returnhere\n\
\"gamemd.exe\"+2929D1:\n\
jmp newmem\n\
nop\n\
nop\n\
returnhere:\n\
[DISABLE]\n\
//code from here till the end of the code will be used to disable the cheat\n\
dealloc(newmem)\n\
\"gamemd.exe\"+2929D1:\n\
mov eax,[esi]\n\
mov ecx,esi\n\
call dword ptr [eax+3C]\n\
//Alt: db 8B 06 8B CE FF 50 3C\n\
";
bool TrainerFunctions::EnableSoldAll_Cursor(int command) //允许玩家售卖所有单位
{
	return AutoAssemble(pid, EnableSoldAll_Cursor_Assemble, command);
}

bool TrainerFunctions::EnableSoldAll_Belong(int command) //允许玩家售卖所有单位-单位所属
{
	if (command == 1) {
		jmpWritProc((LPVOID)0x004C6F48, (LPVOID)0x004C6F9C); //jmp write address
		writeMemory((0x004C6F48 + 5), 0x90, 1);		//0x90->nop
	}
	else {
		DWORD add_l = 0x004C6F48;
		BYTE jmp_l[6] = { 0x0F,0x85,0xBB,0x11,0x00,0x00 };
		WriteProcessMemory(pid, (LPVOID)add_l, &jmp_l, sizeof(jmp_l), 0);
	}
	return true;	// 容我粗糙一下...
}

bool TrainerFunctions::EnableSoldAll_Builder(int command) //允许玩家售卖所有单位-单位建造者
{
	if (command == 1) {
		writeMemory((0x0044711B), 0x90, 1);		//0x90->nop
		writeMemory((0x0044711B + 1), 0x90, 1);		//0x90->nop
		writeMemory((0x0044711B + 2), 0x90, 1);		//0x90->nop
		writeMemory((0x0044711B + 3), 0x90, 1);		//0x90->nop
		writeMemory((0x0044711B + 4), 0x90, 1);		//0x90->nop
		writeMemory((0x0044711B + 5), 0x90, 1);		//0x90->nop
	}
	else {
		DWORD add_l = 0x0044711B;
		BYTE jmp_l[6] = { 0x0F,0x84,0xA4,0x00,0x00,0x00 };
		WriteProcessMemory(pid, (LPVOID)add_l, &jmp_l, sizeof(jmp_l), 0);
	}
	return true;	// 容我粗糙一下...
}

char* SoldierFlashMove_Assemble = "\
[ENABLE]\n\
//code from here to '[DISABLE]' will be used to enable the cheat\n\
alloc(newmem,512)\n\
label(returnhere)\n\
label(originalcode)\n\
label(exit)\n\
newmem: //this is allocated memory, you have read,write,execute access\n\
//place your code here\n\
pushad\n\
mov ecx,[esi+8]\n\
mov ecx,[ecx+21C]\n\
cmp ecx,[00A83D4C]\n\
jne originalcode\n\
mov ecx,eax\n\
mov edx,[esi+24]\n\
test edx,edx\n\
je originalcode\n\
mov edx,[esi+28]\n\
test edx,edx\n\
je originalcode\n\
lea eax,[esi+24]\n\
push eax\n\
mov edx,[ecx]\n\
call [edx+1B4]\n\
originalcode:\n\
popad\n\
mov byte ptr [esi+31],00\n\
mov eax,[esi]\n\
exit:\n\
jmp returnhere\n\
\"gamemd.exe\"+35AC93:\n\
jmp newmem\n\
nop\n\
returnhere:\n\
[DISABLE]\n\
//code from here till the end of the code will be used to disable the cheat\n\
dealloc(newmem)\n\
\"gamemd.exe\"+35AC93:\n\
mov byte ptr [esi+31],00\n\
mov eax,[esi]\n\
//Alt: db C6 46 31 00 8B 06\n\
";
bool TrainerFunctions::SoldierFlashMove(int command) //滑板鞋
{
	return AutoAssemble(pid, SoldierFlashMove_Assemble, command);
}

char* AllElite_Assemble = "\
[ENABLE]\n\
//code from here to '[DISABLE]' will be used to enable the cheat\n\
alloc(newmem,256)\n\
label(returnhere)\n\
label(originalcode)\n\
label(exit)\n\
newmem: //this is allocated memory, you have read,write,execute access\n\
//place your code here\n\
push eax\n\
mov eax,[esi+0000021C]\n\
cmp eax,[00A83D4C]\n\
pop eax\n\
jne originalcode\n\
jmp exit\n\
originalcode:\n\
test ah,01\n\
jne 00750025\n\
exit:\n\
jmp returnhere\n\
\"gamemd.exe\"+35001A:\n\
jmp newmem\n\
returnhere:\n\
[DISABLE]\n\
//code from here till the end of the code will be used to disable the cheat\n\
dealloc(newmem)\n\
\"gamemd.exe\"+35001A:\n\
test ah,01\n\
jne 00750025\n\
//Alt: db F6 C4 01 75 06\n\
";
bool TrainerFunctions::AllElite(int command) //部队全部3级
{
	return AutoAssemble(pid, AllElite_Assemble, command);
}

////////////////////////////////////


