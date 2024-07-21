#include "tools.h"

// Retrieve the system error message for the last-error code
void ErrorInfo(LPTSTR lpszFunction) 
{ 

	LPVOID lpMsgBuf;
	LPVOID lpDisplayBuf;
	DWORD dw = GetLastError(); 

	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | 
		FORMAT_MESSAGE_FROM_SYSTEM |
		FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		dw,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR) &lpMsgBuf,
		0, NULL );


	lpDisplayBuf = (LPVOID)LocalAlloc(LMEM_ZEROINIT, 
		(lstrlen((LPCTSTR)lpMsgBuf)+lstrlen((LPCTSTR)lpszFunction)+40)*sizeof(TCHAR)); 
	wsprintf((LPTSTR)lpDisplayBuf, TEXT("%s Error Code %d: %s"), lpszFunction, dw, lpMsgBuf); 
	MessageBox(NULL, (LPCTSTR)lpDisplayBuf, TEXT("Error"), MB_OK); 

	LocalFree(lpMsgBuf);
	LocalFree(lpDisplayBuf);
}

void printInt(int a){
	char str[10];
	itoa(a, str, 10); // 16 for hex; 
	MessageBoxA(0,str,0,0);
}

void printHex(int a){
	char str[10];
	itoa(a, str, 16); // 10 for decimal; 
	MessageBoxA(0,str,0,0);

}

void print(string s){
	MessageBoxA(0,s.c_str(),0,0);
}

string int2HexString(int a){
	char str[16];
	wsprintfA(str, "%08x", a); 
	return string(str);
}

string int2String(int a){
	char str[256];
	itoa(a, str, 10);
	return string(str);
}


// find a word in a sentence (ignore case)
// if found, return position
// else return -1
int findWordMatch(char *sentence, char *targetWord){

	int sentenceLen = lstrlenA(sentence);
	int wordLen = lstrlenA(targetWord);

	bool isFound = false;
	int k = 0;
	while(k <= sentenceLen - wordLen){
		for(int i = 0; i < wordLen; i++){
			char c = sentence[k + i];
			if (c >= 0x41 && c <= 0x5A){	// to lowercase
				c += 0x20;
			}
			if( c!= targetWord[i]){
				break;
			}
			if(i == wordLen - 1){
				isFound = true;
			}
		}
		if(isFound == true){
			break;
		}
		k++;
	}

	if(isFound){
		return k;
	}else{
		return -1;

	}
}


// find a word in a sentence 
// if found, return position
// else return -1
int findWordMatch2(string sentence, string targetWord, bool ignoreCase){

	int sentenceLen = sentence.size();
	int wordLen = targetWord.size();

	bool isFound = false;
	int k = 0;
	while(k <= sentenceLen - wordLen){
		for(int i = 0; i < wordLen; i++){
			char c1 = sentence[k + i];
			if (ignoreCase && c1 >= 0x41 && c1 <= 0x5A){	// to lowercase
				c1 += 0x20;
			}

			char c2 = targetWord[i];
			if (ignoreCase && c2 >= 0x41 && c2 <= 0x5A){	// to lowercase
				c2 += 0x20;
			}

			if( c1 != c2){
				break;
			}
			if(i == wordLen - 1){
				isFound = true;
			}
		}
		if(isFound == true){
			break;
		}
		k++;
	}

	if(isFound){
		return k;
	}else{
		return -1;

	}
}


// 字串比较 无视英文大小写 string insensitive compare 
bool _strCmpNoCase(string str1, string str2){
	int size1 = str1.size();
	int size2 = str2.size();

	if(size1 != size2)
		return false;

	for(int i = 0; i < size1; i++){
		char c1 = str1[i];
		if (c1 >= 0x41 && c1 <= 0x5A){	// to lowercase
			c1 += 0x20;
		}

		char c2 = str2[i];
		if (c2 >= 0x41 && c2 <= 0x5A){	// to lowercase
			c2 += 0x20;
		}

		if(c1 != c2){
			return false;
		}
	}
	return true;
}




// splits a string with separator, return tokens
vector<string> split(char *str, char separator)
{
	vector<string> token;
	char *pointer = str;
	char *begin;
	while(1) {
		begin = pointer;
		while(*pointer != separator && *pointer != 0) {
			pointer++;
		}
		if(begin < pointer) {
			token.push_back(string(begin, pointer));
		}
		if(*pointer == 0) {
			break;
		}
		pointer++;
	}
	return token;
}


// splits a string with lines, also delete leading spaces and tabs
vector<string> splitLines(char *str)
{
	char CR = 0x0d;
	char LN = 0x0a;
	char space = 0x20;
	char tab = 0x09;

	vector<string> token;
	char *pointer = str;
	char *begin;
	while(1) {
		while(*pointer == space || *pointer == tab) {
			pointer++;
		}
		begin = pointer;

		while(*pointer != CR && *pointer != LN && *pointer != 0) {
			pointer++;
		}
		if(begin < pointer) {
			token.push_back(string(begin, pointer));
		}
		if(*pointer == 0) {
			break;
		}
		pointer++;
	}
	return token;
}





// convert HEX address string to DWORD
bool HexStringToDword(string hex, DWORD *retval){
	if(hex.size()<=0)
		return false;

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
	DWORD _address = 0;
	char *lpAddress = (char *)&_address;
	while(i >= 0){
		char c = chex[i];
		i--;
		if(i >= 0){
			c = c | (chex[i] << 4);
		}
		lpAddress[0] = c;
		lpAddress++;
		i--;
	}

	*retval = _address;

	return true;
}

// check if its a "db" command
int check_DB_DW_DD_DQ(string asmline){

	if(asmline.size() < 2){
		return 0;
	}

	// comapre "db"
	char a1 = asmline[0];
	char a2 = asmline[1];
	if (a1 >= 0x41 && a1 <= 0x5A){	// to lowercase
		a1 += 0x20;
	}
	if (a2 >= 0x41 && a2 <= 0x5A){	// to lowercase
		a2 += 0x20;
	}

	if(a1 != 'd'){
		return 0;
	}

	if(a2 == 'b'){
		return 1;
	}else if(a2 == 'w'){
		return 2;
	}else if(a2 == 'd'){
		return 3;
	}else if(a2 == 'q'){
		return 4;
	}else{
		return 0;
	}
}

// delete leading and trailing spaces
string _trim(string line){

	int start = 0;
	int end = line.size()-1;
	while(start < line.size() && line[start] == ' ')
		start++;

	if(start >= line.size())
		return "";

	while(end>=0 && line[end] == ' ')
		end--;

	if(end < 0 || end < start)
		return "";

	return line.substr(start, end - start + 1);
}








//remove comments  (from CE 5.61 autoassembler.pas)
void removecomments(vector<string> &lines){

	bool instring = false;
	bool incomment = false;
	string currentline;

	for(int i = 0; i < lines.size(); i++)
	{
		currentline = lines[i];

		for(int j = 0; j < currentline.size(); j++)
		{
			if (incomment)
			{
				//inside a comment, remove everything till a } is encountered
				if ( (currentline[j] == '}') || ((currentline[j] == '*') && (j < currentline.size()) && (currentline[j+1] == '/')) )
				{
					incomment = false; //and continue parsing the code...

					if ((currentline[j] == '*') && (j < currentline.size()) && (currentline[j+1] == '/')) 
						currentline[j+1] = ' ';
				}
				currentline[j] = ' ';
			}
			else
			{
				if (currentline[j] == '\'') // everything in single quote 'xxx' is an absolute string
					instring = !instring;

				if (currentline[j] == 0x09)
					currentline[j] = ' '; //tabs are basicly comments 

				if(!instring)
				{
					//not inside a string, so comment markers need to be dealt with
					if ( (currentline[j] == '/') && (j < currentline.size()) && (currentline[j+1] == '/') )  //- comment (only the rest of the line)
					{
						//cut off till the end of the line (and might as well jump out now)
						currentline = currentline.substr(0,j);
						break;
					}

					if ( (currentline[j] == '{') || ((currentline[j] == '/') && (j < currentline.size()) && (currentline[j+1] == '*')) ) 
					{
						incomment = true;
						currentline[j] = ' '; //replace from here till the first } with spaces, this goes on for multiple lines
					}
				}
			}
		}

		lines[i] = _trim(currentline);
	}

}


// address tokenizer  (from CE 5.61 symbolhandler.pas)
vector<string> tokenize(string s)
//Just a tokenizer for simple address specifiers
{
	vector<string> tokens;
	int i;
	int last = 0;
	string t;

	for(i = 0; i < s.size(); i++){
		if (s[i] == '[' || s[i] == ']' || s[i] == '+' || s[i] == '-' || s[i] == '*'){
			t = _trim(s.substr(last, i-last));
			if (t.size() > 0){
				tokens.push_back(t);
			}

			tokens.push_back(s.substr(i,1));
			last = i + 1;
		}else if(s[i] == '\"'){
			t = _trim(s.substr(last, i-last));
			if (t.size() > 0){
				tokens.push_back(t);
			}
			size_t j;
			for(j = i+1; j < s.size() && s[j] != '\"'; ++j){
				// skip all in "xxx"
			}
			t = s.substr(i, j-i+1);
			if (t.size() > 0){
				tokens.push_back(t);
			}
			i=j;
			last = i + 1;
		}

	}

	//last part
	t = _trim(s.substr(last,s.size()-last));
	if (t.size() > 0){
		tokens.push_back(t);
	}

	return tokens;
}


// find target in source, replace it by replacement
string replacetoken(string source, string target, string replacement){

	size_t found = source.find(target);
	if (found != string::npos){
		source.replace((int)found,target.length(),replacement);
	}
	return source;
}


string toUpperCase(string s){
	string s2 = s;
	for(int i = 0; i < s.size(); i++){
		char c = s[i];
		if (c >= 'a' && c <= 'z'){	// to lowercase
			c -= 0x20;
			s2[i] = c;
		}
	}
	return s2;
}


string toLowerCase(string s){
	string s2 = s;
	for(int i = 0; i < s.size(); i++){
		char c = s[i];
		if (c >= 0x41 && c <= 0x5A){	// to lowercase
			c += 0x20;
			s2[i] = c;
		}
	}
	return s2;
}

string toLowerCaseIgnoreQuotes(string s){
	string s2 = s;
	bool inQuotes = false;
	for(int i = 0; i < s.size(); i++){
		char c = s[i];
		if(s[i] == '\"'){
			if(inQuotes)
				inQuotes = false;
			else
				inQuotes = true;
		}
		else if(!inQuotes && c >= 0x41 && c <= 0x5A){	// to lowercase
			c += 0x20;
			s2[i] = c;
		}
	}
	return s2;
}



struct tmoduleinfo{
	string modulename;
	string modulenameLowerCase;
	string modulepath;
	bool isSystemModule;
	DWORD baseaddress;
	DWORD basesize;
};


vector<tmoduleinfo> modulelist;



//DWORD _getModuleBase(DWORD processId, string name)
//{
//
//	string _name = _trim(name);
//	if(_name.size()<1){
//		return 0;
//	}
//
//	bool ignoreCase = true;
//	if(_name.size()>2 && _name[0] == '"' && _name[_name.size()-1] == '"'){
//		_name = _name.substr(1,_name.size()-2);
//		ignoreCase = false;
//	}
//	else{
//			_name = toLowerCase(_name);
//	}
//
//	for(int i = 0; i < modulelist.size(); i++){
//		if(ignoreCase){
//			if(0 == modulelist[i].modulenameLowerCase.compare(_name)){
//				return modulelist[i].baseaddress;
//			}
//		}
//		else{
//			if(0 == modulelist[i].modulename.compare(_name)){
//				return modulelist[i].baseaddress;
//			}
//		}
//	}
//
//	return 0;
//}





/*
参考用
procedure TSymhandler.loadmodulelist;
var
ths: thandle;
me32:MODULEENTRY32;
x: pchar;

processid: dword;
begin
{$ifdef autoassemblerdll}
processid:=symbolhandler.ProcessID;
{$else}
if targetself then
processid:=getcurrentprocessid
else
processid:=cefuncproc.ProcessID;
{$endif}

modulelistMREW.BeginWrite;
try
modulelistpos:=0;
if processid=0 then exit;

//refresh the module list
ths:=CreateToolhelp32Snapshot(TH32CS_SNAPMODULE,processid);
if ths<>0 then
begin
me32.dwSize:=sizeof(MODULEENTRY32);
if ths<>0 then
begin
try
if module32first(ths,me32) then
repeat
if modulelistpos+1>=length(modulelist) then
setlength(modulelist,length(modulelist)*2);

x:=me32.szExePath;
modulelist[modulelistpos].modulename:=extractfilename(x);
modulelist[modulelistpos].modulepath:=x;

//all windows folder files are system modules, except when it is an .exe (minesweeper in xp)
modulelist[modulelistpos].isSystemModule:=(pos(lowercase(windowsdir),lowercase(x))>0) and (ExtractFileExt(lowercase(x))<>'.exe');

if (not modulelist[modulelistpos].isSystemModule) and (commonModuleList<>nil) then //check if it's a common module (e.g nvidia physx dll's)
modulelist[modulelistpos].isSystemModule:=commonModuleList.IndexOf(lowercase(modulelist[modulelistpos].modulename))<>-1;

modulelist[modulelistpos].baseaddress:=dword(me32.modBaseAddr);
modulelist[modulelistpos].basesize:=me32.modBaseSize;
inc(modulelistpos);

until not module32next(ths,me32);
finally
closehandle(ths);
end;
end;
end;
finally
modulelistmrew.EndWrite;
end;
end;

*/
int init_getModuleList(HANDLE processhandle){

	modulelist.clear();

	HANDLE hSnapShotModule;
	MODULEENTRY32 moduleentry;
	moduleentry.dwSize = sizeof(moduleentry);


	hSnapShotModule = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, GetProcessId(processhandle));

	if(hSnapShotModule == INVALID_HANDLE_VALUE){
		return 0;
	}

	int n = 0;

	bool ok =  Module32First(hSnapShotModule, &moduleentry);
	while(ok){
		tmoduleinfo moduleinfo;

		//char path[260];
		//WideCharToMultiByte(CP_ACP,0,moduleentry.szExePath,-1, path,260,NULL, NULL);
		//moduleinfo.modulepath = string(path);

		//int pos;
		//int length = 0;
		//for(pos = moduleinfo.modulepath.size()-1;pos>=0;pos--){
		//	if(moduleinfo.modulepath[pos] != '\\')
		//		length++;
		//	else
		//		break;
		//}
		//pos++;
		//if(pos>=moduleinfo.modulepath.size())
		//	continue;

		//moduleinfo.modulename = _modulename.substr(pos,length);


		// DEBUG: disable unicode
		// char modulename[260];
		// WideCharToMultiByte(CP_ACP,0,moduleentry.szModule,-1, modulename,260,NULL, NULL);
		// moduleinfo.modulename = string(modulename);

		moduleinfo.modulename = string(moduleentry.szModule);
		moduleinfo.modulenameLowerCase = toLowerCase(moduleinfo.modulename);
		moduleinfo.baseaddress = (DWORD)moduleentry.modBaseAddr;

		modulelist.push_back(moduleinfo);

		ok =  Module32Next(hSnapShotModule, &moduleentry);
	}
	CloseHandle(hSnapShotModule);
	return modulelist.size();
}



string replaceModuleNameWithAddress(HANDLE processhandle, string line, bool *yes){
	int i,j,k;

	*yes = false;

	if(modulelist.size()<=0)
		init_getModuleList(processhandle);

	//printInt(modulelist.size());

	for(i = 0; i < modulelist.size(); i++){


		bool found = true;
		char c;
		int pos, len;
		//for(j = 0; j < (int)(line.size()-modulelist[i].modulename.size()+1); j++){
		for(j = 0; j+modulelist[i].modulename.size() < line.size()+1; j++){
			if(line[j] != '\"'){		// not in quotes, ignore case
				found = true;
				//print(modulelist[i].modulenameLowerCase);
				for(k = 0; k < modulelist[i].modulenameLowerCase.size(); k++){
					c = line[j+k];
					if (c >= 0x41 && c <= 0x5A){	// to lowercase
						c += 0x20;
					}
					if(c != modulelist[i].modulenameLowerCase[k]){
						found = false;
						break;
					}
				}
				//print(modulelist[i].modulenameLowerCase);
				//print(line);
				//printInt(j);
				//printInt(line.size()-modulelist[i].modulename.size()+1);

				if(k > 0 && found){
					//print(line);
					//printInt(line.size());
					line = line.substr(0, j) + int2HexString(modulelist[i].baseaddress) + line.substr(j+k, line.size()-j-k);
					*yes = true;
					//print(line);
					//printInt(line.size());
				}
				//print(line+"!="+modulelist[i].modulenameLowerCase);
			}else{	// found quotes, case-sensitive
				pos = line.find("\"", j+1);
				if(pos >= line.size())
					pos = line.size()-1;

				string sInQuotes = line.substr(j+1, pos-j-1);
				len = modulelist[i].modulename.size()+2;
				if(0 == sInQuotes.compare(modulelist[i].modulename)){
					//print(line);
					line = line.substr(0, j) + int2HexString(modulelist[i].baseaddress) + line.substr(j+len, line.size()-j-len);
					*yes = true;
					//print(line);
				}else{
					j = j+len-1;
				}
			}
		}

	}


	return line;
}


// (from CE 5.61 symbolhandler.pas)
bool ParseAsPointer(string s, vector<string> &list)
{
	int i;
	bool prolog;
	int currentlevel;
	string temps;
	bool ispointer;

	//parse the string
	bool result=false;
	currentlevel=0;
	prolog=true;
	temps="";
	ispointer=false;



	for(i=0;i<s.size();i++)
	{
		if(s[i]=='[')
		{
			if(prolog)
			{
				currentlevel++;
				ispointer=true;
			}
			else
				return result; //bracket open after the prolog is not allowed
		}
		else
		{
			if(prolog)
			{
				if(s[i] != '\t' && s[i] != ' ')  //no space or tab
					prolog=false;
			}

			if(!prolog)
			{
				//definition, currentlevel is set, now parse till last ] (currentlevel=0)
				if(s[i]==']') //end of a level
				{
					currentlevel--;
					if(temps.size()==0)
						temps="+0";  

					list.push_back(temps);

					temps="";

					if(currentlevel<0)
						return result;

					continue;
				}
				else
					temps=temps+s[i];
			}
		}
	}



	if(temps.size()==0)
		temps="+0";

	if(ispointer && temps.size()>0)
		list.push_back(temps);

	if(currentlevel>0)
		return result;

	result=ispointer;

	return result;
}

// (from CE 5.61 symbolhandler.pas)
// Will return the address of a pointer noted as [[[xxx+xx]+xx]+xx]+xx
// If it is a invalid pointer, or can not be resolved, the result is NULL 
DWORD GetAddressFromPointer(HANDLE processhandle, string s, bool *pError)
{
	int i;
	vector<string> list;
	vector<DWORD> offsets;
	DWORD baseaddress;
	string off;
	DWORD realaddress, realaddress2;
	bool check;
	DWORD count;

	DWORD result;
	*pError = true;



	if(!ParseAsPointer(s,list))
		return 0;


	if(!_getAddressFromName(processhandle, list[0], &baseaddress))
		return 0;


	for(int i = 0; i < list.size()-1; i++)
		offsets.push_back(0);

	for(i=1; i<list.size();i++) //start from the first offset
	{
		off=list[i].substr(1,list[i].size()-1);

		DWORD dwordtemp;
		if(!HexStringToDword(off, &dwordtemp)){
			return 0;
		}
		offsets[i-1]=dwordtemp;

		if(list[i][0]=='-')
			offsets[i-1]=-offsets[i-1];
	}

	//still here so notation was correct and baseaddress+offsets are filled in
	//now read
	realaddress2=baseaddress;
	//MessageBoxA(0,int2HexString(realaddress2).c_str(),0,0);
	for(i=0;i<offsets.size();i++)
	{
		check=(0 != ReadProcessMemory(processhandle,(LPCVOID)realaddress2,&realaddress,4,&count));
		if(check && count==4){
			realaddress2=realaddress+offsets[i];
			//MessageBoxA(0,int2HexString(realaddress2).c_str(),0,0);
		}else{
			//string s = "cannot read from pointer " + int2HexString(realaddress2);
			//MessageBoxA(0,s.c_str(),0,0);
			return 0;
		}
	}

	result=realaddress2;
	*pError=false;
	return result;
}


DWORD getAddressFromSymbol(HANDLE processhandle, string symbolstring){

	//DWORD dwordtemp2 = GetStartAddress(processhandle);
	//if(dwordtemp2 > 0){
	//printHex(dwordtemp2);
	//}else{
	//ErrorInfo(TEXT("GetStartAddress"));
	//}

	if (!SymInitialize(processhandle,NULL,true)){
		//ErrorInfo(TEXT("SymInitialize"));
		return 0;
	}

	SymSetOptions(SymGetOptions() | SYMOPT_CASE_INSENSITIVE);
	// SymSetSearchPath(processhandle,pchar(searchpath));

	//SymbolsLoaded:=false;
	//if symbolprocesshandle<>0 then Symcleanup(symbolprocesshandle); //cleanup first

	//SymbolsLoaded:=SymInitialize(thisprocesshandle,nil,true);
	//if symbolsloaded then
	//begin
	//  symsetoptions(symgetoptions or SYMOPT_CASE_INSENSITIVE);
	//  symsetsearchpath(processhandle,pchar(searchpath));

	//  if kernelsymbols then LoadDriverSymbols;
	//  LoadDLLSymbols;
	//end else error:=true;


	//symbolprocesshandle:=processhandle;



	// 参考用
	//DWORD GetStartAddress( HANDLE hProcess)
	//{
	//   SYMBOL_INFO *pSymbol;
	//   pSymbol = (SYMBOL_INFO *)new BYTE[sizeof(SYMBOL_INFO )+MAX_SYM_NAME];
	//   pSymbol->SizeOfStruct= sizeof(SYMBOL_INFO );
	//   pSymbol->MaxNameLen = MAX_SYM_NAME;
	//   SymFromName(hProcess,"wWinMainCRTStartup",pSymbol);
	//
	//   // Store address, before deleting pointer  
	//   DWORD dwAddress = pSymbol->Address;
	//
	//   delete [](BYTE*)pSymbol; // Valid syntax!
	//
	//   return dwAddress;
	//}



	BYTE symbolBuffer[ sizeof(IMAGEHLP_SYMBOL) + 255 ];
	PIMAGEHLP_SYMBOL symbol = (PIMAGEHLP_SYMBOL)&symbolBuffer[0];
	symbol->SizeOfStruct = sizeof(symbolBuffer);
	symbol->MaxNameLength = 254;
	symbol->Address = 0;
	symbol->Flags = 0;
	symbol->Size =0;

	if(SymGetSymFromName(processhandle,symbolstring.c_str(),symbol))
	{
		//printHex(symbol->Address);
		SymCleanup(processhandle);

		return symbol->Address;
	}
	else{				
		//ErrorInfo(TEXT("SymGetSymFromName"));
		SymCleanup(processhandle);
		return 0;

	}

}




// (from CE 5.61 symbolhandler.pas)
bool _getAddressFromName(HANDLE processhandle, string name, DWORD *pAddress)
{
	tmoduleinfo mi;

	DWORD offset = 0;

	string sn;
	int i,j;

	//ws: widestring;
	//pws: pwidechar;
	string ws;

	bool error;


	//tokens: TTokens;
	//x: dword;
	string mathstring;
	bool hasMultiplication = false;
	bool hasPointer = false;
	//
	int nextoperation;
	//regnr: integer;
	//
	//haserror:=false;


	name = replaceBase10NotationToHex(name);

	if(HexStringToDword(name, pAddress)){
		return true;			//it's a valid hexadecimal string
	}

	//not a hexadecimal string
	vector<string> tokens = tokenize(name);

	//mathstring="";
	//for(i=0;i<tokens.size();i++)
	//	mathstring=mathstring+tokens[i];
	//MessageBoxA(0,mathstring.c_str(),0,0);


	//first check the most basic thing
	if(tokens.size() == 0)
	{
		return false;
	}

	//if it starts with a *, - or + or }s with it, then it's a bad formula
	if (tokens[0][0] == '*' || tokens[0][0] == '+' || tokens[0][0] == '-')
	{
		return false;
	}
	if (tokens[tokens.size()-1][0] == '*' || tokens[tokens.size()-1][0] == '+' || tokens[tokens.size()-1][0] == '-')
	{
		return false;
	}

	//convert the tokens into hexadecimal values

	//symbolloadervalid.beginread;
	//try

	for(i = 0; i < tokens.size(); i++)
	{
		//MessageBoxA(0,tokens[i].c_str(),0,0);
		if(tokens[i][0] != '[' && tokens[i][0] != ']' && tokens[i][0] != '+' && tokens[i][0] != '-' && tokens[i][0] != '*')
		{

			DWORD dwordtemp;
			if(!HexStringToDword(tokens[i], &dwordtemp))
			{

				//not a hexadecimal value
				//DWORD modulebaseaddress = _getModuleBase(GetProcessId(processhandle), tokens[i]);
				//if(modulebaseaddress > 0)
				//{
				//	tokens[i] = int2HexString(modulebaseaddress);
				//	continue;
				//}
				//print("here1");
				bool yes = false;
				string strtemp = replaceModuleNameWithAddress(processhandle,tokens[i], &yes);
				if(yes)
				{
					tokens[i] = strtemp;
					continue;
				}
				//print("here2");
				//print(strtemp);
				//print(tokens[i]);


				//not a modulename


				//regnr:=getreg(uppercase(tokens[i]),false);

				//if regnr<>9 then
				//begin
				//  if (context<>nil) and (context^.Eip<>0) then
				//  begin
				//    //get the register value, and because this is an address specifier, use the full 32-bits
				//    case regnr of
				//      0: tokens[i]:=inttohex(context^.Eax,8);
				//      1: tokens[i]:=inttohex(context^.Ecx,8);
				//      2: tokens[i]:=inttohex(context^.Edx,8);
				//      3: tokens[i]:=inttohex(context^.Ebx,8);
				//      4: tokens[i]:=inttohex(context^.Esp,8);
				//      5: tokens[i]:=inttohex(context^.Ebp,8);
				//      6: tokens[i]:=inttohex(context^.Esi,8);
				//      7: tokens[i]:=inttohex(context^.Edi,8);
				//    end;
				//    continue; //handled
				//  end;

				//  //not handled, but since it's a register, quit now
				//end
				//else
				//begin
				//  //no context or not a register


				//check the userdefinedsymbols
				if(GetUserdefinedSymbolByName(tokens[i], &dwordtemp))
				{
					tokens[i]=int2HexString(dwordtemp);
					continue;
				}

				////not a userdefined symbol


				//check the symbols
				//if (symbolloaderthread<>nil) then
				//{
				// if symbolloaderthread.isloading and not waitforsymbols then
				// {
				//  if not waitforsymbols then
				//  {
				//		haserror:=true;
				//	  exit;
				//  }

				//  symbolloaderthread.WaitFor;
				// }

				// //it's not a valid address, it's not a calculation, it's not a modulename+offset, so lets see if it's a module

				//tokens[i]=StringReplace(tokens[i],'.','!',[]);


				//DWORD dwordtemp2 = GetStartAddress(processhandle);
				//if(dwordtemp2 > 0){
				//printHex(dwordtemp2);
				//}else{
				//ErrorInfo(TEXT("GetStartAddress"));
				//}

				//		    if (!SymInitialize(processhandle,NULL,true))
				//				ErrorInfo(TEXT("SymInitialize"));

				//SymSetOptions(SymGetOptions() | SYMOPT_CASE_INSENSITIVE);
				// SymSetSearchPath(processhandle,pchar(searchpath));

				//SymbolsLoaded:=false;
				//if symbolprocesshandle<>0 then Symcleanup(symbolprocesshandle); //cleanup first

				//SymbolsLoaded:=SymInitialize(thisprocesshandle,nil,true);
				//if symbolsloaded then
				//begin
				//  symsetoptions(symgetoptions or SYMOPT_CASE_INSENSITIVE);
				//  symsetsearchpath(processhandle,pchar(searchpath));

				//  if kernelsymbols then LoadDriverSymbols;
				//  LoadDLLSymbols;
				//end else error:=true;


				//symbolprocesshandle:=processhandle;



				//BYTE symbolBuffer[ sizeof(IMAGEHLP_SYMBOL) + 255 ];
				//PIMAGEHLP_SYMBOL symbol = (PIMAGEHLP_SYMBOL)&symbolBuffer[0];
				//symbol->SizeOfStruct = sizeof(symbolBuffer);
				//symbol->MaxNameLength = 254;
				//symbol->Address = 0;
				//symbol->Flags = 0;
				//symbol->Size =0;
				//getmem(symbol,sizeof(IMAGEHLP_SYMBOL)+255);
				//try{
				// zeromemory(symbol,sizeof(IMAGEHLP_SYMBOL)+255);
				// symbol.SizeOfStruct:=sizeof(IMAGEHLP_SYMBOL)+255;
				// symbol.MaxNameLength:=254;

				//SymGetSymFromName or SymFromName ?
				//if(SymGetSymFromName(processhandle,tokens[i].c_str(),symbol))
				////if(SymFromName(processhandle,tokens[i].c_str(),symbol))
				//{
				//	tokens[i]=int2HexString(symbol->Address);
				//	print(tokens[i]);
				//	continue;
				//}
				//else{				
				//	ErrorInfo(TEXT("SymGetSymFromName"));
				//}

				dwordtemp = getAddressFromSymbol(processhandle, tokens[i]);
				if(dwordtemp>0)
				{
					tokens[i]=int2HexString(dwordtemp);
					continue;
				}


				//not a symbol

				return false;

			}


		}
		else
		{
			//it's not a real token
			if(tokens[i][0] == '*'){
				hasMultiplication=true; 
			}else if(tokens[i][0] == '[' || tokens[i][0] == ']'){
				hasPointer=true;
			}
		}
	}


	//finally
	//  symbolloadervalid.beginread;
	//};




	//MessageBoxA(0,mathstring.c_str(),0,0);

	if(hasPointer)
	{
		mathstring="";
		for(i=0;i<tokens.size();i++)
			mathstring=mathstring+tokens[i];

		DWORD addr;
		DWORD addr2=0;
		DWORD count=0;

		stack<int> stackP;   // index stack for pointers
		for(int i = 0; i < mathstring.size(); i++){
			if(mathstring[i] == '['){
				stackP.push(i);
			}
			else if(mathstring[i] == ']'){
				if(!stackP.empty()){
					int start = stackP.top();
					stackP.pop();

					if(false == _getAddressFromName(processhandle, mathstring.substr(start+1, i-start-1), &addr))
					{
						return false;
					}

					bool check=ReadProcessMemory(processhandle,(LPCVOID)addr,&addr2,4,&count);
					if(!check || count!=4){
						return false;
					}

					mathstring = mathstring.substr(0, start) + int2HexString(addr2) + mathstring.substr(i+1);

					i = start;
				}else{
					return false;
				}
			}
		}

		if(!stackP.empty()){
			return false;
		}

		if(false == _getAddressFromName(processhandle, mathstring, &addr))
		{
			return false;
		}else{
			*pAddress = addr;
			return true;
		}
		//DWORD result=GetAddressFromPointer(processhandle,mathstring,&error);
		////MessageBoxA(0,int2HexString(result).c_str(),0,0);
		//if(!error)
		//{
		//	*pAddress = result+offset;

		//	return true;
		//}
		//else
		//{
		//	*pAddress = 0;
		//	//it has a pointer notation but the pointer didn't get handled... ERROR!
		//	return false;
		//}
	}


	DWORD dwordtemp1, dwordtemp2;

	//handle the mathstring
	if(hasMultiplication)
	{
		//first do the multiplications
		for(i=0;i<tokens.size();i++)
		{
			if(tokens[i][0]=='*')
			{
				//multiply the left and right
				if(!HexStringToDword(tokens[i-1],&dwordtemp1)){
					return false;
				}
				if(!HexStringToDword(tokens[i+1],&dwordtemp2)){
					return false;
				}
				tokens[i-1]=int2HexString(dwordtemp1 * dwordtemp2);
				tokens[i]="";
				tokens[i+1]="";
			}
		}
	}

	DWORD result=0;
	//handle addition and subtraction
	nextoperation=1;                // 1: +, 0: -
	for(i=0;i<tokens.size();i++)
	{
		if(tokens[i].size()>0)
		{
			if(tokens[i][0] == '+'){
				nextoperation = 1;
			}else if(tokens[i][0] == '-'){

				if(nextoperation==0){
					nextoperation=1;
				}else{ //--=+
					nextoperation=0;
				}
			}else{
				//do the calculation
				if(nextoperation == 1){
					if(!HexStringToDword(tokens[i],&dwordtemp1)){
						return false;
					}
					result=result+dwordtemp1;
				}else{
					if(!HexStringToDword(tokens[i],&dwordtemp1)){
						return false;
					}
					result=result-dwordtemp1;
				}

			}
		}

	}

	*pAddress = result;
	return true;
}



DWORD floatToDword(float f)
{
 return *((DWORD*)&f);
}

__int64 doubleToInt64(double d)
{
 return *((__int64 *)&d);
}




// #123   (int)123   (float)123.4   (double)123.4
string replaceBase10NotationToHex(string asmline){

	// #123
	int pos = findWordMatch2(asmline, "#", true);
	while(pos != -1){
		int length = 0;
		for(int i = pos + 1; i < asmline.size(); i++){
			if(asmline[i] >= 0x30 && asmline[i] <= 0x39)
				length++;
			else
				break;

		}
		if(length > 0){
			string target = asmline.substr(pos, length+1);
			asmline = replacetoken(asmline, target, int2HexString(atoi(target.substr(1).c_str())));
		}
		pos = findWordMatch2(asmline, "#", true);
	}

	// (int)123
	pos = findWordMatch2(asmline, "(int)", true);
	while(pos != -1){
		int length = 0;
		for(int i = pos + 5; i < asmline.size(); i++){
			if(asmline[i] >= 0x30 && asmline[i] <= 0x39)
				length++;
			else
				break;
		}
		if(length > 0){
			string target = asmline.substr(pos, length+5);
			asmline = replacetoken(asmline, target, int2HexString(atoi(target.substr(5).c_str())));
		}
		pos = findWordMatch2(asmline, "(int)", true);
	}


		// (float)
	pos = findWordMatch2(asmline, "(float)", true);
	while(pos != -1){
		int length = 0;
		for(int i = pos + 7; i < asmline.size(); i++){
			if((asmline[i] >= 0x30 && asmline[i] <= 0x39) || asmline[i] == '.')
				length++;
			else
				break;
		}
		if(length > 0){
			string target = asmline.substr(pos, length+7);
			float f = (float)atof(target.substr(7).c_str());
			asmline = replacetoken(asmline, target, int2HexString(floatToDword(f)));
		}
		pos = findWordMatch2(asmline, "(float)", true);
	}


		// (double)
	pos = findWordMatch2(asmline, "(double)", true);
	while(pos != -1){
		int length = 0;
		for(int i = pos + 8; i < asmline.size(); i++){
			if((asmline[i] >= 0x30 && asmline[i] <= 0x39) || asmline[i] == '.')
				length++;
			else
				break;
		}
		if(length > 0){
			string target = asmline.substr(pos, length+8);
			double d = atof(target.substr(8).c_str());
			__int64 i64 = doubleToInt64(d);
			DWORD d1 = (DWORD)(i64 & 0xffffffff);
			DWORD d2 = (DWORD)((i64 >> 32) & 0xffffffff);
			string i64hex = int2HexString(d2) + int2HexString(d1);
			asmline = replacetoken(asmline, target, i64hex);
		}
		pos = findWordMatch2(asmline, "(double)", true);
	}
//print(asmline);
	return asmline;
}


//vector<BYTE> stringToAOB(string aobstring){
//	vector<BYTE> aob;
//	DWORD dwordtemp = 0;
//	vector<string> bytestrings = split((char *)aobstring.c_str(),' ');
//	for(int i = 0; i < bytestrings.size(); i++){
//		if(HexStringToDword(bytestrings[i],&dwordtemp)){
//			aob.push_back((BYTE)dwordtemp);
//		}else{
//			aob.clear();
//			return aob;
//		}
//	}
//	return aob;
//}





bool EnableDebugPrivilege(bool bEnable)
{
	//Enabling the debug privilege allows the application to see 
	//information about service application
	BOOL fOK = false;     //Assume function fails
	HANDLE hToken;
	//Try to open this process's acess token
	if (OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES, &hToken))
	{
		//Attempt to modify the "Debug" privilege
		TOKEN_PRIVILEGES tp;
		tp.PrivilegeCount = 1;
		LookupPrivilegeValue(NULL, SE_DEBUG_NAME, &tp.Privileges[0].Luid);
		tp.Privileges[0].Attributes = bEnable ? SE_PRIVILEGE_ENABLED : 0;
		AdjustTokenPrivileges(hToken, FALSE, &tp, sizeof(tp), NULL, NULL);
		fOK  = (GetLastError() ==  ERROR_SUCCESS);
		CloseHandle(hToken);
	}
	return fOK;
}


