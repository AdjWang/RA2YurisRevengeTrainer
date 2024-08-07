#ifndef AXDXTOOLS
#define AXDXTOOLS

#pragma comment(lib, "DbgHelp.lib")
#include <windows.h>
#include <vector>
#include <string>
#include <TlHelp32.h>
#include <DbgHelp.h>
#include <stack>


using namespace std;

string _trim(string line);
bool _strCmpNoCase(string str1, string str2);
void print(string s);
string int2HexString(int a);
string int2String(int a);
bool _getAddressFromName(HANDLE processhandle, string name, DWORD *pAddress);
bool HexStringToDword(string hex, DWORD *retval);
int findWordMatch2(string sentence, string targetWord, bool ignoreCase);
string replacetoken(string source, string target, string replacement);
string replaceBase10NotationToHex(string);
bool GetUserdefinedSymbolByName(string symbolname, DWORD *address);
int check_DB_DW_DD_DQ(string asmline);
vector<string> split(char *str, char separator);
bool EnableDebugPrivilege(bool bEnable);
int init_getModuleList(HANDLE processhandle);
string replaceModuleNameWithAddress(HANDLE processhandle, string line, bool *yes);
vector<string> splitLines(char *str);
void removecomments(vector<string> &lines);

#endif