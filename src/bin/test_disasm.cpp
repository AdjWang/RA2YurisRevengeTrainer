#include "logging.h"
#include "win32utils.h"
#include "aa_engine/autoassembler.h"

std::string script = R"(
[ENABLE]
//code from here to '[DISABLE]' will be used to enable the cheat
alloc(newmem,2048)
label(returnhere)
label(originalcode)
label(exit)

newmem: //this is allocated memory, you have read,write,execute access
//place your code here
mov [ebx+000004AC],36
jmp exit

originalcode:
sub [ebx+000004AC],eax

exit:
jmp returnhere

"Tutorial-i386.exe"+2578F:
jmp newmem
nop
returnhere:


 
 
[DISABLE]
//code from here till the end of the code will be used to disable the cheat
dealloc(newmem)
"Tutorial-i386.exe"+2578F:
sub [ebx+000004AC],eax
//Alt: db 29 83 AC 04 00 00
)";

int main() {
    DWORD pid = 0;
    CHECK(yrtr::win32::GetProcessIDFromName("Tutorial-i386.exe", &pid));
    LOG(INFO, "pid={:#x}", pid);
    HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, false, pid);
    if (hProcess == NULL) {
        PLOG(WARN, "OpenProcess pid={} failed", pid);
    }
    yrtr::win32::HandleGuard handle(hProcess);
    bool retval = _autoassemble(handle.handle(), script, 0);
    CHECK(retval);

    return 0;
}
