#pragma once


/*	OllyDbg Disasm 支持庫
使用支持庫, 必須將編譯預設選項設為 "以 unsigned char 取代 char"
方法為在 Project Properties 中的
Configuration Properties -> C/C++ -> Command Line -> Additional Options: 裡
寫上 /J
*/
// #pragma comment(lib, "disasm.lib")

#include <windows.h>
#include <vector>
#include <string>
//#include <time.h>

#include "aobscan.h"
#include "tools.h"

// disasm頭文件, 必須放在最後
extern "C"{
#include "disasm.h"
}

using namespace std;


// 命令
#define WRONG_COMMAND	2
#define ENABLE			1
#define DISABLE			0


// (from CE 5.61 autoassembler.pas)


	struct tassembled{
  DWORD address;
  vector<BYTE> bytes;
};

	struct tlabel{
  bool defined;
  DWORD address;
  string labelname;
  int assemblerline;
  vector<int> references; //index of assembled array
  vector<int> references2; //index of assemblerlines array
};

struct tfullaccess{

  DWORD address;
  DWORD size;
};

struct tdefine{
  string name;
  string whatever;
};

struct tloadbinary{
      string address; //string since it might be a label/alloc/define
      string filename;
};

struct tcealloc{
  DWORD address;
  string varname;
  DWORD size;
};





struct TUserdefinedsymbol{
  string symbolname;
  DWORD address;
  string addressstring;

  DWORD allocsize; //if it is a global alloc, allocsize>0
  DWORD processid; //the processid this memory was allocated to (in case of processswitches)
};



struct t2params{
string s1;
string s2;
};


struct tcheats{
	//string name;
	string script;
	vector<tcealloc> ceallocarray;
};


bool _autoassemble(HANDLE hGame, const string& aa_script, int command);
