#include "aobscan.h"

// HEX 字串转 DWORD
bool _HexStringToDword(string hex, DWORD *retval){
	char * chex = (char *)hex.c_str();

	for(int i = 0; i < hex.size(); i++){
		char c = chex[i];
		if (c >= '0' && c <='9'){
			c -= 0x30;
		}else if (c >= 'A' && c <='F'){
			c -= 0x37;
		}else if (c >= 'a' && c <='f'){
			c -= 0x57;
		}else{

			// c = 0x20;
			return false;
		}
		chex[i] = c;
	}
	int i = hex.size() - 1;
	DWORD _dword = 0;
	char *lpDword = (char *)&_dword;
	while(i >= 0){
		char c = chex[i];
		i--;
		if(i >= 0){
			c = c | (chex[i] << 4);
		}
		lpDword[0] = c;
		lpDword++;
		i--;
	}

	*retval = _dword;

	return true;
}

// 打印 Hex
void _printHex(int a){
	char str[10];
	itoa(a, str, 16);
	MessageBoxA(0,str,0,0);
}

// 带未知字节AOB字串 转 双字节数组
// 未知字节 ?? 或 * 以 0x0100 表示
vector<WORD> _stringToAOBwithUnknown(string aobstring){

	vector<WORD> AOBwU;
	DWORD dwordtemp = 0;

	vector<string> hexTokens;
	bool inHex = false;
	int hexStart = -1;
	int hexLen = 0;
	for(int i = 0; i < aobstring.size(); i++){
		if(aobstring[i] == ' ' || aobstring[i] == '\t'){
			if(inHex){
				inHex = false;
				hexTokens.push_back(aobstring.substr(hexStart, hexLen));
			}
		}else{
			if(!inHex){
				inHex  = true;
				hexStart = i;
				hexLen = 1;
			}else{
				hexLen++;
			}
		}
	
	}
	if(inHex){
		hexTokens.push_back(aobstring.substr(hexStart, hexLen));
	}


	for(int i = 0; i < hexTokens.size(); i++){
		if(string::npos != hexTokens[i].find('?') || string::npos != hexTokens[i].find('*')){
			AOBwU.push_back(0x0100);
			continue;
		}

		if(_HexStringToDword(hexTokens[i],&dwordtemp)){
			AOBwU.push_back((WORD)dwordtemp & 0x00FF);
			
		}else{
			AOBwU.clear();
			return AOBwU;
		}
	}

	return AOBwU;
}

//
/////////////////////////////////////////////////////////////////////



/////////////////////////////////////////////////////////////////////
// 
// Sunday 算法 (Quick Search algorithm)
// http://www-igm.univ-mlv.fr/~lecroq/string/node19.html#SECTION00190
//
// 现改为支持??字节的版本
//
//

// 初始化: 建立 坏字节位移查询表　(Bad-character shift table)
void initShiftTable(WORD *aob, int aobSize, int shiftTable[]) {
	int i, unknownByteShift;
	bool hasUnknown = false;

	for (i = 0; i < SHIFT_TABLE_SIZE; i++){
		shiftTable[i] = aobSize + 1;
	}

	for (i = 0; i < aobSize; i++){
		if(aob[i] != 0x0100){						// 已知字节
			shiftTable[aob[i]] = aobSize - i;
		}else{									// 默认以 0x100 代表 ?? 未知字节
			hasUnknown = true;	
			unknownByteShift = aobSize - i;
		}
	}

	if(hasUnknown){								// 所有位移大于最后一个??字节的, 必须设为 = 最后一个??字节的位移
		for (i = 0; i < SHIFT_TABLE_SIZE; i++){	// 因此??字节越后面, 效率越差
			if(shiftTable[i] >  unknownByteShift){
				shiftTable[i] = unknownByteShift;
			}
		}
	}
}

// Sunday 算法改, 支持??字节
bool SundayUnkownByteMatch(WORD *aob, int aobSize, BYTE *memBuffer, int memBufferSize, int *shiftTable, DWORD *foundPos) {
	int i = 0; 
	int j = 0;
	BYTE *pByte;
	BYTE *pStart = (BYTE *)aob;;

	while (i <= memBufferSize - aobSize) {
		for(j = 0, pByte = pStart; j < aobSize; j++, pByte += 2){
			if(*pByte == memBuffer[i+j] || aob[j] == 0x100){	
				continue;									
			}else{
				break;
			}
		}

		if(j == aobSize){		// 找到
			*foundPos = i;
			return true;
		}else if(i + aobSize < memBufferSize){
			i += shiftTable[memBuffer[i + aobSize]];     // 未找到, 根据位移表进行位移
		}else{
			break;
		}	
	}

	return false;			// 最终失败
}

//
/////////////////////////////////////////////////////////////////////



/////////////////////////////////////////////////////////////////////
//
//     AOBSCAN 主函数
//
//     使用方式:
//
//     aobscan(游戏句柄, 第几个命中目标, 起始地址, 结束地址, AOB, 返回地址)
//
//     1. 返回地址为一个DWORD *指针, 用来返回找到的地址
//     2. 若找到目标, 返回true, 结果写在返回地址里
//	   3. 若没有找到目标, 则返回false
//
//     例:
//
//         DWORD address = 0;													// 存放目标地址用
//         string aob = "e8 75 00 32 ac ?? 05 ?? ?? ff ff 3c 0a";				// 大小写不分, 未知字节以 ?? 或 * 表示皆可
//
//         bool OK  = aobscan(hProcess, 1, 0x0, 0x7fffffff, aob, &address);     // 此为CE使用的标准aobscan设定
//         if(OK){
//
//              // 找到, 返回地址在address
//
//         }else{
//
//			   // 没找到
//         }
//
//
//
bool aobscan(HANDLE processhandle, int Nth_match, DWORD startaddress, DWORD stopaddress, string aobstring, DWORD *foundAddress)
{
	if(Nth_match<1 || startaddress>stopaddress)
		return false;

	vector<WORD> aobtokens = _stringToAOBwithUnknown(aobstring);		// 带??之AOB 转 双字节数组
	DWORD aobSize = aobtokens.size();
	if(aobSize<1)
		return false;

	WORD *aob = new WORD[aobSize];				// 使用传统 [] 数组结构, 其效率比高级结构好
	for(int i = 0; i < aobSize; i++){
		aob[i] = aobtokens[i];
	}

	int shiftTable[SHIFT_TABLE_SIZE];			// 初始化 Sunday 位移表
	initShiftTable(aob, aobSize, shiftTable);

	BYTE *memBuffer = new BYTE[PAGE_SIZE];		// 内存分页缓冲

	DWORD overlapSize = (aobSize-1)*2;			// 专门处理AOB跨分页或跨内存区的小缓冲
	BYTE *overlapBuffer = new BYTE[overlapSize];


	// 以下参考并修改自CE 5.61源码: memscan.pas, procedure TScanController.firstScan;
	// CE源码还使用了多线程, 我这里只是单线程

	MEMORY_BASIC_INFORMATION mbi;
//	MEMORY_BASIC_INFORMATION previousMbi;
//	previousMbi.BaseAddress = 0;
//	previousMbi.RegionSize = 0;

	//DWORD totalProcessMemorySize = 0;
	DWORD actualread = 0;
	int n = 0;
	DWORD offset = 0;
	DWORD result = 0;

	// 查看内存区域属性, 只处理可读内存
	DWORD currentBaseAddress=startaddress;
	while( (currentBaseAddress < stopaddress)
		&& (VirtualQueryEx(processhandle, (LPCVOID)currentBaseAddress, &mbi, sizeof(mbi)) != 0))
	{

		// 属性判断
		if ( (mbi.State==MEM_COMMIT) && ((mbi.Protect & PAGE_NOACCESS)==0) && ((mbi.Protect & PAGE_GUARD)==0))  //必须是MEM_COMMIT, 非PAGE_NOACCESS, 非PAGE_GUARD
		{
			// 修正起始地址
			if ((DWORD)(mbi.BaseAddress)<startaddress)
			{
				mbi.RegionSize = mbi.RegionSize - (startaddress-(DWORD)(mbi.BaseAddress));
				mbi.BaseAddress=(PVOID)startaddress;
			}

			// 修正结束地址
			if( (DWORD)(mbi.BaseAddress)+mbi.RegionSize >= stopaddress)
				mbi.RegionSize=stopaddress-(DWORD)(mbi.BaseAddress);

/*
	处理跨分页AOB时, 也一并处理了跨内存区AOB, 因此本段代码已不需要

			// 处理跨可读内存区的AOB (across regions)
			if( (DWORD)(previousMbi.BaseAddress)+previousMbi.RegionSize == (DWORD)(mbi.BaseAddress) ) // 连续相连的的两个可读内存区, 需要处理跨内存区之AOB
			{
					int here = 0;
				if (0 != ReadProcessMemory(processhandle, (LPVOID)((DWORD)(mbi.BaseAddress)-(aobSize-1)), overlapBuffer, overlapSize, &actualread))
				{
					offset = 0;
					result = 0;
					while(SundayUnkownByteMatch(aob, aobSize, overlapBuffer+offset, actualread-offset, shiftTable, &result)){		// 在跨内存区处找到
						here++;
						_printHex(here);
						n++;
						if(n == Nth_match){
							*foundAddress = offset + result + (DWORD)(mbi.BaseAddress)-(aobSize-1);
							delete [] aob;
							delete [] memBuffer;
							delete [] overlapBuffer;
							return true;
						}
						offset = offset + result + 1;
					}

				}
			}
*/
			
			// 处理开头不足一个分页的多馀内存
			DWORD prefix=(DWORD)(mbi.BaseAddress) & 0xFFF;
			if(prefix > 0){
				if (0 != ReadProcessMemory(processhandle, (LPVOID)(mbi.BaseAddress), memBuffer, PAGE_SIZE-prefix, &actualread))
				{
					offset = 0;
					result = 0;
					while(SundayUnkownByteMatch(aob, aobSize, memBuffer+offset, actualread-offset, shiftTable, &result)){		// 在分页前的多馀处找到
						n++;
						if(n == Nth_match){
							*foundAddress = offset + result + (DWORD)(mbi.BaseAddress);
							delete [] aob;
							delete [] memBuffer;
							delete [] overlapBuffer;
							return true;
						}
						offset = offset + result + 1;
					}
				}
				mbi.BaseAddress = (PVOID)((DWORD)(mbi.BaseAddress) + PAGE_SIZE-prefix);
				mbi.RegionSize = mbi.RegionSize - (PAGE_SIZE-prefix);
			}

			// 开始处理可读内存区的所有分页
			for(DWORD pagestart = (DWORD)(mbi.BaseAddress); pagestart < (DWORD)(mbi.BaseAddress) + mbi.RegionSize; pagestart += PAGE_SIZE)
			{
				// 处理跨分页AOB (across pages)
				if(0 != ReadProcessMemory(processhandle, (LPVOID)(pagestart-(aobSize-1)), overlapBuffer, overlapSize, &actualread)){
					offset = 0;
					result = 0;
					while(SundayUnkownByteMatch(aob, aobSize, overlapBuffer+offset, actualread-offset, shiftTable, &result)){		// 在跨分页处找到
						n++;
						if(n == Nth_match){
							*foundAddress = offset + result + (pagestart-(aobSize-1));
							delete [] aob;
							delete [] memBuffer;
							delete [] overlapBuffer;
							return true;
						}
						offset = offset + result + 1;
					}
				}

				// 处理分页内的AOB
				if (0 != ReadProcessMemory(processhandle, (LPVOID)pagestart, memBuffer, PAGE_SIZE, &actualread))
				{
					offset = 0;
					result = 0;
					while(SundayUnkownByteMatch(aob, aobSize, memBuffer+offset, actualread-offset, shiftTable, &result)){		// 在分页中找到
						n++;
						if(n == Nth_match){
							*foundAddress = offset + result + pagestart;
							delete [] aob;
							delete [] memBuffer;
							delete [] overlapBuffer;
							return true;
						}
						offset = offset + result + 1;
					}
				}

			}
			
			// 所有分页处理完毕

			//totalProcessMemorySize += mbi.RegionSize;
			//previousMbi.BaseAddress = mbi.BaseAddress;	// 记录上次可读的内存区
			//previousMbi.RegionSize = mbi.RegionSize;
		}


		// 一个内存区处理完毕, 换下一个

		currentBaseAddress=(DWORD)(mbi.BaseAddress)+mbi.RegionSize;		// 设定下个内存区
	}    // while end

	// 所有可读内存区处理完毕

	// 显示实际处理的内存总大小
	//_printHex(totalProcessMemorySize);


	*foundAddress = 0;
	delete [] aob;
	delete [] memBuffer;
	delete [] overlapBuffer;
	return false;
}
