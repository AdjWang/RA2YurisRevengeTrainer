#include "autoassembler.h"

vector<tcheats> cheats;
//vector<string> registeredsymbols;


vector<TUserdefinedsymbol> userdefinedsymbols;






/*
// assemblerunit.pas: tokenize
vector<string> opcodeTokenize(string opcode)
{
	int i,last;
	int j;
	int spacecount;
	int seperatorcount;

	bool firstquote;

	vector<string> tokens;

	while (opcode.size()>0 && ((opcode[opcode.size()-1]=' ') || (opcode[opcode.size()-1]=','))){
		opcode=opcode.substr(0,opcode.size()-1);
	}

	last=0;
	firstquote=false;
	for(i=0;i<opcode.size();i++)
	{
		if( (i==opcode.size()-1) || (opcode[i]==' ') || (opcode[i]==',') || (opcode[i]='\''))
		{
			if(!firstquote)
			{
				//setlength(tokens,length(tokens)+1);
				if(i==opcode.size()-1)
					tokens.push_back(opcode.substr(last,i-last+1));
				else
					tokens.push_back(opcode.substr(last,i-last));


				if (_strCmpNoCase(tokens[tokens.size()-1], "KERNEL_"))
					tokens[tokens.size()-1]=toUpperCase(tokens[tokens.size()-1]);


				if ((tokens[tokens.size()-1]=="DQWORD")
					|| (tokens[tokens.size()-1]=="TBYTE")
					|| (tokens[tokens.size()-1]=="TWORD")
					|| (tokens[tokens.size()-1]=="QWORD")
					|| (tokens[tokens.size()-1]=="DWORD")
					|| (tokens[tokens.size()-1]=="WORD")
					|| (tokens[tokens.size()-1]=="BYTE")
					|| (tokens[tokens.size()-1]=="DQWORD PTR")
					|| (tokens[tokens.size()-1]=="TBYTE PTR")
					|| (tokens[tokens.size()-1]=="TWORD PTR")
					|| (tokens[tokens.size()-1]=="QWORD PTR")
					|| (tokens[tokens.size()-1]=="DWORD PTR")
					|| (tokens[tokens.size()-1]=="WORD PTR")
					|| (tokens[tokens.size()-1]=="BYTE PTR")
					|| (tokens[tokens.size()-1]=="SHORT")
					|| (tokens[tokens.size()-1]=="LONG")
					|| (tokens[tokens.size()-1]=="FAR"))         
				{
					tokens.pop_back();
				}
				else
				{
					last=i+1;

					// ???
					//if (tokens.size()>1)
					//{
					//    //Rewrite
					//    rewrite(tokens[length(tokens)-1]);
					//}
				}

				if (opcode[i]='\'')
					firstquote=true;

			}
			else
			{
				//inside a quote and a token seperator was encountered
				if (opcode[i]='\'')  //check if it is the string terminator
				{
					firstquote=false;
					if (i==opcode.size()-1)
						tokens[tokens.size()-1]=opcode.substr(last-1,i-last+2);
					else
						tokens[tokens.size()-1]=opcode.substr(last-1,i-last+2);
					last=i+1;
				}

			}
		}

	}

	i=0;
	while( i<tokens.size())
	{
		if( (tokens[i]=="") || (tokens[i]==" ") || (tokens[i]==","))
		{

			tokens.erase(tokens.begin()+i);
			//for (j=i ;j<= tokens.size()-2; j++)
			//  tokens[j]=tokens[j+1];
			//setlength(tokens,length(tokens)-1);
			continue;
		}
		i++;
	}

	return tokens;
}
*/






bool get2params(string commandline, t2params *params){

	int a = commandline.find('(');
	int b = commandline.find(',');
	int c = commandline.find(')');

	if ((a>0) && (b>a+1) && (c>b+1))
	{
		params->s1 = _trim(commandline.substr(a+1,b-a-1));   // name
		params->s2 = _trim(commandline.substr(b+1,c-b-1));   // size
		return true;
	}
	return false;

}


//void ReinitializeUserdefinedSymbolList(HANDLE processhandle)
//{
//	int i;
//	DWORD x;
//	for(i=0; i < userdefinedsymbols.size(); i++){
//		if(_getAddressFromName(processhandle, userdefinedsymbols[i].addressstring, &x)){
//			userdefinedsymbols[i].address = x;
//
//		}
//	}
//}


bool DeleteUserdefinedSymbol(HANDLE processhandle, string symbolname)
{
	int i,j;
	bool found = false;
    for(i=0; i < userdefinedsymbols.size(); i++)
	{
      if(_strCmpNoCase(userdefinedsymbols[i].symbolname, symbolname))
	  {
        //found it
        //check if it had a alloc, if so, free it
        if (userdefinedsymbols[i].allocsize > 0)
          VirtualFreeEx(processhandle, (LPVOID)userdefinedsymbols[i].address, 0 ,MEM_RELEASE);

	        //now move up all the others and decrease the list
		userdefinedsymbols.erase(userdefinedsymbols.begin() + i);

        return true;
	  }
	}
	return false;
	// ???
  //if assigned(UserdefinedSymbolCallback) then
  //  UserdefinedSymbolCallback();
}



bool SetUserdefinedSymbolAllocSize(HANDLE processhandle, string symbolname, DWORD size)
{
	//This function will find the userdefined symbol, and when found checks if it already
	//allocated memory. If not allocate memory, else check if the size matches
	int i;
	LPVOID p;


	if(size==0){
		print("Please provide a bigger size");
		return false;
	}


	for(i = 0; i < userdefinedsymbols.size(); i++){
		if(_strCmpNoCase(userdefinedsymbols[i].symbolname,symbolname))
		{

			//it exists, check first
			if (userdefinedsymbols[i].allocsize>0)
			{
				//already allocated and processid is the same
				if(size != userdefinedsymbols[i].allocsize){
					print("The symbol named "+userdefinedsymbols[i].symbolname+" was previously declared with a size of "+int2String(userdefinedsymbols[i].allocsize)+" instead of "+int2String(size)+". all scripts that use this memory must give the same size. Adjust the size, or delete the old alloc from the userdefined symbol list");
					return false;
				}
			}


			return true;
		}
	}



	//doesn't exist yet. Add it
	p = VirtualAllocEx(processhandle,NULL,size,MEM_COMMIT , PAGE_EXECUTE_READWRITE);
	if(p == NULL){
		print("Error allocating memory");
		return false;
	}


	TUserdefinedsymbol symboltemp;
	symboltemp.address=(DWORD)p;
	symboltemp.addressstring=int2HexString((DWORD)p);
	symboltemp.symbolname=symbolname;
	symboltemp.allocsize=size;
	symboltemp.processid=0;

	userdefinedsymbols.push_back(symboltemp);



	return true; //managed to get here without crashing...

	//if assigned(UserdefinedSymbolCallback) then
	//  UserdefinedSymbolCallback();

}




bool GetUserdefinedSymbolByName(string symbolname, DWORD *address){
	for(int i = 0; i < userdefinedsymbols.size(); i++){
		if(_strCmpNoCase(userdefinedsymbols[i].symbolname,symbolname))
		{
			*address = userdefinedsymbols[i].address;
			return true;
		}
	}
	*address = 0;
	return false;
}



//string GetUserdefinedSymbolByAddress(DWORD address)
//{
//	for(int i = 0; i < userdefinedsymbols.size(); i++){
//		if(userdefinedsymbols[i].address==address)
//		{
//			return userdefinedsymbols[i].symbolname;
//		}
//	}
//	return "";
//}



bool AddUserdefinedSymbol(HANDLE processhandle, string addressstring, string symbolname)
//This routine will add the symbolname+address combination to the symbollist
{
	DWORD address;
	//bool error;

	if(GetUserdefinedSymbolByName(symbolname, &address)){
		print(symbolname + " already exists");
		return false;
	}


	if(!_getAddressFromName(processhandle, addressstring, &address)){
		print(symbolname + "You can''t add a symbol with address 0");
		return false;
	}

	TUserdefinedsymbol symboltemp;
	symboltemp.address=address;
	symboltemp.addressstring=addressstring;
	symboltemp.symbolname=symbolname;
	symboltemp.allocsize=0;
	symboltemp.processid=0;

	userdefinedsymbols.push_back(symboltemp);

	return true;

	// ???
  //if assigned(UserdefinedSymbolCallback) then
  //  UserdefinedSymbolCallback();
}



bool isAtoF(char c){
	if((c>='a' && c<='f') || (c>='A' && c<='F') )
		return true;
	else
		return false;
}

string autoHexLeading0Adder(string s){

	vector<string> tokens;
	int i;
	int last = 0;
	string t = "";
	DWORD dwordtemp = 0;

	for(i = 0; i < s.size(); i++){
		if (s[i] == '[' || s[i] == ']' || s[i] == '+' || s[i] == '-' || s[i] == '*' || s[i] == ',' || s[i] == ' ' || s[i] == '\t'){
			t = s.substr(last, i-last);
			if(t.size()>0)
				tokens.push_back(t);
			tokens.push_back(s.substr(i,1));
			last = i + 1;
		}
	}

	//last part
	tokens.push_back(s.substr(last,s.size()-last));

	t = "";
	for(i = 1; i < tokens.size(); i++){
		if(isAtoF(tokens[i][0]) && HexStringToDword(tokens[i], &dwordtemp)){
			tokens[i] = "0"+tokens[i];
			//print("\""+tokens[i]+"\"");
		}
	}
	for(i = 0; i < tokens.size(); i++){
		t += tokens[i];
	}
	//print(t);
	return t;

}


string aotuDwordPtrAdder(string asmline){


	int pos = findWordMatch2(asmline, "[", true);
	if(pos==-1)
		return asmline;

	pos = findWordMatch2(asmline, "ptr", true);
	if(pos!=-1)
		return asmline;

	return replacetoken(asmline, "[", "dword ptr [");
}


// assembler function
bool _assemble(DWORD opcodesAddr, string asmline, vector<BYTE> &result){
	//MessageBoxA(0,asmline.c_str(),0,0);

	char szError[TEXTLEN];


	// #123   (int)123   (float)123.4   (double)123.4
	asmline = replaceBase10NotationToHex(asmline);



	int dCommand = check_DB_DW_DD_DQ(asmline);

	// DB
	if(dCommand == 1){
		vector<string> dbCommand = split((char *)asmline.c_str(), ' ');

		for(int i = 1; i < dbCommand.size(); i++){
			DWORD dwordtemp;
			if(HexStringToDword(dbCommand[i], &dwordtemp)){
				BYTE b = (BYTE)dwordtemp;
				result.push_back(b);
			}else{
				return false;
			}
		}
		return true;
	}

	// DW
	if(dCommand == 2){
		// translate dw command
		vector<string> dbCommand = split((char *)asmline.c_str(), ' ');

		for(int i = 1; i < dbCommand.size(); i++){
			DWORD dwordtemp;
			if(HexStringToDword(dbCommand[i], &dwordtemp)){
				BYTE b = (BYTE)(dwordtemp & 0xff);
				result.push_back(b);
				b = (BYTE)(dwordtemp >> 8 & 0xff);
				result.push_back(b);
			}else{
				return false;
			}
		}
		return true;
	}

	// DD
	if(dCommand == 3){
		// translate dd command
		vector<string> dbCommand = split((char *)asmline.c_str(), ' ');

		for(int i = 1; i < dbCommand.size(); i++){
			DWORD dwordtemp;
			if(HexStringToDword(dbCommand[i], &dwordtemp)){
				BYTE b = (BYTE)(dwordtemp & 0xff);
				result.push_back(b);
				b = (BYTE)(dwordtemp >> 8 & 0xff);
				result.push_back(b);
				b = (BYTE)(dwordtemp >> 16 & 0xff);
				result.push_back(b);
				b = (BYTE)(dwordtemp >> 24 & 0xff);
				result.push_back(b);
			}else{
				return false;
			}
		}
		return true;
	}

	// DQ
	if(dCommand == 4){	// translate dq command
		// translate dd command
		vector<string> dbCommand = split((char *)asmline.c_str(), ' ');

		for(int i = 1; i < dbCommand.size(); i++){
			dbCommand[i] = "00000000000000000" + dbCommand[i];

			DWORD d1 = 0;
			DWORD d2 = 0;
			if(!HexStringToDword(dbCommand[i].substr(dbCommand[i].size()-8), &d1)){
				return false;
			}
			if(!HexStringToDword(dbCommand[i].substr(dbCommand[i].size()-16,8), &d2)){
				return false;
			}

			BYTE b = (BYTE)(d1 & 0xff);
			result.push_back(b);
			b = (BYTE)(d1 >> 8 & 0xff);
			result.push_back(b);
			b = (BYTE)(d1 >> 16 & 0xff);
			result.push_back(b);
			b = (BYTE)(d1 >> 24 & 0xff);
			result.push_back(b);

			b = (BYTE)(d2 & 0xff);
			result.push_back(b);
			b = (BYTE)(d2 >> 8 & 0xff);
			result.push_back(b);
			b = (BYTE)(d2 >> 16 & 0xff);
			result.push_back(b);
			b = (BYTE)(d2 >> 24 & 0xff);
			result.push_back(b);

		}
		return true;
	}

	





		// decide minimumn length codes
		int byteAssembled = 0;
		t_asmmodel am;
		t_asmmodel minAM;
		minAM.length = MAXCMDSIZE + 1;
		for(int i = 0; i < 4; i++){
			for(int j = 0; j < 4; j++){
				byteAssembled = Assemble((char *)asmline.c_str(), opcodesAddr, &am, i, j, szError);
				if (byteAssembled > 0 && am.length > 0 && am.length < minAM.length){
					for(int k = 0; k < am.length; k++){
						minAM.code[k] = am.code[k];
						minAM.mask[k] = am.mask[k];
					}
					minAM.length = am.length;
					minAM.jmpoffset = am.jmpoffset;
					minAM.jmppos = am.jmppos;
					minAM.jmpsize = am.jmpsize;
				}
			}
		}

		string realerror = string(szError);

		if((minAM.length == MAXCMDSIZE + 1)
			&& (0 == findWordMatch2(string(szError), "Unknown identifier",true)))
		{
			asmline = autoHexLeading0Adder(asmline);
			// do it again
			byteAssembled = 0;
			minAM.length = MAXCMDSIZE + 1;
			for(int i = 0; i < 4; i++){
				for(int j = 0; j < 4; j++){
					byteAssembled = Assemble((char *)asmline.c_str(), opcodesAddr, &am, i, j, szError);
					if (byteAssembled > 0 && am.length > 0 && am.length < minAM.length){
						for(int k = 0; k < am.length; k++){
							minAM.code[k] = am.code[k];
							minAM.mask[k] = am.mask[k];
						}
						minAM.length = am.length;
						minAM.jmpoffset = am.jmpoffset;
						minAM.jmppos = am.jmppos;
						minAM.jmpsize = am.jmpsize;
					}
				}
			}
		}


		if((minAM.length == MAXCMDSIZE + 1) 
			&& ((0 == findWordMatch2(string(szError), "Command does not support given operands",true)
			|| (0 == findWordMatch2(string(szError), "Unrecognized command",true)))))
		{
			asmline = aotuDwordPtrAdder(asmline);
			// do it again
			byteAssembled = 0;
			minAM.length = MAXCMDSIZE + 1;
			for(int i = 0; i < 4; i++){
				for(int j = 0; j < 4; j++){
					byteAssembled = Assemble((char *)asmline.c_str(), opcodesAddr, &am, i, j, szError);
					if (byteAssembled > 0 && am.length > 0 && am.length < minAM.length){
						for(int k = 0; k < am.length; k++){
							minAM.code[k] = am.code[k];
							minAM.mask[k] = am.mask[k];
						}
						minAM.length = am.length;
						minAM.jmpoffset = am.jmpoffset;
						minAM.jmppos = am.jmppos;
						minAM.jmpsize = am.jmpsize;
					}
				}
			}

		}





		// failed all assembling tries
		if(minAM.length == MAXCMDSIZE + 1){
			//char errinfo[256];
			//wsprintfA(errinfo, "%s: %s", (char *)asmline.c_str(), szError);
			//MessageBoxA(0,errinfo,0,0);
			print(realerror);
			return false;
		}

		// succeeded
		for(int i = 0; i < minAM.length; i++){
			BYTE b = (BYTE)(minAM.code[i] & minAM.mask[i]);
			result.push_back(b);
			opcodesAddr++;
		}

	return true;

}






// Inject DLL (from CE 5.61: CEFuncProc.pas)
bool InjectDll(HANDLE processhandle, string dllname, string functiontocall)
{
	DWORD LoadLibraryPtr;
	DWORD GetProcAddressPtr;


	HMODULE h;

	BYTE inject[4096];
	DWORD x;

	vector<BYTE> outp;
    // WARN: suppress warning
	int counter = 0;
	DWORD position,position2;

	string dllLocation;
	DWORD startaddress;
	DWORD functionloc;
	DWORD injectionlocation;
	HANDLE threadhandle;


	int i;

	h=LoadLibraryA("Kernel32.dll");
	if (h==0) {
		print("No kernel32.dll loaded");
		return false;
	}

	injectionlocation=0;

	if(!_getAddressFromName(processhandle, "GetProcAddress", &GetProcAddressPtr)){
		GetProcAddressPtr=(DWORD)GetProcAddress(h,"GetProcAddress");
	}

	if(GetProcAddressPtr==0){
		print("GetProcAddress not found");
		return false;
	}

	if(!_getAddressFromName(processhandle, "LoadLibraryA", &LoadLibraryPtr)){
		//failed getting the address of LoadLibraryA, use old method
		LoadLibraryPtr=(DWORD)GetProcAddress(h,"LoadLibraryA");
	}


	if(LoadLibraryPtr==0){
		print("LoadLibraryA not found");
		return false;
	}

	injectionlocation=(DWORD)VirtualAllocEx(processhandle,NULL,4096,MEM_COMMIT,PAGE_EXECUTE_READWRITE);

	if(injectionlocation==0){
		print("Failed to allocate memory");
		return false;
	}

	dllLocation=dllname;

	position=injectionlocation;
	position2=0;
	for(i = 0; i < dllLocation.size(); i++){
		inject[i] = dllLocation[i];
	}
	inject[i] = 0;


	position += dllLocation.size()+1;
	position2 += dllLocation.size()+1;

	functionloc=position;
	for(i = position2; i < position2+functiontocall.size(); i++){
		inject[i] = functiontocall[i];
	}
	inject[i] = 0;
	position += functiontocall.size()+1;
	position2 += functiontocall.size()+1;
	startaddress=position;

	//loadlibrary(cehook);
	outp.clear();
	_assemble(position, "PUSH "+ int2HexString(injectionlocation), outp);

	for(i = position2; i < position2+outp.size(); i++){
		inject[i] = outp[i];
	}
	position += outp.size();
	position2 += outp.size();

	outp.clear();
	_assemble(position, "CALL " + int2HexString(LoadLibraryPtr), outp);
	for(i = position2; i < position2+outp.size(); i++){
		inject[i] = outp[i];
	}
	position += outp.size();
	position2 += outp.size();


	//safetycode, test if the dll was actually loaded and skip if not
	outp.clear();
	_assemble(position, "TEST EAX,EAX",outp);
	for(i = position2; i < position2+outp.size(); i++){
		inject[i] = outp[i];
	}
	position += outp.size();
	position2 += outp.size();

	outp.clear();
	_assemble(position, "JNE " + int2HexString(position+3+5), outp); //jump over the ret
	for(i = position2; i < position2+outp.size(); i++){
		inject[i] = outp[i];
	}
	position += outp.size();
	position2 += outp.size();

	outp.clear();
	_assemble(position, "MOV EAX,2", outp); //exitcode=2
	for(i = position2; i < position2+outp.size(); i++){
		inject[i] = outp[i];
	}
	position += outp.size();
	position2 += outp.size();

	outp.clear();
	_assemble(position, "RET", outp);
	for(i = position2; i < position2+outp.size(); i++){
		inject[i] = outp[i];
	}
	position += outp.size();
	position2 += outp.size();


	if(functiontocall.size()>0)
	{
		//getprocaddress
		outp.clear();
		_assemble(position, "PUSH " + int2HexString(functionloc), outp);
		for(i = position2; i < position2+outp.size(); i++){
			inject[i] = outp[i];
		}
		position += outp.size();
		position2 += outp.size();

		outp.clear();
		_assemble(position, "PUSH EAX",outp);
		for(i = position2; i < position2+outp.size(); i++){
			inject[i] = outp[i];
		}
		position += outp.size();
		position2 += outp.size();

		outp.clear();
		_assemble(position, "CALL " + int2HexString(GetProcAddressPtr), outp);
		for(i = position2; i < position2+outp.size(); i++){
			inject[i] = outp[i];
		}
		position += outp.size();
		position2 += outp.size();

		outp.clear();
		_assemble(position, "TEST EAX,EAX", outp);
		for(i = position2; i < position2+outp.size(); i++){
			inject[i] = outp[i];
		}
		position += outp.size();
		position2 += outp.size();

		outp.clear();
		_assemble(position, "JNE " + int2HexString(position+3+5),outp);
		for(i = position2; i < position2+outp.size(); i++){
			inject[i] = outp[i];
		}
		position += outp.size();
		position2 += outp.size();

		outp.clear();
		_assemble(position, "MOV EAX,3", outp); //exitcode=3
		for(i = position2; i < position2+outp.size(); i++){
			inject[i] = outp[i];
		}
		position += outp.size();
		position2 += outp.size();

		outp.clear();
		_assemble(position, "RET", outp);
		for(i = position2; i < position2+outp.size(); i++){
			inject[i] = outp[i];
		}
		position += outp.size();
		position2 += outp.size();

		//call function
		outp.clear();
		_assemble(position, "CALL EAX", outp);
		for(i = position2; i < position2+outp.size(); i++){
			inject[i] = outp[i];
		}
		position += outp.size();
		position2 += outp.size();


		outp.clear();
		_assemble(position, "MOV EAX,1", outp); //causes the exitcode of the thread be 1
		for(i = position2; i < position2+outp.size(); i++){
			inject[i] = outp[i];
		}
		position += outp.size();
		position2 += outp.size();

		outp.clear();
		_assemble(position, "RET", outp);
		for(i = position2; i < position2+outp.size(); i++){
			inject[i] = outp[i];
		}
		position += outp.size();
		position2 += outp.size();

	}
	//call the routine

	if(!WriteProcessMemory(processhandle,(LPVOID)injectionlocation,inject,position2,&x)){
		print("Failed to inject the dll loader");
		return false;	
	}

	//{$ifndef standalonetrainer} 
	//{$ifndef net}   

	//useapctoinjectdll:=false;
	//if useapctoinjectdll then
	//{
	//  showmessage('injected code at:'+inttohex(startaddress,8));
	//  
	//  //susp} , message, resume is needed to prevent a crash when it is in a message loop 
	//  ntsusp}process(processid);
	//  x:=getathreadid(processid);
	//  PostThreadMessage(x,wm_paint,0,0);
	//  CreateRemoteAPC(x,pointer(startaddress));
	//  ntresumeprocess(processid);
	//}
	//else

	//{$}if}
	//{$}if}
	//{      
	threadhandle=CreateRemoteThread(processhandle,NULL,0,(LPTHREAD_START_ROUTINE)startaddress,NULL,0,&x);
	if(threadhandle==0){
		print("Failed to execute the dll loader");
		FreeLibrary(h);
		if(injectionlocation!=0)
			VirtualFreeEx(processhandle,(LPVOID)injectionlocation,0,MEM_RELEASE	);
		return false;
	}

	/*
	counter=10000 / 10;
	while ((WaitForSingleObject(threadhandle,10)==WAIT_TIMEOUT) && (counter>0))
	{
	if(GetCurrentThreadID() == MainThreadID)
	CheckSynchronize(); //handle sychronize calls while it's waiting

	counter--;
	}
	*/
	if (counter==0){
		print("The injection thread took longer than 10 seconds to execute. Injection routine not freed");
		FreeLibrary(h);
		if(injectionlocation!=0)
			VirtualFreeEx(processhandle,(LPVOID)injectionlocation,0,MEM_RELEASE	);
		return false;
	}

	if(GetExitCodeThread(threadhandle,&x))
	{
		switch(x){
		  case 1: ;//success
			  break;

		  case 2: 
			  print("Failed injecting the DLL");
			  FreeLibrary(h);
			  if(injectionlocation!=0)
				  VirtualFreeEx(processhandle,(LPVOID)injectionlocation,0,MEM_RELEASE	);
			  return false;
			  break;

		  case 3:
			  print("Failed executing the function of the dll");
			  FreeLibrary(h);
			  if(injectionlocation!=0)
				  VirtualFreeEx(processhandle,(LPVOID)injectionlocation,0,MEM_RELEASE	);
			  return false;
			  break;

		  default:
			  print("Unknown error during injection");
			  FreeLibrary(h);
			  if(injectionlocation!=0)
				  VirtualFreeEx(processhandle,(LPVOID)injectionlocation,0,MEM_RELEASE	);
			  return false;

		}
	} //else unsure, did it work or not , or is it crashing?

	//}

	FreeLibrary(h);
	if(injectionlocation!=0)
		VirtualFreeEx(processhandle,(LPVOID)injectionlocation,0,MEM_RELEASE	);

	return true;
}









bool _autoassemble2(HANDLE processhandle, vector<string> code, vector<tcealloc> &ceallocarray)
{
	//提升为调适权限
	EnableDebugPrivilege(true);

	// 取得所有模块名称与其对应地址
	init_getModuleList(processhandle);

	int i,j,k,l,e;
    string currentline;
    char *currentlinep;

    // WARN: suppress warning
    DWORD currentaddress = 0;
    vector<tassembled> assembled;
    DWORD x,y,op,op2;
    bool ok1,ok2;
    vector<tloadbinary> loadbinary;


    vector<tcealloc> globalallocs, allocs, kallocs;
    vector<tlabel> labels;
    vector<tdefine> defines;
    vector<tfullaccess> fullaccess;
    vector<DWORD> dealloc;
    vector<string> addsymbollist;
    vector<string> deletesymbollist;
    vector<string> createthread;

    int a,b,c,d;
    string s1,s2,s3;

    vector<string> assemblerlines;

    int varsize;
    vector<string> tokens;
    DWORD baseaddress;

    vector<string> include;
    DWORD testdword,bw;
    string binaryfile;  // binaryfile: tmemorystream;

    bool incomment;

    vector<BYTE> bytebuf;

    DWORD ProcessID;

    vector<BYTE> bytes;


	t2params params_2;

    // symhandler: TSymhandler;


  //if syntaxcheckonly and (registeredsymbols<>nil) then
  //{
  //  //add the symbols as defined labels
  //  setlength(labels,registeredsymbols.count);
  //  for i = 0 to registeredsymbols.count-1 do
  //  {
  //    labels[i].labelname = registeredsymbols[i];
  //    labels[i].defined = true;
  //    labels[i].address = 0;
  //    labels[i].assemblerline = 0;
  //    setlength(labels[i].references,0);
  //    setlength(labels[i].references2,0);
  //  };
  //};



//  symhandler.waitforsymbolsloaded;
//





//2 pass scanner

//try{
    
	//setlength(assembled,1);
tassembled assembledtemp;
assembled.push_back(assembledtemp);


    //tokens = tstringlist.Create;

    //incomment = false;



    // removecomments(code);  // has been done earlier
   
	// unlabeledlabels(code);


    //first pass
    for(i = 0; i < code.size(); i++)
    {

          currentline = code[i];


			  // currentline = _trim(currentline); // has been done in removecomments()

		//// define() 取代功能
		  for(j = 0; j < defines.size(); j++){
			currentline = replacetoken(currentline,defines[j].name,defines[j].whatever);
		  }



		  //     if (currentline.size() <= 0)    // 忽略空行  has been done earlier
			  //continue;
		  //     if copy(currentline,1,2)='//' then continue; //skip   // has been done in removecomments()


		  // 加入 assemberlines
			assemblerlines.push_back(currentline);



			// 开始寻找分析特殊命令(脚本函数)



			////////////  ASSERT
			if (true == _strCmpNoCase(currentline.substr(0,7), "ASSERT(")) //assert(address,aob)
			{

				if(get2params(currentline,&params_2))
				{
					s1 = params_2.s1;		// address
					s2 = params_2.s2;		// aob

					testdword =  0;
					if(!_getAddressFromName(processhandle,s1,&testdword)){
						s1 += " is not a valid name or address";
						MessageBoxA(0,s1.c_str(),0,0);
						return false;
					}

					vector<WORD> aobWithUnknown = _stringToAOBwithUnknown(s2);
					if(aobWithUnknown.size() <= 0){
						s2 += " is not a valid bytestring";
						MessageBoxA(0,s2.c_str(),0,0);
						return false;
					}

					BYTE *aobuff = NULL;
					aobuff = new BYTE[aobWithUnknown.size()];

					if(0 != ReadProcessMemory(processhandle, (LPVOID)testdword, aobuff, aobWithUnknown.size(), &x) && x == aobWithUnknown.size())
					{
						for(j = 0; j < aobWithUnknown.size(); j++)
						{
							if(aobWithUnknown[j] < 0x0100){					// 0x0100 = ??, *
								if((BYTE)(aobWithUnknown[j]) != aobuff[j]){
									s1 = "The bytes at " + s1 + " are not what was expected";
									MessageBoxA(0,s1.c_str(),0,0);
									delete [] aobuff;
									aobuff = NULL;
									return false;
								}
							}
						}

						delete [] aobuff;
						aobuff = NULL;

					} else{
						s1 = "The memory at " + s1 + " can not be read";
						MessageBoxA(0,s1.c_str(),0,0);
						delete [] aobuff;
						aobuff = NULL;
						return false;
					}
				}
				else{
					MessageBoxA(0,"Wrong syntax. ASSERT(address,11 22 33 ** 55 66)",0,0);
					return false;
				}
				assemblerlines.pop_back();
				continue;
			}

	


			////////////  ASSERT2
			if (true == _strCmpNoCase(currentline.substr(0,8), "ASSERT2(")) //assert(address,aob)
			{


				a = currentline.find('(');
				//b = currentline.find(',');
				c = currentline.find(')');

				if ((a>0) && (c>a+1))
				{
					vector<string> params = split((char *)currentline.substr(a+1,c-a-1).c_str(),',');
					if(params.size() != 3){
						MessageBox(0,TEXT("语法错误: assert2(address,aob,\"failing message\")"),0,0);
						return false;
					}
					s1 = _trim(params[0]);   // varname
					s2 = _trim(params[1]);   // bytestring

					string failMsg = _trim(params[2]);
					bool msgOK = false;
					if(failMsg.size()>=2 && failMsg[0]=='\"' && failMsg[failMsg.size()-1]=='\"'){
						msgOK=true;
					}

					if(!msgOK){
						MessageBox(0,TEXT("assert2 参数错误"),0,0);
						return false;
					}


					testdword =  0;
					if(!_getAddressFromName(processhandle,s1,&testdword)){
						s1 += " is not a valid name or address";
						MessageBoxA(0,s1.c_str(),0,0);
						return false;
					}

					vector<WORD> aobWithUnknown = _stringToAOBwithUnknown(s2);
					if(aobWithUnknown.size() <= 0){
						s2 += " is not a valid bytestring";
						MessageBoxA(0,s2.c_str(),0,0);
						return false;
					}

					BYTE *aobuff = NULL;
					aobuff = new BYTE[aobWithUnknown.size()];

					if(0 != ReadProcessMemory(processhandle, (LPVOID)testdword, aobuff, aobWithUnknown.size(), &x) && x == aobWithUnknown.size())
					{
						for(j = 0; j < aobWithUnknown.size(); j++)
						{
							if(aobWithUnknown[j] < 0x0100){					// 0x0100 = ??, *
								if((BYTE)(aobWithUnknown[j]) != aobuff[j]){

									if(failMsg.size()>2){
										MessageBoxA(0,failMsg.substr(1,failMsg.size()-2).c_str(),0,0);
									}
									delete [] aobuff;
									aobuff = NULL;
									return false;
								}
							}
						}

						delete [] aobuff;
						aobuff = NULL;

					} else{

						if(failMsg.size()>2){
							MessageBoxA(0,failMsg.substr(1,failMsg.size()-2).c_str(),0,0);
						}
						delete [] aobuff;
						aobuff = NULL;
						return false;

					}
				}
				else{
					MessageBoxA(0,"Wrong syntax. ASSERT(address,11 22 33 ** 55 66)",0,0);
					return false;
				}
				assemblerlines.pop_back();
				continue;
			}






			///////////  GLOBALALLOC
			if (true == _strCmpNoCase(currentline.substr(0,12), "GLOBALALLOC("))
			{

				if(get2params(currentline,&params_2))
				{
					s1 = params_2.s1;   // name
					s2 = params_2.s2;   // size

					int size = atoi(s2.c_str());
					if(size <= 0){
						s2 += " is not a valid size";
						MessageBoxA(0,s2.c_str(),0,0);
						return false;
					}
					x = size;

					////define it here already
					SetUserdefinedSymbolAllocSize(processhandle, s1, x);              


					tcealloc globalalloc;
					DWORD dwordtemp = 0;

					GetUserdefinedSymbolByName(s1, &dwordtemp);
					globalalloc.address =dwordtemp;
					globalalloc.varname = s1;
					globalalloc.size = x;
					globalallocs.push_back(globalalloc);

					assemblerlines.pop_back(); // 此行已处理
					continue;

				}
				else{
					MessageBoxA(0,"Wrong syntax. GLOBALALLOC(name,size)",0,0);
					return false;
				}
			}






			////////////     INCLUDE
          //if uppercase(copy(currentline,1,8))='INCLUDE(' then
          //{
          //  a = pos('(',currentline);
          //  b = pos(')',currentline);

          //  if (a>0) and (b>0) then
          //  {
          //    s1 = copy(currentline,a+1,b-a-1);

          //    if ExtractFileExt(uppercase(s1))='.' then
          //      s1 = s1+'CEA';

          //    if ExtractFileExt(uppercase(s1))='' then
          //      s1 = s1+'.CEA';

          //    if not fileexists(s1) then
          //      raise exception.Create(s1+' could not be found');


          //    include = tstringlist.Create;
          //    try
          //      include.LoadFromFile(s1);
          //      for j = i+1 to (i+1)+(include.Count-1) do
          //        code.Insert(j,include[j-(i+1)]);
          //    finally
          //      include.Free;
          //    };

          //    setlength(assemblerlines,length(assemblerlines)-1);
          //    continue;
          //  }
          //  else raise exception.Create('Wrong syntax. Include(filename.cea)');
          //};





		  /////////////      CREATETHREAD
		  if(true == _strCmpNoCase(currentline.substr(0,13), "CREATETHREAD("))
          {
            //load a binary file into memory , this one already executes BEFORE the 2nd pass to get addressnames correct
            a = currentline.find('(');
            b = currentline.find(')');
            if ((a>0) && (b>a+1))
            {
              s1 = _trim(currentline.substr(a+1,b-a-1));   // name
            
			  createthread.push_back(s1);

				assemblerlines.pop_back(); // 此行已处理
              continue;
			} else{
				MessageBoxA(0,"Wrong syntax. CreateThread(address)",0,0);
				return false;
			}
		  }





		  /////   LOADLIBRARY
		  if(true == _strCmpNoCase(currentline.substr(0,12), "LOADLIBRARY("))
		  {
			  //load a library into memory , this one already executes BEFORE the 2nd pass to get addressnames correct
			  a = currentline.find('(');
			  b = currentline.find(')');

			  if ((a>0) && (b>a+1))
			  {
				  s1 = _trim(currentline.substr(a+1,b-a-1)); 

				  if(s1.size()>0 && s1[0] ==':')
				  {
					  // ???
					  //s2 = extractfilename(s1);

					  //if( fileexists(cheatenginedir+s2)){
						 // s1 = cheatenginedir+s2 ;
					  //}else if( fileexists(getcurrentdir+'\\'+s2)){
						 // s1 = getcurrentdir+'\\'+s2 ;
					  //}else if( fileexists(cheatenginedir+s1)){
						 // s1 = cheatenginedir+s1;
					  //}

					  //else just hope it's in the dll searchpath
				  } //else direct file path


					MessageBox(0,TEXT("LOADLIBRARY 功能未完成"),0,0);
				  //if(InjectDll(processhandle, s1, "")){
					 // // ???
					 // //		  symhandler.reinitialize;
					 // //		  symhandler.waitforsymbolsloaded;
				  //}else{
					 // print(s1+" could not be injected");
					 // return false;
				  //}

				  assemblerlines.pop_back(); 
				  continue;
			  } else {
				  print("Wrong syntax. LoadLibrary(filename)");
				  return false;
			  }
		  }



		  /////////   READMEM
		  if (true == _strCmpNoCase(currentline.substr(0,8), "READMEM(")) //readmem(address,size)
		  {

			  if(get2params(currentline,&params_2))
			  {
				  s1 = params_2.s1;   // address
				  s2 = params_2.s2;   // size

				  //read memory and replace with lines of DB xx xx xx xx xx xx xx xx
				  x = 0;
				  if(!_getAddressFromName(processhandle, s1, &x)){
					  MessageBoxA(0,"Invalid address for ReadMem",0,0);
					  return false;
				  }


				  a = atoi((char *)s2.c_str());
				  if(a<=0){
					  MessageBoxA(0,"Invalid size for ReadMem",0,0);
					  return false;  
				  }

				  BYTE *buffer = NULL;
				  buffer = new BYTE[a];
				  DWORD actualread = 0;
				  if(0 == ReadProcessMemory(processhandle, (LPVOID)x, buffer, a, &actualread) || actualread < a)
				  {
					  delete [] buffer;
					  buffer = NULL;
					  s1 = "The memory at " + s1 + " could not be fully read";
					  MessageBoxA(0,s1.c_str(),0,0);
					  return false;
				  }

				  //still here so everything ok
				  assemblerlines.pop_back();

				  s1 = "db";

				  for(j = 0; j < a; j++)
				  {
					  s1 = s1 + " " + int2HexString(buffer[j]);
					  if((j % 16)==15)
					  {
						  assemblerlines.push_back(s1);
						  s1 = "db";
					  }
				  }

				  if(s1.size()>2)
				  {
					  assemblerlines.push_back(s1);
				  }

				  delete [] buffer;
				  buffer = NULL;

			  } else {
				  MessageBoxA(0,"Wrong syntax. ReadMem(address,size)",0,0);
				  return false;
			  }
			  continue;
		  }


			///////   LOADBINARY
   //       if uppercase(copy(currentline,1,11))='LOADBINARY(' then
   //       {
   //         //load a binary file into memory
   //         a = pos('(',currentline);
   //         b = pos(',',currentline);
   //         c = pos(')',currentline);
   //         if (a>0) and (b>0) and (c>0) then
   //         {
   //           s1 = copy(currentline,a+1,b-a-1);
   //           s2 = copy(currentline,b+1,c-b-1);

   //           if not fileexists(s2) then raise exception.Create('The file '+s2+' does not exist');

   //           setlength(loadbinary,length(loadbinary)+1);
   //           loadbinary[length(loadbinary)-1].address = s1;
   //           loadbinary[length(loadbinary)-1].filename = s2;

   //           setlength(assemblerlines,length(assemblerlines)-1);
   //           continue;
   //         } else raise exception.Create('Wrong syntax. LoadBinary(address,filename)');
   //       };



			//////////   REGISTERSYMBOL
		  if(true == _strCmpNoCase(currentline.substr(0,15), "REGISTERSYMBOL("))
          {
            //add this symbol to the register symbollist
            a = currentline.find('(');
            b = currentline.find(')');

            if ((a>0) && (b>a+1))
            {
              s1 = _trim(currentline.substr(a+1,b-a-1));   // name

			  addsymbollist.push_back(s1);


				// ???
			  //if(registeredsymbols.size()>0)
				 // registeredsymbols.push_back(s1);



            }
			else{ 
				MessageBoxA(0,"Syntax error",0,0);
				return false;
			}
				assemblerlines.pop_back(); // 此行已处理
            continue;
		  }





		  /////////   UNREGISTERSYMBOL
  		  if(true == _strCmpNoCase(currentline.substr(0,17), "UNREGISTERSYMBOL("))
          {
            //add this symbol to the register symbollist
            a = currentline.find('(');
            b = currentline.find(')');

            if ((a>0) && (b>0) && (b>a+1))
            {
              s1 = _trim(currentline.substr(a+1,b-a-1));   // name

			  deletesymbollist.push_back(s1);
            }
			else{ 
				MessageBoxA(0,"Syntax error",0,0);
				return false;
			}

				assemblerlines.pop_back(); // 此行已处理

            continue;
		  }



		  ////////   AOBSCAN
		  //AOBSCAN(variable,aobstring)  (works like define)
		  if(true == _strCmpNoCase(currentline.substr(0,8), "AOBSCAN("))
		  {
			  //convert this line from AOBSCAN(varname,bytestring) to DEFINE(varname,address)
			  if(get2params(currentline,&params_2))
			  {
				  s1 = params_2.s1;   // varname
				  s2 = params_2.s2;   // bytestring

				  //s1=varname
				  //s2=AOBstring
				  testdword = 0;

				  //clock_t t1, t2;t1 = clock();
				  bool isFound = aobscan(processhandle, 1, 0, 0x7FFFFFFF, s2, &testdword);
				  //t2 = clock();
				  //float diff = ((float)t2 - (float)t1) / 1000.0F;
				  //printInt((int)diff) ;
				  if (!isFound){
					  string strtemp = "The array of byte \'";
					  strtemp += s2;
					  strtemp += "\' could not be found";
					  MessageBoxA(0,strtemp.c_str(),0,0);
					  return false;
				  }



				  currentline = "DEFINE(" + s1 + "," + int2HexString(testdword) + ")";

					//NO CONTINUE LINE HERE
				  // 继续交给 define() 处理



			  } else{ 
				  MessageBoxA(0,"Wrong syntax. AOBSCAN(name,11 22 33 ** 55)",0,0);
				  return false;
			  }
		  }




		  ////////   AOBSCAN2
		  //AOBSCAN(variable,aobstring)  (works like define)
		  //aobscan2(name,aob,start,end,n,"failing message");
		  if(true == _strCmpNoCase(currentline.substr(0,9), "AOBSCAN2("))
		  {
			  //convert this line from AOBSCAN(varname,bytestring) to DEFINE(varname,address)
			  a = currentline.find('(');
			  //b = currentline.find(',');
			  c = currentline.find(')');

			  if ((a>0) && (c>a+1))
			  {
				  vector<string> params = split((char *)currentline.substr(a+1,c-a-1).c_str(),',');
				  if(params.size() != 6){
					  MessageBox(0,TEXT("语法错误: aobscan2(name,aob,start,end,n,\"failing message\")"),0,0);
					  return false;
				  }
				  s1 = _trim(params[0]);   // varname
				  s2 = _trim(params[1]);   // bytestring

				  //s1=varname
				  //s2=AOBstring
				  testdword = 0;

				  int Nth = atoi((char *)_trim(params[4]).c_str());

				  DWORD startaddress=0;
				  bool startOK = HexStringToDword(_trim(params[2]),&startaddress);

				  DWORD endaddress=0;
				  bool endOK = HexStringToDword(_trim(params[3]),&endaddress);

				  string failMsg = _trim(params[5]);
				  bool msgOK = false;
				  if(failMsg.size()>=2 && failMsg[0]=='\"' && failMsg[failMsg.size()-1]=='\"'){
					msgOK=true;
				  }

				  if(Nth<1 || !startOK || !endOK || !msgOK){
					  MessageBox(0,TEXT("aobscan2 参数错误"),0,0);
					  return false;
				  }
				 
				  //clock_t t1, t2;t1 = clock();
				  bool isFound = aobscan(processhandle, Nth, startaddress, endaddress, s2, &testdword);
				  //t2 = clock();
				  //float diff = ((float)t2 - (float)t1) / 1000.0F;
				  //printInt((int)diff) ;
				  if (!isFound){
					  if(failMsg.size()>2){
						  MessageBoxA(0,failMsg.substr(1,failMsg.size()-2).c_str(),0,0);
						  return false;
					  }else{
						  return false;
					  }
				  }



				  currentline = "DEFINE(" + s1 + "," + int2HexString(testdword) + ")";

					//NO CONTINUE LINE HERE
				  // 继续交给 define() 处理

			  } else{ 
				  MessageBox(0,TEXT("语法错误: aobscan2(name,aob,start,end,n,\"failing message\")"),0,0);
				  return false;
			  }
		  }




		  /////////   DEFINE
          //define                                         
   		  if(true == _strCmpNoCase(currentline.substr(0,7), "DEFINE("))        // must be after AOBSCAN
          {
			  if(get2params(currentline,&params_2))
			  {
				  s1 = params_2.s1;   // name
				  s2 = params_2.s2;   // whatever


			  for( j = 0; j < defines.size(); j++){
				  if(_strCmpNoCase(defines[j].name, s1)) {
					  string strtemp = "Define ";
					  strtemp += s1;
					  strtemp += " already defined";
					 MessageBoxA(0,strtemp.c_str(),0,0);
					return false;
				  }
			  }

			  tdefine definetemp = {s1, s2};
			  defines.push_back(definetemp);

				assemblerlines.pop_back();   //don't bother with this in the 2nd pass
              continue;
			} else {
				 MessageBoxA(0,"Wrong syntax. DEFINE(name,whatever)",0,0);
				return false;
			}
		  }





		  //////       FULLACCESS
		  if(true == _strCmpNoCase(currentline.substr(0,11), "FULLACCESS("))
		  {

			  if(get2params(currentline,&params_2))
			  {
				  s1 = params_2.s1;   // name for address
				  s2 = params_2.s2;   // size

				  tfullaccess fatemp;

				  // 待修正　getAddressFromName　及　size　检查
				  DWORD dwordtemp;
				  fatemp.size = atoi((char *)s2.c_str());

				  if(_getAddressFromName(processhandle,s1,&dwordtemp) && fatemp.size > 0){
					  fatemp.address = dwordtemp;

					  fullaccess.push_back(fatemp);
				  }else{
					  MessageBoxA(0,"Syntax error. FullAccess(address,size)",0,0);
					  return false;
				  }

			  } else {
				  MessageBoxA(0,"Syntax error. FullAccess(address,size)",0,0);
				  return false;
			  }
			  assemblerlines.pop_back(); // 此行已处理
			  continue;
		  }





		//////////   LABEL
		  if(true == _strCmpNoCase(currentline.substr(0,6), "LABEL("))
		  {
			  //syntax: label(x)  x=name of the label
			  //later on in the code there has to be a line with "labelname:"
			  a = currentline.find('(');
			  b = currentline.find(')');

			  if ((a>0) && (b>0) && (b>a+1))
			  {
				  s1 = _trim(currentline.substr(a+1,b-a-1));   // name

				  // if s1 can be converted to DWORD, then it's an invalid name
				  DWORD dwordtemp = 0;
				  if(true == HexStringToDword(s1, &dwordtemp)){
					  s1 += " is not a valid identifier";
					  MessageBoxA(0,s1.c_str(),0,0);
					  return false;
				  }

				  varsize = s1.size();
				  for( j = 0; j < labels.size(); j++)
				  {
					  if (0 == labels[j].labelname.compare(s1)){
						  s1 += " is being redeclared";
						  MessageBoxA(0,s1.c_str(),0,0);
						  return false;
					  }
				  }

				  //check for the line "labelname:"
				  ok1 = false;
				  string s1temp = s1 + ":";
				  for (j = 0; j < code.size(); j++){
					  if (0 == _trim(code[j]).compare(s1temp))
					  {
						  if(ok1){
							  string strtemp = "label " + s1 + " is being defined more than once";
							  MessageBoxA(0,strtemp.c_str(),0,0);
							  return false;
						  }
						  ok1 = true;
					  }
				  }

				  if (!ok1){
					  string strtemp = "label ";
					  strtemp += s1;
					  strtemp += " is not defined in the scriptd";
					  MessageBoxA(0,strtemp.c_str(),0,0);
					  return false;
				  }

				  //still here so ok
				  //insert it
				  tlabel label;
				  label.labelname = s1;
				  label.defined = false;

				  j=0;
				  while(j < labels.size() && labels[j].labelname.size() > s1.size())
					  j++;

				  if(j >= labels.size()){
					  labels.push_back(label);
				  }
				  else{
					    vector<tlabel>::iterator it;
						  it = labels.begin();
					  labels.insert(it + j, label);
				  }

				  assemblerlines.pop_back();		// 已处理

				  continue;
			  } else {
				  MessageBoxA(0,"Syntax Error",0,0);
				  return false;
			  }
		  }








		  ///////       DEALLOC
		  if(true == _strCmpNoCase(currentline.substr(0,8), "DEALLOC("))
		  {
			  if (ceallocarray.size() > 0) //memory dealloc=possible
			  {

				  //syntax: dealloc(x)  x=name of region to deallocate
				  //later on in the code there has to be a line with "labelname:"
				  a = currentline.find('(');
				  b = currentline.find(')');

				  if ((a>0) && (b>0) && (b>a+1))
				  {
					  s1 = _trim(currentline.substr(a+1,b-a-1));   // name

					  //find s1 in the ceallocarray
					  for(j = 0; j < ceallocarray.size(); j++)
					  {
						  if(true == _strCmpNoCase(ceallocarray[j].varname,s1))
						  {
							  dealloc.push_back(ceallocarray[j].address);
						  }
					  }
				  }
			  }
			  assemblerlines.pop_back();		// 已处理
			  continue;
		  }





		  //////////      ALLOC
		  //memory alloc
		  if(true == _strCmpNoCase(currentline.substr(0,6), "ALLOC("))
		  {
			  //syntax: alloc(x,size)    x=variable name size=bytes
			  //allocate memory

			  if(get2params(currentline,&params_2))
			  {
				  s1 = params_2.s1;   // name
				  s2 = params_2.s2;   // size

				  // if s1 can be converted to DWORD, then it's an invalid name
				  DWORD dwordtemp = 0;
				  if(true == HexStringToDword(s1, &dwordtemp)){
					  s1 += " is not a valid identifier";
					  MessageBoxA(0,s1.c_str(),0,0);
					  return false;
				  }

				  varsize = s1.size();

				  //check for duplicate identifiers
				  for(j = 0; j < allocs.size(); j++)
				  {
					  if (0 == allocs[j].varname.compare(s1)){
						  string strtemp = "The identifier " + s1 + " has already been declared";
						  MessageBoxA(0,strtemp.c_str(),0,0);
						  return false;
					  }
				  }


				  //longest varnames first so the rename of a shorter matching var wont override the longer one
				  //move up the other allocs so I can inser this element (A linked list might have been better)
				  tcealloc alloctemp;
				  alloctemp.varname = s1;
				  alloctemp.size = atoi(s2.c_str());

				  j=0;
				  while(j < allocs.size() && allocs[j].varname.size() > s1.size())
					  j++;

				  if(j >= allocs.size()){
					  allocs.push_back(alloctemp);
				  }
				  else{
					     vector<tcealloc>::iterator it;
					  it = allocs.begin();
					  allocs.insert(it + j, alloctemp);
				  }
				  assemblerlines.pop_back();   //don't bother with this in the 2nd pass
				  continue;
			  } else {
				  MessageBoxA(0,"Wrong syntax. ALLOC(identifier,sizeinbytes)",0,0);
				  return false;
			  }
		  }






		///////////    KALLOC
          //{$ifndef net}
          ////memory kalloc
          //if uppercase(copy(currentline,1,7))='KALLOC(' then
          //{
          //  if not DBKReadWrite then raise exception.Create('You need to use kernelmode read/writeprocessmemory if you want to use KALLOC');

          //  if DarkByteKernel=0 then
          //    raise exception.Create('Sorry, but without the driver KALLOC will not function');

          //  //syntax: kalloc(x,size)    x=variable name size=bytes
          //  //kallocate memory
          //  a = pos('(',currentline);
          //  b = pos(',',currentline);
          //  c = pos(')',currentline);

          //  if (a>0) and (b>0) and (c>0) then
          //  {
          //    s1 = copy(currentline,a+1,b-a-1);
          //    s2 = copy(currentline,b+1,c-b-1);

          //    val('$'+s1,j,a);
          //    if a=0 then raise exception.Create(s1+' is not a valid identifier');

          //    varsize = length(s1);

          //    //check for duplicate identifiers
          //    j = 0;
          //    while (j<length(kallocs)) and (length(kallocs[j].varname)>varsize) do
          //    {
          //      if kallocs[j].varname=s1 then
          //        raise exception.Create('The identifier '+s1+' has already been declared');

          //      inc(j);
          //    };

          //    j = length(kallocs);//quickfix

          //    setlength(kallocs,length(kallocs)+1);

          //    //longest varnames first so the rename of a shorter matching var wont override the longer one
          //    //move up the other kallocs so I can inser this element (A linked list might have been better)
          //    for k = length(kallocs)-1 downto j+1 do
          //      kallocs[k] = kallocs[k-1];

          //    kallocs[j].varname = s1;
          //    kallocs[j].size = StrToInt(s2);

          //    setlength(assemblerlines,length(assemblerlines)-1);   //don't bother with this in the 2nd pass
          //    continue;
          //  } else raise exception.Create('Wrong syntax. kalloc(identifier,sizeinbytes)');
          //};

          //{$}if}

          ////replace KALLOC identifiers with values so the assemble error check doesnt crash on that
          //for j = 0 to length(kallocs)-1 do
          //  currentline = replacetoken(currentline,kallocs[j].varname,'00000000');









          //check for assembler errors
          //address
			
		// 检查地址行

			if (currentline[currentline.size()-1] == ':')
			{

				ok1 = false;
				for( j = 0; j < labels.size(); j++){
					string tempstring = labels[j].labelname;
					tempstring += ":";
					if (0 == currentline.compare(tempstring))  // 相等
					{
						labels[j].assemblerline = assemblerlines.size()-1;
						ok1 = true;
						continue;
					}
				}

				if (ok1)
					continue; //no check


				for( j = 0; j < allocs.size(); j++){
					string tempstring = allocs[j].varname;
					tempstring += ":";
					if (0 == currentline.compare(tempstring))  // 相等
					{
						ok1 = true;
						continue;
					}
				}

				if (ok1)
					continue; //no check


				//  getAddressFromName
				DWORD dwordtemp;
				if(_getAddressFromName(processhandle, currentline.substr(0,currentline.size()-1), &dwordtemp)){
					string addrtemp = int2HexString(dwordtemp) + ":";
					assemblerlines.pop_back();
					assemblerlines.push_back(addrtemp);
				}else{
					string strtemp = "The address specifier is not valid: " + currentline;
					MessageBoxA(0,strtemp.c_str(),0,0);
					return false;
				}

				continue; //next line
			}







			// 檢查代碼中是否有使用 module name 例 mov [java.exe+2000],p
			// 替换模块名称为地址
			bool yes = false;
			currentline = replaceModuleNameWithAddress(processhandle, currentline, &yes);
			if(yes){
				assemblerlines.pop_back();
				assemblerlines.push_back(currentline);
			}



			// replace call symbolname to call address
			// so call win api in game like "call messageboxA" is possible
			int pos = findWordMatch2(currentline, "call", true);
			if(pos != -1){
				x = 0;
				if(_getAddressFromName(processhandle, currentline.substr(pos+4), &x)){

					currentline = "call " + int2HexString(x);
					assemblerlines.pop_back();
					assemblerlines.push_back(currentline);
				}
			}



		  //replace ALLOC identifiers with values so the assemble error check doesnt crash on that
		  for(j = 0; j < allocs.size(); j++){
            currentline = replacetoken(currentline,allocs[j].varname,"00000000");
		  }


		  for(j = 0; j < globalallocs.size(); j++){
			  currentline = replacetoken(currentline,globalallocs[j].varname,"00000000");
		  }


          //replace label references with 00000000 so the assembler check doesn't complain about it
			for(j = 0; j < labels.size(); j++){
				currentline = replacetoken(currentline,labels[j].labelname,"00000000");
			}



			// 检查汇编代码是否可以通过编译器

            //replace identifiers in the line with their address
			if(!_assemble(currentaddress,currentline, assembled[0].bytes)){
				MessageBoxA(0,"This instruction can\'t be compiled",0,0);
				return false;
			}



	}
	
	
	////////////      end while loop












	// 检查registersymbol是否有效
    if (addsymbollist.size()>0)
    {
      //now scan the addsymbollist entries for allocs and labels and see if they exist
      for(i = 0; i < addsymbollist.size(); i++)
      {
        ok1 = false;
		for(j = 0; j < allocs.size(); j++){  //scan allocs
          if (true == _strCmpNoCase(addsymbollist[i], allocs[j].varname))
          {
            ok1 = true;
            break;
          }
		}

		if (!ok1){ //scan labels
			for(j = 0; j < labels.size(); j++){
				if (true == _strCmpNoCase(addsymbollist[i], labels[j].labelname))
				{
				  ok1 = true;
				  break;
				}
			}
		}

		if (!ok1){
			string strtemp = addsymbollist[i] + " was supposed to be added to the symbollist, but it isn\'t declared";
			MessageBoxA(0,strtemp.c_str(),0,0);
			return false;
		}
	  }
	}



	// 检查createthread是否有效
    //check to see if the addresses are valid (label, alloc, define)
	if(createthread.size()>0){
      for(i = 0; i < createthread.size(); i++)
      {
        //ok1 = true;


          ok1 = _getAddressFromName(processhandle, createthread[i], &testdword);

		if(!ok1){
			for(j = 0; j < labels.size(); j++){
				if(true == _strCmpNoCase(labels[j].labelname, createthread[i]))
				{
				  ok1 = true;
				  break;
				}
			}
		}

		if(!ok1){
			for(j = 0; j < allocs.size(); j++){
				if(true == _strCmpNoCase(allocs[j].varname, createthread[i]))
				{
				  ok1 = true;
				  break;
				}
			}
		}

        //{$ifndef net}
        //if not ok1 then
        //  for j = 0 to length(kallocs)-1 do
        //    if uppercase(kallocs[j].varname)=uppercase(createthread[i]) then
        //    {
        //      ok1 = true;
        //      break;
        //    };
        //{$}if}

		if(!ok1){
			for(j = 0; j < defines.size(); j++){
				if(true == _strCmpNoCase(defines[j].name, createthread[i]))
				{
					
					ok1 = _getAddressFromName(processhandle, defines[j].whatever, &testdword);
					break;
				}
			}
		}
	  
		if(!ok1){
			string strtemp = "The address in createthread(" + createthread[i] + ") is not valid";
			MessageBoxA(0,strtemp.c_str(),0,0);
			return false;
		}

	  }
	}



    //if length(loadbinary)>0 then
    //  for i = 0 to length(loadbinary)-1 do
    //  {
    //    ok1 = true;

    //    try
    //      testdword = symhandler.getAddressFromName(loadbinary[i].address);
    //    except
    //      ok1 = false;
    //    };

    //    if not ok1 then
    //      for j = 0 to length(labels)-1 do
    //        if uppercase(labels[j].labelname)=uppercase(loadbinary[i].address) then
    //        {
    //          ok1 = true;
    //          break;
    //        };

    //    if not ok1 then
    //      for j = 0 to length(allocs)-1 do
    //        if uppercase(allocs[j].varname)=uppercase(loadbinary[i].address) then
    //        {
    //          ok1 = true;
    //          break;
    //        };

    //    {$ifndef net}
    //    if not ok1 then
    //      for j = 0 to length(kallocs)-1 do
    //        if uppercase(kallocs[j].varname)=uppercase(loadbinary[i].address) then
    //        {
    //          ok1 = true;
    //          break;
    //        };
    //    {$}if}

    //    if not ok1 then
    //      for j = 0 to length(defines)-1 do
    //        if uppercase(defines[j].name)=uppercase(loadbinary[i].address) then
    //        {
    //          try
    //            testdword = symhandler.getAddressFromName(defines[j].whatever);
    //            ok1 = true;
    //          except
    //          };
    //          break;
    //        };

    //    if not ok1 then raise exception.Create('The address in loadbinary('+loadbinary[i].address+','+loadbinary[i].filename+') is not valid');

    //  };




	// 通过所有检查, 开始工作


	//分配内存空间
    //allocate the memory
    //first find out how much I should allocate
    if(allocs.size()>0)
    {
      x = 0;
	  for(i = 0; i < allocs.size(); i++){
       x += allocs[i].size;
	  }

      allocs[0].address = (DWORD)VirtualAllocEx(processhandle,NULL,x+4096,MEM_COMMIT,PAGE_EXECUTE_READWRITE);

	  for(i = 1; i < allocs.size(); i++){
        allocs[i].address = allocs[i-1].address + allocs[i-1].size;
	  }
	}

    //{$ifndef net}
    ////kernel alloc
    //if length(kallocs)>0 then
    //{
    //  x = 0;
    //  for i = 0 to length(kallocs)-1 do
    //   inc(x,kallocs[i].size);

    //  kallocs[0].address = dword(KernelAlloc(x));

    //  for i = 1 to length(kallocs)-1 do
    //    kallocs[i].address = kallocs[i-1].address+kallocs[i-1].size;
    //};
    //{$}if}














    //-----------------------2nd pass------------------------
    //assemblerlines only contains label specifiers and assembler instructions
    
	assembled.clear();
    for(i = 0; i < assemblerlines.size(); i++)
    {
      currentline = assemblerlines[i];

      
	  // ???
	  //tokenize(currentline,tokens);


	  
	  //if alloc then replace with the address
      for(j = 0; j < allocs.size(); j++)
        currentline = replacetoken(currentline,allocs[j].varname,int2HexString(allocs[j].address));

	  for(j = 0; j < globalallocs.size(); j++)
		  currentline = replacetoken(currentline,globalallocs[j].varname,int2HexString(globalallocs[j].address));

      ////if kalloc then replace with the address
      //for(j = 0; j < kallocs.size(); j++)
      //  currentline = replacetoken(currentline,kallocs[j].varname,IntToHex(kallocs[j].address,8));

      for(j = 0; j < defines.size(); j++)
        currentline = replacetoken(currentline,defines[j].name,defines[j].whatever);



      ok1 = false;

	  if( currentline[currentline.size()-1] != ':'){ //if it's not a definition then
		  for(j = 0; j < labels.size(); j++){
			  if (-1 != findWordMatch2(currentline, labels[j].labelname, true))
			  {
				  if (!labels[j].defined)
				  {
					  //the address hasn't been found yet
					  //this is the part that causes those nops after a short jump below the current instruction

					  //close
					  s1 = replacetoken(currentline,labels[j].labelname,int2HexString(currentaddress));

					  //far and big
					  //FFFFF, goes over the $FF boundary, and $FFFF boundary, and is a big value (address)
					  currentline = replacetoken(currentline,labels[j].labelname,int2HexString(currentaddress + 0xFFFFF));

					  tassembled assembledtemp;
					  assembledtemp.address = currentaddress;
					  assembled.push_back(assembledtemp);
					  _assemble(currentaddress, currentline, assembled[assembled.size()-1].bytes);
					  a = assembled[assembled.size()-1].bytes.size();

					  assembled[assembled.size()-1].bytes.clear();
					  _assemble(currentaddress, s1, assembled[assembled.size()-1].bytes);
					  b = assembled[assembled.size()-1].bytes.size();

					  if (a > b){ //pick the biggest one
						  _assemble(currentaddress, currentline, assembled[assembled.size()-1].bytes);
					  }

					  
					  labels[j].references.push_back(assembled.size()-1);		// refer to index of assembled

					  labels[j].references2.push_back(i);						// refer to index assemblerlines


					  currentaddress += assembled[assembled.size()-1].bytes.size();
					  ok1 = true;
				  } else{
					  currentline = replacetoken(currentline, labels[j].labelname, int2HexString(labels[j].address));
				  }

				  break;
			  }
		  }
	  }


	  if(ok1){
		  continue;
	  }


      if(currentline[currentline.size()-1] == ':')
	  {
        ok1 = false;
        for(j = 0; j < labels.size(); j++)
        {
          if(i == labels[j].assemblerline)		// label found
          {
            labels[j].address = currentaddress;
            labels[j].defined = true;
            ok1 = true;

            //fill in the undefined opcodes
			for(k = 0; k < labels[j].references.size(); k++)
            {
              a = assembled[labels[j].references[k]].bytes.size(); //original size of the assembled code
              s1 = replacetoken(assemblerlines[labels[j].references2[k]], labels[j].labelname, int2HexString(labels[j].address));
			  assembled[labels[j].references[k]].bytes.clear();
              _assemble(assembled[labels[j].references[k]].address, s1, assembled[labels[j].references[k]].bytes);

              b = assembled[labels[j].references[k]].bytes.size(); //new size

              //fill the difference with nops (not the most efficient approach, but it should work)
			  for(l = b; l < a; l++){
                assembled[labels[j].references[k]].bytes.push_back(0x90);
			  }
            }


            break;
		  }
		}
	  


		if(ok1){
		  continue;
		}



		if(true == _getAddressFromName(processhandle, currentline.substr(0,currentline.size()-1), &currentaddress)){
			continue; //next line
		}else{
			string strtemp = "This address specifier is not valid: " + currentline;
			MessageBoxA(0,strtemp.c_str(),0,0);
			return false;
		}

	  }


	  tassembled assembledtemp;
	  assembledtemp.address = currentaddress;
	  assembled.push_back(assembledtemp);
	  _assemble(currentaddress, currentline, assembled[assembled.size()-1].bytes);

      currentaddress += assembled[assembled.size()-1].bytes.size();
	}
    //} of loop 
	// end of the loop of assemblerlines

	
	// end of 2nd pass

	
	
	
	ok2 = true;

    //unprotectmemory
    for(i = 0; i < fullaccess.size(); i++)
    {

		VirtualProtectEx(processhandle, (LPVOID)fullaccess[i].address, fullaccess[i].size, PAGE_EXECUTE_READWRITE, &op);

      //if (fullaccess[i].address>80000000) and (DarkByteKernel<>0) then
      //  MakeWritable(fullaccess[i].address,(fullaccess[i].size div 4096)*4096,false);
    }

    ////load binaries
    //if length(loadbinary)>0 then
    //  for i = 0 to length(loadbinary)-1 do
    //  {
    //    ok1 = true;
    //    try
    //      testdword = symhandler.getAddressFromName(loadbinary[i].address);
    //    except
    //      ok1 = false;
    //    };

    //    if not ok1 then
    //      for j = 0 to length(labels)-1 do
    //        if uppercase(labels[j].labelname)=uppercase(loadbinary[i].address) then
    //        {
    //          ok1 = true;
    //          testdword = labels[j].address;
    //          break;
    //        };

    //    if not ok1 then
    //      for j = 0 to length(allocs)-1 do
    //        if uppercase(allocs[j].varname)=uppercase(loadbinary[i].address) then
    //        {
    //          ok1 = true;
    //          testdword = allocs[j].address;
    //          break;
    //        };

    //    if not ok1 then
    //      for j = 0 to length(kallocs)-1 do
    //        if uppercase(kallocs[j].varname)=uppercase(loadbinary[i].address) then
    //        {
    //          ok1 = true;
    //          testdword = kallocs[j].address;
    //          break;
    //        };

    //    if not ok1 then
    //      for j = 0 to length(defines)-1 do
    //        if uppercase(defines[j].name)=uppercase(loadbinary[i].address) then
    //        {
    //          try
    //            testdword = symhandler.getAddressFromName(defines[j].whatever);
    //            ok1 = true;
    //          except
    //          };

    //          break;
    //        };

    //    if ok1 then
    //    {
    //      binaryfile = tmemorystream.Create;
    //      try
    //        binaryfile.LoadFromFile(loadbinary[i].filename);
    //        ok2 = writeprocessmemory(processhandle,pointer(testdword),binaryfile.Memory,binaryfile.Size,bw);
    //      finally
    //        binaryfile.free;
    //      };
    //    };
    //  };




    //we're still here so, inject it
	for(i = 0; i < assembled.size(); i++)
	{
		VirtualProtectEx(processhandle, (LPVOID)assembled[i].address, assembled[i].bytes.size(), PAGE_EXECUTE_READWRITE, &op);


		BYTE *bytearray = NULL;
		bytearray = new BYTE[assembled[i].bytes.size()];
		for(j = 0; j < assembled[i].bytes.size(); j++){
			bytearray[j] = assembled[i].bytes[j];

		}

		ok1 = WriteProcessMemory(processhandle, (LPVOID)assembled[i].address, bytearray,assembled[i].bytes.size(), &op2);

		delete [] bytearray;
		bytearray = NULL;

		VirtualProtectEx(processhandle, (LPVOID)assembled[i].address, assembled[i].bytes.size(), op, &op2);

		if(!ok1){
			ok2 = false;
		}
	}

    if(!ok2)
    {
      MessageBoxA(0,"Not all instructions could be injected",0,0);
    }
    else
    {


		//see if all allocs are deallocated
		if(dealloc.size() == ceallocarray.size()) //free everything
		{
			baseaddress = 0xFFFFFFFF;

			for(i = 0; i < dealloc.size(); i++)
			{
				if(ceallocarray[i].address < baseaddress)
					baseaddress = dealloc[i];
			}
			VirtualFreeEx(processhandle, (LPVOID)baseaddress, 0, MEM_RELEASE);
		}

		ceallocarray.clear();
		for(i = 0; i < allocs.size(); i++){
			ceallocarray.push_back(allocs[i]);
		}




		////check the addsymbollist array and deletesymbollist array

		//first delete
		for(i = 0; i < deletesymbollist.size(); i++)
			DeleteUserdefinedSymbol(processhandle,deletesymbollist[i]);

		//now scan the addsymbollist array and add them to the userdefined list
		for(i = 0; i < addsymbollist.size(); i++)
		{

			ok1 = false;
			for(j = 0; j < allocs.size(); j++){
				if(_strCmpNoCase(addsymbollist[i], allocs[j].varname))
				{

					DeleteUserdefinedSymbol(processhandle, addsymbollist[i]); //delete old one so you can add the new one
					AddUserdefinedSymbol(processhandle, int2HexString(allocs[j].address), addsymbollist[i]);
					ok1 = true;

					//don't crash when it's already defined or address=0


					break;
				}
			}

			if(!ok1){
				for(j = 0; j < labels.size(); j++){
					if(_strCmpNoCase(addsymbollist[i], labels[j].labelname))
					{
						DeleteUserdefinedSymbol(processhandle, addsymbollist[i]); //delete old one so you can add the new one
						AddUserdefinedSymbol(processhandle, int2HexString(labels[j].address), addsymbollist[i]);
						ok1 = true;
						//don't crash when it's already defined or address=0


					}
				}
			}
		}





      //still here, so create threads if needed

      //load binaries
	if(createthread.size()>0){
        for(i = 0; i < createthread.size(); i++)
        {
          //ok1 = true;
		  ok1 = _getAddressFromName(processhandle, createthread[i], &testdword);

		  if(!ok1){
			for(j = 0; j < labels.size(); j++){
              if(_strCmpNoCase(labels[j].labelname, createthread[i]))
              {
                ok1 = true;
                testdword = labels[j].address;
                break;
              }
			}
		  }

		  if(!ok1){
			for(j = 0; j < allocs.size(); j++){
              if(_strCmpNoCase(allocs[j].varname ,createthread[i]))
              {
                ok1 = true;
                testdword = allocs[j].address;
                break;
              }
			}
		  }

		 // if(!ok1){
			//for(j = 0; j < kallocs.size(); j++){
   //           if(_strCmpNoCase(kallocs[j].varname, createthread[i]))
   //           {
   //             ok1 = true;
   //             testdword = kallocs[j].address;
   //             break;
   //           }
			//}
		 // }

			  if(!ok1){
				  for(j = 0; j < defines.size(); j++){
					  if(_strCmpNoCase(defines[j].name, createthread[i]))
					  {
						  ok1 = _getAddressFromName(processhandle, defines[j].whatever, &testdword);
						  if(ok1){
							  break;
						  }
					  }
				  }
			  }

			  if(ok1) //address found
			  {

				  //         binaryfile = tmemorystream.Create;
				  //try{
				  if(!CreateRemoteThread(processhandle, NULL, 0, (LPTHREAD_START_ROUTINE)testdword, NULL, 0, &bw)){
				  MessageBox(0,TEXT("CreateThread失敗"),0,0);
				  }
				  //}finally{
				  //           binaryfile.free;
				  //         }
			  }
		  


		}
	}
	}


// } finally{
//    for i = 0 to length(assembled)-1 do
//      setlength(assembled[i].bytes,0);
//
//    setlength(assembled,0);
//    tokens.free;
//
  


  return ok2;
}







// ENABLE: command = 1
// DISABLE: command = 0
bool _autoassemble(HANDLE hGame, const string& aa_script, int command){

	// find cheat id (decided by the script) or insert it
	int cheatId = -1;
	for(int i = 0; i < cheats.size(); i++){
		if(0 == aa_script.compare(cheats[i].script)){
			cheatId = i;
			break;
		}

	}

	if(cheatId < 0){			// not found, need to insert
		tcheats cheattemp;
		cheattemp.script = aa_script;
		cheats.push_back(cheattemp);
		cheatId = cheats.size()-1;
	}

	if(cheatId < 0){
		MessageBox(0,"PC_Asm: 無法增加腳本",0,0);
		return false;
	}


	// get string tokens
	vector<string> pretokens = splitLines((char *)aa_script.c_str());


	// remove comments
	removecomments(pretokens);


	// decide command
	//int command = atoi(parInf.pszValue);
	string s1;
	string s2;
	if(command == 1){			// ENABLE: command = 1
		s1 = "[enable]";
		s2 = "[disable]";
	}else if(command == 0){		// DISABLE: command = 0
		s1 = "[disable]";
		s2 = "[enable]";
	}else{
		//MessageBox(0,L"PC_Asm: 無效數值/取消值",0,0);
		return false;
	}


	// remove wrong section
	vector<string> tokens;
	bool inSection = true;	// pre-section is included
	for(int i = 0; i < pretokens.size(); i++){
		if(0 == findWordMatch2(pretokens[i], s1, true)){
			inSection = true;
		}else if (0 == findWordMatch2(pretokens[i], s2, true)){
			inSection = false;
		}else if(inSection && pretokens[i].size()>0){
			tokens.push_back(pretokens[i]);
		}
	}

	// 沒有代碼
	if(tokens.size()<=0){
		return false;
	}

	// 打印最終腳本
	//string finalscript = "";
	//for(int i=0;i<tokens.size();i++){
	//	finalscript += tokens[i] + "\n";	
	//}
	//print(finalscript);


	// 运行脚本内容
	if(hGame){
//		return _autoassemble2(hGame, tokens, cheats[cheatId].ceallocarray, registeredsymbols);
		return _autoassemble2(hGame, tokens, cheats[cheatId].ceallocarray);
	}else{
		return false;
	}

}


DWORD _getAddressFromAddrExpression(HANDLE hGame, string addr_expression){

			// get string tokens
			vector<string> pretokens = splitLines((char *)addr_expression.c_str());
			// remove comments
			removecomments(pretokens);
			string line = "";
			for(int i = 0; i < pretokens.size(); i++){
				if(pretokens[i].size() > 0){  // 空行一并忽略
					line = pretokens[i];
					break;
				}
			}

			if(hGame && line.size()>0){
				DWORD address = 0;
				if(_getAddressFromName(hGame, line, &address)){
					return address;
				}else{
					return 0;
				}
			}else{
				return false;
			}
}


