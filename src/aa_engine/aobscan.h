/*
 *   作者: axdx
 *
 *   更新日志:
 *
 *      v1.0: Sunday 算法
 *      v1.1: 修正 AOB 跨分页时, 找不到的问题
 *      v1.2: 忽略不可读内存区 (效率大增)
 *      v1.3: Sunday 算法改, 支持使用 ?? 或 * 表示未知字节
 *      v1.4: 可指定第几个命中目标
 *
 *
 *   使用环境: VC++ 2008, 其他环境请自行测试
 *
 */

#pragma once
#include <windows.h>
#include <vector>
#include <string>
using namespace std;



/////////////////////////////////////////////////////////////////////
//
// 内部使用之工具函数
//
//

#define SHIFT_TABLE_SIZE 256	// from 0x0 to 0xFF
#define PAGE_SIZE 0x1000

vector<WORD> _stringToAOBwithUnknown(string aobstring);
bool aobscan(HANDLE processhandle, int Nth_match, DWORD startaddress, DWORD stopaddress, string aobstring, DWORD *foundAddress);

//
/////////////////////////////////////////////////////////////////////
