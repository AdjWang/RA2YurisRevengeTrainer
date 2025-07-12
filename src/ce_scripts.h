#pragma once

namespace yrtr {
namespace script {

// Tip: player is the human player, others are AI

inline constexpr static std::string_view kUnlimitPower = R"(
[ENABLE]
//code from here to '[DISABLE]' will be used to enable the cheat
alloc(newmem,2048)
label(returnhere)
label(originalcode)
label(exit)
newmem: //this is allocated memory, you have read,write,execute access
//place your code here
push ecx
mov ecx,[00A83D4C]      // only work for player
//mov ecx,dword ptr [ecx]
cmp ecx,esi
pop ecx
jnz originalcode
jmp exit            // cheat
originalcode:
mov [esi+000053A8],edx
exit:
jmp returnhere
"{proc}"+108D18:
jmp newmem
nop
returnhere:
[DISABLE]
//code from here till the end of the code will be used to disable the cheat
dealloc(newmem)
"{proc}"+108D18:
mov [esi+000053A8],edx
//Alt: db 89 96 A8 53 00 00
)";


inline constexpr static std::string_view kBuildEveryWhereGround = R"(
[ENABLE]
//code from here to '[DISABLE]' will be used to enable the cheat
alloc(newmem,256)
label(returnhere)
label(originalcode)
label(exit)
newmem: //this is allocated memory, you have read,write,execute access
//place your code here
mov eax,01
jmp 004A9063
jmp exit
originalcode:
mov eax,[00A83D4C]
exit:
jmp returnhere
"{proc}"+A8EB0:
jmp newmem
returnhere:
[DISABLE]
//code from here till the end of the code will be used to disable the cheat
dealloc(newmem)
"{proc}"+A8EB0:
mov eax,[00A83D4C]
//Alt: db A1 4C 3D A8 00
)";

inline constexpr static std::string_view kBuildEveryWhereWater = R"(
[ENABLE]
//code from here to '[DISABLE]' will be used to enable the cheat
alloc(newmem,256)
label(returnhere)
label(originalcode)
label(exit)
newmem: //this is allocated memory, you have read,write,execute access
//place your code here
mov eax,[esp+24]
cmp eax,[00A83D4C]
jne originalcode
mov ecx,03
originalcode:
cmp ecx,-01
jne 0047CA4D
exit:
jmp returnhere
"{proc}"+7C9D1:
jmp newmem
returnhere:
[DISABLE]
//code from here till the end of the code will be used to disable the cheat
dealloc(newmem)
"{proc}"+7C9D1:
cmp ecx,-01
jne 0047CA4D
//Alt: db 83 F9 FF 75 77
)";


inline constexpr static std::string_view kAutoRepair = R"(
[ENABLE]
//code from here to '[DISABLE]' will be used to enable the cheat
alloc(newmem,2048)
label(returnhere)
label(originalcode)
label(exit)
newmem: //this is allocated memory, you have read,write,execute access
//place your code here
mov ecx,05
originalcode:
cmp ecx,eax
jl 00450813
exit:
jmp returnhere
"{proc}"+50651:
jmp newmem
nop
nop
nop
returnhere:
[DISABLE]
//code from here till the end of the code will be used to disable the cheat
dealloc(newmem)
"{proc}"+50651:
cmp ecx,eax
jl 00450813
//Alt: db 3B C8 0F 8C BA 01 00 00
)";

inline constexpr static std::string_view kAutoRepairNeutral = R"(
[ENABLE]
//code from here to '[DISABLE]' will be used to enable the cheat
alloc(newmem,256)
label(returnhere)
label(originalcode)
label(exit)
newmem: //this is allocated memory, you have read,write,execute access
//place your code here
mov ecx,[00A83D4C]
mov edx,[esi+21C]
cmp ecx,edx
jne originalcode    // only work for player
mov cl,1
originalcode:
test cl,cl
jne 00452652
xor al,al
exit:
jmp returnhere
"{proc}"+5264A:
jmp newmem
nop
returnhere:
[DISABLE]
//code from here till the end of the code will be used to disable the cheat
dealloc(newmem)
"{proc}"+5264A:
test cl,cl
jne 00452652
xor al,al
//Alt: db 84 C9 75 04 32 C0
)";


inline constexpr static std::string_view kRevertYuri = R"(
[ENABLE]
//code from here to '[DISABLE]' will be used to enable the cheat
alloc(newmem, 2048)
label(unitlist)
label(cheat)
label(returnhere)
label(originalcode)
label(exit)
newmem: //this is allocated memory, you have read,write,execute access
//place your code here
pushad
cmp eax,[00A83D4C]
je originalcode // skip player units
// the unit must be alive here
mov eax,0
mov ebx,[00A8EC7C]  // all units array
unitlist:
cmp eax,[00A8EC88]  // iter
jge originalcode    // >= disappeared or died
cmp ecx,[ebx+eax*4] // alive
je cheat
inc eax
jmp unitlist
cheat:
popad
mov eax,[00A83D4C]
push eax
//mov ecx,esi   // ecx -> who emits control
mov ebp,[ecx]   // unit type
call dword ptr [ebp+000003D4]
jmp 00471DC2    // no animation
originalcode:
popad
mov eax,[00A83D4C]
push eax
mov ecx,esi
call dword ptr [ebp+000003D4]
exit:
jmp returnhere
"{proc}"+71DB5:
jmp newmem
nop
nop
nop
nop
returnhere:
[DISABLE]
//code from here till the end of the code will be used to disable the cheat
dealloc(newmem)
"{proc}"+71DB5:
push eax
mov ecx,esi
call dword ptr[ebp+000003D4]
//Alt: db 50 8B CE FF 95 D4 03 00 00
)";

inline constexpr static std::string_view kPermanentYuri = R"(
[ENABLE]
//code from here to '[DISABLE]' will be used to enable the cheat
alloc(newmem,2048)
label(returnhere)
label(originalcode)
label(exit)
newmem: //this is allocated memory, you have read,write,execute access
//place your code here
mov eax,[00A83D4C]
originalcode:
push eax
mov ecx,esi
call dword ptr [edx+000003D4]
exit:
jmp returnhere
"{proc}"+720D7:
jmp newmem
nop
nop
nop
nop
returnhere:
[DISABLE]
//code from here till the end of the code will be used to disable the cheat
dealloc(newmem)
"{proc}"+720D7:
push eax
mov ecx,esi
call dword ptr [edx+000003D4]
//Alt: db 50 8B CE FF 92 D4 03 00 00
)";


inline constexpr static std::string_view kThisIsMineOnAttack = R"(
[ENABLE]
//code from here to '[DISABLE]' will be used to enable the cheat
alloc(newmem,2048)
label(unitlist)
label(cheat)
label(returnhere)
label(originalcode)
label(exit)
newmem: //this is allocated memory, you have read,write,execute access
//place your code here
pushad
mov eax,0
mov ebx,[esi+B0]    // unit address
mov ecx,[00A8EC7C]  // all untis array
unitlist:
cmp eax,[00A8EC88]  // iter
jge originalcode    // >= disappeared or died
cmp ebx,[ecx+eax*4] // alive
je cheat
inc eax
jmp unitlist
cheat:
push 0  //提示??
mov eax,[00A83D4C]
push eax
mov ecx,[esi+B0]
mov ebx,[ecx]
call [ebx+3D4]
jmp originalcode
originalcode:
popad
mov eax,[ebp+08]
mov edx,[eax]
exit:
jmp returnhere
"{proc}"+69AA4:
jmp newmem
returnhere:
[DISABLE]
//code from here till the end of the code will be used to disable the cheat
dealloc(newmem)
"{proc}"+69AA4:
mov eax,[ebp+08]
mov edx,[eax]
//Alt: db 8B 45 08 8B 10
)";


inline constexpr static std::string_view kThisIsMineOnGarrison = R"(
[ENABLE]
//code from here to '[DISABLE]' will be used to enable the cheat
alloc(newmem,2048)
label(returnhere)
label(originalcode)
label(exit)
newmem: //this is allocated memory, you have read,write,execute access
//place your code here
mov eax,[00A83D4C]
originalcode:
push eax
call dword ptr [edx+000003D4]
exit:
jmp returnhere
"{proc}"+58322:
jmp newmem
nop
nop
returnhere:
[DISABLE]
//code from here till the end of the code will be used to disable the cheat
dealloc(newmem)
"{proc}"+58322:
push eax
call dword ptr [edx+000003D4]
//Alt: db 50 FF 92 D4 03 00 00
)";


inline constexpr static std::string_view kThisIsMineOnCapture = R"(
[ENABLE]
//code from here to '[DISABLE]' will be used to enable the cheat
alloc(newmem,2048)
label(returnhere)
label(originalcode)
label(exit)
newmem: //this is allocated memory, you have read,write,execute access
//place your code here
mov eax,[00A83D4C]
originalcode:
push eax
mov ecx,edi
call dword ptr [edx+000003D4]
exit:
jmp returnhere
"{proc}"+119F7B:
jmp newmem
nop
nop
nop
nop
returnhere:
[DISABLE]
//code from here till the end of the code will be used to disable the cheat
dealloc(newmem)
"{proc}"+119F7B:
push eax
mov ecx,edi
call dword ptr [edx+000003D4]
//Alt: db 50 8B CF FF 92 D4 03 00 00
)";


inline constexpr static std::string_view kInstBuild = R"(
[ENABLE]
//code from here to '[DISABLE]' will be used to enable the cheat
alloc(newmem,2048)
label(returnhere)
label(cheat)
label(originalcode)
label(exit)
newmem: //this is allocated memory, you have read,write,execute access
//place your code here
push eax
push ebx
mov eax,[00A83D4C]      // only work for player
mov ebx,dword ptr [eax+53AC]    // aircraft
cmp ebx,esi
jz cheat
mov ebx,dword ptr [eax+53B0]    // infantry
cmp ebx,esi
jz cheat
mov ebx,dword ptr [eax+53B4]    // vehicle
cmp ebx,esi
jz cheat
mov ebx,dword ptr [eax+53B8]    // ship
cmp ebx,esi
jz cheat
mov ebx,dword ptr [eax+53BC]    // building
cmp ebx,esi
jz cheat
mov ebx,dword ptr [eax+53CC]    // weapon
cmp ebx,esi
jz cheat
jmp originalcode
cheat:
mov [esi+24],36     // aha
jmp exit
originalcode:
mov [esi+24],edx
exit:
pop ebx
pop eax
mov edx,[esp+10]
jmp returnhere
"{proc}"+C9B82:
jmp newmem
nop
nop
returnhere:
[DISABLE]
//code from here till the end of the code will be used to disable the cheat
dealloc(newmem)
"{proc}"+C9B82:
mov [esi+24],edx
mov edx,[esp+10]
//Alt: db 89 56 24 8B 54 24 10
)";


inline constexpr static std::string_view kGodPlayer = R"(
[ENABLE]
//code from here to '[DISABLE]' will be used to enable the cheat
alloc(newmem,2048)
label(returnhere)
label(originalcode)
label(exit)
newmem: //this is allocated memory, you have read,write,execute access
//place your code here
push ecx
mov ecx,[00A83D4C]      // only work for player
//mov ecx,dword ptr [ecx+0]
// check if attacking player
cmp ecx,[esi+21C]
pop ecx
jnz originalcode
sub eax,0   // is player
jmp exit
originalcode:
sub eax,edx
exit:
test eax,eax
mov [esi+6C],eax
jmp returnhere
"{proc}"+1F5509:
jmp newmem
nop
nop
returnhere:
[DISABLE]
//code from here till the end of the code will be used to disable the cheat
dealloc(newmem)
"{proc}"+1F5509:
sub eax,edx
test eax,eax
mov [esi+6C],eax
//Alt: db 2B C2 85 C0 89 46 6C
)";


inline constexpr static std::string_view kAntiChrono = R"(
[ENABLE]
//code from here to '[DISABLE]' will be used to enable the cheat
alloc(newmem,128)
label(returnhere)
label(originalcode)
label(exit)
newmem: //this is allocated memory, you have read,write,execute access
//place your code here
push eax
mov eax,[esi+24] // check if player attacked by chrono
mov eax,[eax+21C]
cmp eax,[00A83D4C]
pop eax
jne 71AB08
jmp originalcode
originalcode:
cmp eax,ebx
jg 71AB08
exit:
jmp returnhere
"{proc}"+31A88D:
jmp newmem
nop
nop
nop
returnhere:
[DISABLE]
//code from here till the end of the code will be used to disable the cheat
dealloc(newmem)
"{proc}"+31A88D:
cmp eax,ebx
jg 71AB08
//Alt: db 3B C3 0F 8F 73 02 00 00
)";

inline constexpr static std::string_view kAntiChronoDisbuild = R"(
[ENABLE]
//code from here to '[DISABLE]' will be used to enable the cheat
alloc(newmem,128)
label(returnhere)
label(originalcode)
label(exit)
newmem: //this is allocated memory, you have read,write,execute access
//place your code here
mov byte ptr [ecx+00000660],01
mov eax,[ecx+21C]
cmp eax,[00A83D4C]
je exit
originalcode:
mov byte ptr [ecx+00000660],00
exit:
jmp returnhere
"{proc}"+521C0:
jmp newmem
nop
nop
returnhere:
[DISABLE]
//code from here till the end of the code will be used to disable the cheat
dealloc(newmem)
"{proc}"+521C0:
mov byte ptr [ecx+00000660],00
//Alt: db C6 81 60 06 00 00 00
)";


inline constexpr static std::string_view kRevertRepair = R"(
[ENABLE]
//code from here to '[DISABLE]' will be used to enable the cheat
alloc(newmem,2048)
label(returnhere)
label(originalcode)
label(exit)
newmem: //this is allocated memory, you have read,write,execute access
//place your code here
push eax
mov eax,[00A83D4C]
cmp eax,[esi+21C]
pop eax
jz originalcode
sub ebx,ebp
cmp ebx,0
jge exit
mov ebx,0
jmp exit
originalcode:
add ebx,ebp
exit:
add edi,ebp
mov [esi+6C],ebx
jmp returnhere
"{proc}"+508B4:
jmp newmem
nop
nop
returnhere:
[DISABLE]
//code from here till the end of the code will be used to disable the cheat
dealloc(newmem)
"{proc}"+508B4:
add ebx,ebp
add edi,ebp
mov [esi+6C],ebx
//Alt: db 01 EB 01 EF 89 5E 6C
)";


inline constexpr static std::string_view kUnlimitTech = R"(
[ENABLE]
//code from here to '[DISABLE]' will be used to enable the cheat
alloc(newmem,64)
label(returnhere)
label(originalcode)
label(exit)
newmem: //this is allocated memory, you have read,write,execute access
//place your code here
jmp 004F7B96
originalcode:
mov ecx,[00A8B238]
exit:
jmp returnhere
"{proc}"+F7B6C:
jmp newmem
nop
returnhere:
[DISABLE]
//code from here till the end of the code will be used to disable the cheat
dealloc(newmem)
"{proc}"+F7B6C:
mov ecx,[00A8B238]
//Alt: db 8B 0D 38 B2 A8 00
)";


inline constexpr static std::string_view kInstFire = R"(
[ENABLE]
//code from here to '[DISABLE]' will be used to enable the cheat
alloc(newmem,256)
label(returnhere)
label(originalcode)
label(exit)
newmem: //this is allocated memory, you have read,write,execute access
//place your code here
mov eax,[esi+21C]
cmp eax,[00A83D4C]
jne originalcode
jmp 006FC981      // constant fire
originalcode:
mov eax,[esi+000002F4]
exit:
jmp returnhere
"{proc}"+2FC955:
jmp newmem
nop
returnhere:
[DISABLE]
//code from here till the end of the code will be used to disable the cheat
dealloc(newmem)
"{proc}"+2FC955:
mov eax,[esi+000002F4]
//Alt: db 8B 86 F4 02 00 00
)";


inline constexpr static std::string_view kInstTurnBattery = R"(
[ENABLE]
//code from here to '[DISABLE]' will be used to enable the cheat
alloc(newmem,256)
label(returnhere)
label(unitlist)
label(cheat)
label(originalcode)
label(exit)
newmem: //this is allocated memory, you have read,write,execute access
//place your code here
pushad
mov eax,0
mov ebx,[00A8EC7C]  // all units array
unitlist:
cmp eax,[00A8EC88]  // iter
jge originalcode    // >= disappeared or died
cmp ebp,[ebx+eax*4] // alive
je cheat
inc eax
jmp unitlist
cheat:
mov edx,[ebp+21C]
cmp edx,[00A83D4C]
jne exit
mov [ecx+08],0
originalcode:
exit:
popad
mov edx,[ecx+10]
mov ecx,[ecx+08]
jmp returnhere
"{proc}"+C941E:
jmp newmem
nop
returnhere:
[DISABLE]
//code from here till the end of the code will be used to disable the cheat
dealloc(newmem)
"{proc}"+C941E:
mov edx,[ecx+10]
mov ecx,[ecx+08]
//Alt: db 8B 51 10 8B 49 08
)";


inline constexpr static std::string_view kInstTurnRound = R"(
[ENABLE]
//code from here to '[DISABLE]' will be used to enable the cheat
alloc(newmem,256)
label(returnhere)
label(originalcode)
label(exit)
newmem: //this is allocated memory, you have read,write,execute access
//place your code here
lea esi,[ecx-388]
mov edi,[esi+21C]
cmp edi,[00A83D4C]
je 004C92EB
test bp,bp
originalcode:
jle 004C92EB
mov ax,dx
exit:
jmp returnhere
"{proc}"+C92BF:
jmp newmem
returnhere:
[DISABLE]
//code from here till the end of the code will be used to disable the cheat
dealloc(newmem)
"{proc}"+C92BF:
jle 004C92EB
mov ax,dx
//Alt: db 7E 2A 66 8B C2
)";


inline constexpr static std::string_view kFastReload = R"(
[ENABLE]
//code from here to '[DISABLE]' will be used to enable the cheat
alloc(newmem,256)
label(returnhere)
label(originalcode)
label(exit)
newmem: //this is allocated memory, you have read,write,execute access
//place your code here
mov ecx,[00A83D4C]
cmp ecx,[esi+21C]
jne originalcode
mov eax,0A
jmp exit
originalcode:
mov eax,[eax+00000D60]  // hack reload time
exit:
jmp returnhere
"{proc}"+2F3FC8:
jmp newmem
nop
returnhere:
[DISABLE]
//code from here till the end of the code will be used to disable the cheat
dealloc(newmem)
"{proc}"+2F3FC8:
mov eax,[eax+00000D60]
//Alt: db 8B 80 60 0D 00 00
)";


inline constexpr static std::string_view kUnlimitFirePower = R"(
[ENABLE]
//code from here to '[DISABLE]' will be used to enable the cheat
alloc(newmem,2048)
label(returnhere)
label(originalcode)
label(exit)
newmem: //this is allocated memory, you have read,write,execute access
//place your code here
mov ecx,[00A83D4C]
cmp ecx,[esi+21C]
jne originalcode
mov eax,0A
jmp exit
originalcode:
mov eax,[eax+00000D5C]  // ammunition
exit:
jmp returnhere
"{proc}"+2F3FD9:
jmp newmem
nop
returnhere:
[DISABLE]
//code from here till the end of the code will be used to disable the cheat
dealloc(newmem)
"{proc}"+2F3FD9:
mov eax,[eax+00000D5C]
//Alt: db 8B 80 5C 0D 00 00
)";


inline constexpr static std::string_view kRangeToYourBase = R"(
[ENABLE]
//code from here to '[DISABLE]' will be used to enable the cheat
alloc(newmem,256)
label(returnhere)
label(originalcode)
label(exit)
newmem: //this is allocated memory, you have read,write,execute access
//place your code here
push ecx
mov ecx,[00A83D4C]
cmp ecx,[esi+21C]
pop ecx
jne originalcode
mov edi,F900
jmp exit
originalcode:
mov edi,[ebx+000000B4]
exit:
jmp returnhere
"{proc}"+2F7248:
jmp newmem
nop
returnhere:
[DISABLE]
//code from here till the end of the code will be used to disable the cheat
dealloc(newmem)
"{proc}"+2F7248:
mov edi,[ebx+000000B4]
//Alt: db 8B BB B4 00 00 00
)";


inline constexpr static std::string_view kFireToYourBase = R"(
[ENABLE]
//code from here to '[DISABLE]' will be used to enable the cheat
alloc(newmem,256)
label(returnhere)
label(originalcode)
label(exit)
newmem: //this is allocated memory, you have read,write,execute access
//place your code here
mov ecx,[00A83D4C]
cmp ecx,[esi+21C]
jne originalcode
mov ebx,F900    // alert range F9 249
jmp exit
originalcode:
mov ebx,[eax+000000B4]
exit:
jmp returnhere
"{proc}"+3012DF:
jmp newmem
nop
returnhere:
[DISABLE]
//code from here till the end of the code will be used to disable the cheat
dealloc(newmem)
"{proc}"+3012DF:
mov ebx,[eax+000000B4]
//Alt: db 8B 98 B4 00 00 00
)";


inline constexpr static std::string_view kInstChronoMove = R"(
[ENABLE]
//code from here to '[DISABLE]' will be used to enable the cheat
alloc(newmem,128)
label(returnhere)
label(originalcode)
label(exit)
newmem: //this is allocated memory, you have read,write,execute access
//place your code here
mov edx,[ecx+21C]
cmp edx,[00A83D4C]
jne originalcode
mov byte ptr [ecx+00000271],00
jmp exit
originalcode:
mov byte ptr [ecx+00000271],01
exit:
jmp returnhere
"{proc}"+319579:
jmp newmem
nop
nop
returnhere:
[DISABLE]
//code from here till the end of the code will be used to disable the cheat
dealloc(newmem)
"{proc}"+319579:
mov byte ptr [ecx+00000271],01
//Alt: db C6 81 71 02 00 00 01
)";


inline constexpr static std::string_view kInstChronoAttack = R"(
[ENABLE]
//code from here to '[DISABLE]' will be used to enable the cheat
alloc(newmem,256)
label(returnhere)
label(originalcode)
label(exit)
newmem: //this is allocated memory, you have read,write,execute access
//place your code here
push eax
mov eax,[esi+24]    // only work for player
mov eax,[eax+21C]
cmp [00A83D4C],eax
pop eax
jne originalcode
mov [esi+48],0
jmp exit
originalcode:
mov [esi+48],eax
exit:
je 0071B064
jmp returnhere
"{proc}"+31AFB9:
jmp newmem
nop
nop
nop
nop
returnhere:
[DISABLE]
//code from here till the end of the code will be used to disable the cheat
dealloc(newmem)
"{proc}"+31AFB9:
mov [esi+48],eax
je 0071B064
//Alt: db 89 46 48 0F 84 A2 00 00 00
)";


inline constexpr static std::string_view kSpySpy = R"(
[ENABLE]
//code from here to '[DISABLE]' will be used to enable the cheat
alloc(newmem,256)
label(returnhere)
label(originalcode)
label(exit)
newmem: //this is allocated memory, you have read,write,execute access
//place your code here
cmp eax,[00A83D4C]
je originalcode
mov ebx,[edi+21C]   // who is the poor devil
mov [edi+21C],eax   // ohh, the spy works for player now
mov eax,[00A83D4C]
push eax
call 004571E0
mov [edi+21C],ebx   // give the spy back
jmp exit
originalcode:
push eax
call 004571E0
exit:
jmp returnhere
"{proc}"+11A00A:
jmp newmem
nop
returnhere:
[DISABLE]
//code from here till the end of the code will be used to disable the cheat
dealloc(newmem)
"{proc}"+11A00A:
push eax
call 004571E0
//Alt: db 50 E8 D0 D1 F3 FF
)";


inline constexpr static std::string_view kFreezeGAGAP = R"(
[ENABLE]
//code from here to '[DISABLE]' will be used to enable the cheat
alloc(newmem,256)
label(returnhere)
label(originalcode)
label(exit)
newmem: //this is allocated memory, you have read,write,execute access
//place your code here
mov eax,[esi+21C]
cmp eax,[00A83D4C]
je originalcode   // not work for player
mov eax,[esi]
cmp eax,007E3EBC
jne originalcode  // not a building
mov eax,[esi+520]
mov eax,[eax+DF8]
cmp eax,3F
jne originalcode  // not the gap generator
mov eax,1
jmp 006FAF1C
originalcode:
mov eax,[esi+00000504]
exit:
jmp returnhere
"{proc}"+2FAF0D:
jmp newmem
nop
returnhere:
[DISABLE]
//code from here till the end of the code will be used to disable the cheat
dealloc(newmem)
"{proc}"+2FAF0D:
mov eax,[esi+00000504]
//Alt: db 8B 86 04 05 00 00
)";


inline constexpr static std::string_view kFreezeUnit = R"(
[ENABLE]
//code from here to '[DISABLE]' will be used to enable the cheat
alloc(newmem,256)
label(returnhere)
label(originalcode)
label(exit)
newmem: //this is allocated memory, you have read,write,execute access
//place your code here
mov eax,[esi+21C]
cmp eax,[00A83D4C]
je originalcode
mov eax,1
jmp 006FAF1C
originalcode:
mov eax,[esi+504]
test eax,eax
jng 006FAFFD
exit:
jmp returnhere
"{proc}"+2FAF15:
jmp newmem
nop
returnhere:
[DISABLE]
//code from here till the end of the code will be used to disable the cheat
dealloc(newmem)
"{proc}"+2FAF15:
jng 006FAFFD
//Alt: db 0F 8E E2 00 00 00
)";


inline constexpr static std::string_view kSellTheWorldOnCursor = R"(
[ENABLE]
//code from here to '[DISABLE]' will be used to enable the cheat
alloc(newmem,256)
label(returnhere)
label(originalcode)
label(exit)
newmem: //this is allocated memory, you have read,write,execute access
//place your code here
//jmp 006929FC
//type judge
mov edx,[esi]
mov ecx,esi
call dword ptr [edx+2C]
cmp eax,24
je originalcode
//enable value is 0C disable is 0E
mov eax,0C
jmp 00692A20
originalcode:
mov eax,[esi]
mov ecx,esi
call dword ptr [eax+3C]
exit:
jmp returnhere
"{proc}"+2929D1:
jmp newmem
nop
nop
returnhere:
[DISABLE]
//code from here till the end of the code will be used to disable the cheat
dealloc(newmem)
"{proc}"+2929D1:
mov eax,[esi]
mov ecx,esi
call dword ptr [eax+3C]
//Alt: db 8B 06 8B CE FF 50 3C
)";


inline constexpr static std::string_view kInfantrySlip = R"(
[ENABLE]
//code from here to '[DISABLE]' will be used to enable the cheat
alloc(newmem,512)
label(returnhere)
label(originalcode)
label(exit)
newmem: //this is allocated memory, you have read,write,execute access
//place your code here
pushad
mov ecx,[esi+8]
mov ecx,[ecx+21C]
cmp ecx,[00A83D4C]
jne originalcode
mov ecx,eax
mov edx,[esi+24]
test edx,edx
je originalcode
mov edx,[esi+28]
test edx,edx
je originalcode
lea eax,[esi+24]
push eax
mov edx,[ecx]
call [edx+1B4]
originalcode:
popad
mov byte ptr [esi+31],00
mov eax,[esi]
exit:
jmp returnhere
"{proc}"+35AC93:
jmp newmem
nop
returnhere:
[DISABLE]
//code from here till the end of the code will be used to disable the cheat
dealloc(newmem)
"{proc}"+35AC93:
mov byte ptr [esi+31],00
mov eax,[esi]
//Alt: db C6 46 31 00 8B 06
)";


inline constexpr static std::string_view kUnitLeveledUp = R"(
[ENABLE]
//code from here to '[DISABLE]' will be used to enable the cheat
alloc(newmem,256)
label(returnhere)
label(originalcode)
label(exit)
newmem: //this is allocated memory, you have read,write,execute access
//place your code here
push eax
mov eax,[esi+0000021C]
cmp eax,[00A83D4C]
pop eax
jne originalcode
jmp exit
originalcode:
test ah,01
jne 00750025
exit:
jmp returnhere
"{proc}"+35001A:
jmp newmem
returnhere:
[DISABLE]
//code from here till the end of the code will be used to disable the cheat
dealloc(newmem)
"{proc}"+35001A:
test ah,01
jne 00750025
//Alt: db F6 C4 01 75 06
)";

}  // namespace script
}  // namespace yrtr
