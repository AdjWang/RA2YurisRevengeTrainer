#include "wsock32/wsock32.h"

#include <Wsnwlink.h>
#include <intrin.h>
#include <nspapi.h>
#include <stdio.h>
#include <wsipx.h>

#include <string>
#include <system_error>

#include "MinHook.h"
#include "base/logging.h"

namespace {
inline std::string_view get_log_header() { return "[WSOCK32] "; }
}  // namespace

// #define WSOCK32_ENABLE_TRACE_CALLER

#ifdef WSOCK32_ENABLE_TRACE_CALLER
#define WSOCK32_CALLER_ADDR(arg1) (*(uint32_t*)((uint32_t)(&arg1) - 4))
// FUTURE: Use stack to resolve address.
#define WSOCK32_TRACE_CALLER_VOID() \
  HLOG_F(INFO, "caller=(todo) func={}", "Calling " __FUNCTION__)
#define WSOCK32_TRACE_CALLER(arg1)                                 \
  HLOG_F(INFO, "caller={:08X} func={}", WSOCK32_CALLER_ADDR(arg1), \
         "Calling " __FUNCTION__)
#else
#define WSOCK32_TRACE_CALLER_VOID()
#define WSOCK32_TRACE_CALLER(arg1)
#endif

int WSAAPI _getsockopt(SOCKET s, int level, int optname, char* optval,
                       int* optlen) {
  WSOCK32_TRACE_CALLER(s);
  return getsockopt(s, level, optname, optval, optlen);
}

u_long WSAAPI _ntohl(u_long netlong) {
  WSOCK32_TRACE_CALLER(netlong);
  return ntohl(netlong);
}

u_short WSAAPI _htons(u_short hostshort) {
  WSOCK32_TRACE_CALLER(hostshort);
  return htons(hostshort);
}

int WSAAPI _recvfrom(SOCKET s, char* buf, int len, int flags,
                     sockaddr* from_addr_arg, int* fromlen) {
  WSOCK32_TRACE_CALLER(s);
  bool isIPXAddr;
  INT Fromlen;
  DWORD recvedBytes;
  DWORD numberOfBytesRecvd;
  WSABUF wsa_buf;
  sockaddr_in from_addr_wsa;

  numberOfBytesRecvd = 0;
  wsa_buf.buf = buf;
  wsa_buf.len = len;
  isIPXAddr = *fromlen == 14;
  if (isIPXAddr) {
    Fromlen = sizeof(from_addr_wsa);
    // int WSAAPI WSARecvFrom(
    //   SOCKET                             s,
    //   LPWSABUF                           lpBuffers,
    //   DWORD                              dwBufferCount,
    //   LPDWORD                            lpNumberOfBytesRecvd,
    //   LPDWORD                            lpFlags,
    //   sockaddr                           *lpFrom,
    //   LPINT                              lpFromlen,
    //   LPWSAOVERLAPPED                    lpOverlapped,
    //   LPWSAOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine
    // );
    if (WSARecvFrom(s, &wsa_buf, 1, &numberOfBytesRecvd, (LPDWORD)&flags,
                    (sockaddr*)&from_addr_wsa, &Fromlen, 0, 0)) {
      recvedBytes = 0;
    } else {
      recvedBytes = numberOfBytesRecvd;
    }

    // Adjust UDP(possible) package to IPX package
    // typedef struct sockaddr {
    //   u_short sa_family;
    //   char    sa_data[14];
    // } SOCKADDR, *PSOCKADDR, *LPSOCKADDR;
    from_addr_arg->sa_family = AF_IPX;

    // sa_data  0   1   2   3   4   5   6   7   8   9   A   B   C   D
    // IPX:    [ IPX netnum  ] [     IPX nodenum     ]
    // IP:                     [ IP Address  ]         [port ]

    *(DWORD*)&from_addr_arg->sa_data = 0;  // index 0 1 2 3
    *(DWORD*)&from_addr_arg->sa_data[4] =
        from_addr_wsa.sin_addr.S_un.S_addr;  // ip address
    *(WORD*)&from_addr_arg->sa_data[8] = 0;  // index 8 9
    *(WORD*)&from_addr_arg->sa_data[10] =
        from_addr_wsa.sin_port;  // prot number
  } else if (WSARecvFrom(s, &wsa_buf, 1, &numberOfBytesRecvd, (LPDWORD)&flags,
                         from_addr_arg, fromlen, 0, 0)) {
    recvedBytes = 0;
  } else {
    recvedBytes = numberOfBytesRecvd;
  }
  return recvedBytes;
}

u_long WSAAPI _htonl(u_long hostlong) {
  WSOCK32_TRACE_CALLER(hostlong);
  return htonl(hostlong);
}

int WSAAPI _setsockopt(SOCKET s, int level, int optname, const char* optval,
                       int optlen) {
  WSOCK32_TRACE_CALLER(s);
  int result;
  if (level == NSPROTO_IPX) {
    result = 0;  // no errors
  } else {
    result = setsockopt(s, level, optname, optval, optlen);
  }
  return result;
}

int WSAAPI _sendto(SOCKET s, const char* buf, int len, int flags,
                   const sockaddr* to, int tolen) {
  WSOCK32_TRACE_CALLER(s);
  DWORD ip;
  USHORT port;
  int rc;
  WSABUF wsa_buf;
  sockaddr_in wraped_to;

  wsa_buf.buf = (CHAR*)buf;
  wsa_buf.len = len;
  // is IPX address
  if (to->sa_family == AF_IPX) {
    // sa_data  0   1   2   3   4   5   6   7   8   9   A   B   C   D
    // IPX:    [ IPX netnum  ] [     IPX nodenum     ]
    // IP:                     [ IP Address  ]         [port ]
    ip = *(DWORD*)&to->sa_data[4];
    port = *(WORD*)&to->sa_data[10];
    wraped_to.sin_family = AF_INET;
    wraped_to.sin_addr.S_un.S_addr = ip;
    wraped_to.sin_port = port;
    rc = WSASendTo(s, &wsa_buf, 1, (LPDWORD)&flags, flags,
                   (const sockaddr*)&wraped_to, sizeof(wraped_to), 0, 0);
  } else {
    rc = WSASendTo(s, &wsa_buf, 1, (LPDWORD)&flags, flags, to, tolen, 0, 0);
  }
  return rc == 0 ? flags : 0;
}

u_short WSAAPI _ntohs(u_short netshort) {
  WSOCK32_TRACE_CALLER(netshort);
  return ntohs(netshort);
}

int WSAAPI _gethostname(char* name, int namelen) {
  WSOCK32_TRACE_CALLER(name);
  return gethostname(name, namelen);
}

int WSAAPI _WSAStartup(WORD wVersionRequested, LPWSADATA lpWSAData) {
  WSOCK32_TRACE_CALLER(wVersionRequested);
  // Yuri requests version 1.1 where wVersionRequested is 0x0101
  int result;
  result = WSAStartup(MAKEWORD(2, 2), lpWSAData);
  if (!result) {
    lpWSAData->wVersion = wVersionRequested;
  }
  return result;
}

int WSAAPI _EnumProtocolsA(LPINT lpiProtocols, LPVOID lpProtocolBuffer,
                           LPDWORD lpdwBufferLength) {
  WSOCK32_TRACE_CALLER(lpiProtocols);
  return EnumProtocolsA(lpiProtocols, lpProtocolBuffer, lpdwBufferLength);
}

int WSAAPI _WSAAsyncSelect(SOCKET s, HWND hWnd, u_int wMsg, long lEvent) {
  WSOCK32_TRACE_CALLER(s);
  return WSAAsyncSelect(s, hWnd, wMsg, lEvent);
}

int WSAAPI _WSACleanup() {
  WSOCK32_TRACE_CALLER_VOID();
  int iResult = WSACleanup();
  return iResult;
}

int WSAAPI _WSAGetLastError() {
  WSOCK32_TRACE_CALLER_VOID();
  int iError = WSAGetLastError();
  return iError;
}

char* WSAAPI _inet_ntoa(in_addr in) {
  WSOCK32_TRACE_CALLER(in);
  return inet_ntoa(in);
}

SOCKET WSAAPI _socket(int af, int type, int protocol) {
  WSOCK32_TRACE_CALLER(af);
  int result;
  if (af == AF_IPX && type == SOCK_DGRAM && protocol == NSPROTO_IPX)
    result = WSASocketA(AF_INET, SOCK_DGRAM, IPPROTO_UDP, 0, 0, 0);
  else
    result = WSASocketA(af, type, protocol, 0, 0, 0);
  return result;
}

int WSAAPI _bind(SOCKET s, const sockaddr* name, int namelen) {
  WSOCK32_TRACE_CALLER(s);
  USHORT port;
  struct sockaddr_in addr;
  if (name->sa_family != AF_IPX) {
    return bind(s, name, namelen);
  }
  // TCP / UDP
  // sa_data  0   1   2   3   4   5   6   7   8   9   A   B   C   D
  // IPX:    [ IPX netnum  ] [     IPX nodenum     ]
  // IP:                     [ IP Address  ]         [port ]
  port = *(WORD*)&name->sa_data[10];
  addr.sin_family = AF_INET;
  addr.sin_addr.S_un.S_addr = 0;  // ip address
  addr.sin_port = port;           // port number
  return bind(s, (const sockaddr*)&addr, sizeof(addr));
}

int WSAAPI _closesocket(SOCKET s) {
  WSOCK32_TRACE_CALLER(s);
  int iResult = closesocket(s);
  return iResult;
}

hostent* WSAAPI _gethostbyname(const char* name) {
  WSOCK32_TRACE_CALLER(name);
  return gethostbyname(name);
}

static FARPROC(WINAPI* RealGetProcAddress)(HMODULE hModule,
                                           LPCSTR lpProcName) = GetProcAddress;

FARPROC WINAPI HookGetProcAddress(HMODULE hModule, LPCSTR lpProcName) {
  if (strcmp(lpProcName, "inet_addr") != 0) {
    return RealGetProcAddress(hModule, lpProcName);
  } else {
    return (FARPROC)inet_addr;
  }
}

#define HOOK_SKIP_2 0x00000001l
#define HOOK_LOCAL_ONLY 0x00000002l

typedef struct HOOKLISTDATA {
  char function_name[32];
  PROC new_function;
  PROC* function;
  DWORD flags;
  PROC org_function;
  HMODULE mod;
} HOOKLISTDATA;

typedef struct HOOKLIST {
  char module_name[32];
  HOOKLISTDATA data[33];
} HOOKLIST;

void hook_patch_obfuscated_iat_list(HMODULE hmod, BOOL unhook, HOOKLIST* hooks,
                                    BOOL is_local) {
  if (!hmod || hmod == INVALID_HANDLE_VALUE || !hooks) return;

  __try {
    PIMAGE_DOS_HEADER dos_header = (PIMAGE_DOS_HEADER)hmod;
    if (dos_header->e_magic != IMAGE_DOS_SIGNATURE) return;

    PIMAGE_NT_HEADERS nt_headers =
        (PIMAGE_NT_HEADERS)((DWORD)dos_header + (DWORD)dos_header->e_lfanew);
    if (nt_headers->Signature != IMAGE_NT_SIGNATURE) return;

    DWORD import_desc_rva =
        nt_headers->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT]
            .VirtualAddress;
    DWORD import_desc_size =
        nt_headers->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT]
            .Size;

    if (import_desc_rva == 0 || import_desc_size == 0) return;

    PIMAGE_IMPORT_DESCRIPTOR import_desc =
        (PIMAGE_IMPORT_DESCRIPTOR)((DWORD)dos_header + import_desc_rva);

    while (import_desc->FirstThunk) {
      if (!import_desc->Name) {
        import_desc++;
        continue;
      }

      for (int i = 0; hooks[i].module_name[0]; i++) {
        char* imp_module_name = (char*)((DWORD)dos_header + import_desc->Name);

        if (_stricmp(imp_module_name, hooks[i].module_name) == 0) {
          HMODULE cur_mod = GetModuleHandleA(hooks[i].module_name);

          PIMAGE_THUNK_DATA first_thunk =
              (PIMAGE_THUNK_DATA)((DWORD)dos_header + import_desc->FirstThunk);

          while (first_thunk->u1.Function) {
            for (int x = 0; hooks[i].data[x].function_name[0]; x++) {
              /* GetProcAddress is slow, save the pointer and reuse it for
               * better performance */
              DWORD org_function = (DWORD)InterlockedExchangeAdd(
                  (LONG*)&hooks[i].data[x].org_function, 0);

              if (!org_function || cur_mod != hooks[i].data[x].mod) {
                hooks[i].data[x].mod = cur_mod;

                org_function = (DWORD)RealGetProcAddress(
                    cur_mod, hooks[i].data[x].function_name);

                InterlockedExchange((LONG*)&hooks[i].data[x].org_function,
                                    (LONG)org_function);
              }

              if (!hooks[i].data[x].new_function || !org_function) continue;

              if (!is_local && (hooks[i].data[x].flags & HOOK_LOCAL_ONLY))
                continue;

              if (unhook) {
                if (first_thunk->u1.Function ==
                    (DWORD)hooks[i].data[x].new_function) {
                  DWORD op;

                  if (VirtualProtect(&first_thunk->u1.Function, sizeof(DWORD),
                                     PAGE_READWRITE, &op)) {
                    first_thunk->u1.Function = org_function;

                    VirtualProtect(&first_thunk->u1.Function, sizeof(DWORD), op,
                                   &op);
                  }

                  break;
                }
              } else {
                if (first_thunk->u1.Function == org_function) {
                  DWORD op;

                  if (VirtualProtect(&first_thunk->u1.Function, sizeof(DWORD),
                                     PAGE_READWRITE, &op)) {
                    first_thunk->u1.Function =
                        (DWORD)hooks[i].data[x].new_function;

                    VirtualProtect(&first_thunk->u1.Function, sizeof(DWORD), op,
                                   &op);
                  }

                  break;
                }
              }
            }

            first_thunk++;
          }

          break;
        }
      }

      import_desc++;
    }
  } __except (EXCEPTION_EXECUTE_HANDLER) {
  }
}

void hook_patch_iat_list(HMODULE hmod, BOOL unhook, HOOKLIST* hooks,
                         BOOL is_local) {
  hook_patch_obfuscated_iat_list(hmod, unhook, hooks, is_local);

  if (!hmod || hmod == INVALID_HANDLE_VALUE || !hooks) return;

  __try {
    PIMAGE_DOS_HEADER dos_header = (PIMAGE_DOS_HEADER)hmod;
    if (dos_header->e_magic != IMAGE_DOS_SIGNATURE) return;

    PIMAGE_NT_HEADERS nt_headers =
        (PIMAGE_NT_HEADERS)((DWORD)dos_header + (DWORD)dos_header->e_lfanew);
    if (nt_headers->Signature != IMAGE_NT_SIGNATURE) return;

    DWORD import_desc_rva =
        nt_headers->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT]
            .VirtualAddress;
    DWORD import_desc_size =
        nt_headers->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT]
            .Size;

    if (import_desc_rva == 0 || import_desc_size == 0) return;

    PIMAGE_IMPORT_DESCRIPTOR import_desc =
        (PIMAGE_IMPORT_DESCRIPTOR)((DWORD)dos_header + import_desc_rva);

    while (import_desc->FirstThunk) {
      if (!import_desc->OriginalFirstThunk || !import_desc->Name) {
        import_desc++;
        continue;
      }

      for (int i = 0; hooks[i].module_name[0]; i++) {
        char* imp_module_name = (char*)((DWORD)dos_header + import_desc->Name);

        if (_stricmp(imp_module_name, hooks[i].module_name) == 0) {
          PIMAGE_THUNK_DATA first_thunk =
              (PIMAGE_THUNK_DATA)((DWORD)dos_header + import_desc->FirstThunk);
          PIMAGE_THUNK_DATA o_first_thunk =
              (PIMAGE_THUNK_DATA)((DWORD)dos_header +
                                  import_desc->OriginalFirstThunk);

          while (first_thunk->u1.Function) {
            if (!o_first_thunk->u1.AddressOfData) {
              first_thunk++;
              o_first_thunk++;
              continue;
            }

            PIMAGE_IMPORT_BY_NAME import =
                (PIMAGE_IMPORT_BY_NAME)((DWORD)dos_header +
                                        o_first_thunk->u1.AddressOfData);

            if ((o_first_thunk->u1.Ordinal & IMAGE_ORDINAL_FLAG) == 0) {
              for (int x = 0; hooks[i].data[x].function_name[0]; x++) {
                if (!unhook && !hooks[i].data[x].new_function) continue;

                if (!is_local && (hooks[i].data[x].flags & HOOK_LOCAL_ONLY))
                  continue;

#if defined(__GNUC__)
                if (util_is_bad_read_ptr((void*)import->Name)) continue;
#endif

                if (strcmp((const char*)import->Name,
                           hooks[i].data[x].function_name) == 0) {
                  DWORD op;

                  if (VirtualProtect(&first_thunk->u1.Function, sizeof(DWORD),
                                     PAGE_READWRITE, &op)) {
                    if (unhook) {
                      DWORD org = (DWORD)RealGetProcAddress(
                          GetModuleHandleA(hooks[i].module_name),
                          hooks[i].data[x].function_name);

                      if (org && first_thunk->u1.Function != org) {
                        first_thunk->u1.Function = org;
                      }
                    } else {
                      if (first_thunk->u1.Function !=
                          (DWORD)hooks[i].data[x].new_function)
                        first_thunk->u1.Function =
                            (DWORD)hooks[i].data[x].new_function;
                    }

                    VirtualProtect(&first_thunk->u1.Function, sizeof(DWORD), op,
                                   &op);
                  }

                  break;
                }
              }
            }

            first_thunk++;
            o_first_thunk++;
          }

          break;
        }
      }

      import_desc++;
    }
  } __except (EXCEPTION_EXECUTE_HANDLER) {
  }
}

void hook_patch_iat(HMODULE hmod, BOOL unhook, const char* module_name,
                    const char* function_name, PROC new_function) {
  HOOKLIST hooks[2];
  memset(&hooks, 0, sizeof(hooks));

  hooks[0].data[0].new_function = new_function;

  strncpy_s(hooks[0].module_name, module_name,
            sizeof(hooks[0].module_name) - 1);
  strncpy_s(hooks[0].data[0].function_name, function_name,
            sizeof(hooks[0].data[0].function_name) - 1);

  hook_patch_iat_list(hmod, unhook, (HOOKLIST*)&hooks, TRUE);
}

BOOL WINAPI DllMain(HINSTANCE /*hinstDLL*/, DWORD fdwReason,
                    LPVOID lpvReserved) {
  switch (fdwReason) {
    case DLL_PROCESS_ATTACH: {
      // Setup win32 api hooks.
      hook_patch_iat(GetModuleHandle(NULL), FALSE, "kernel32.dll",
                     "GetProcAddress", (PROC)HookGetProcAddress);
      yrtr::logging::InitLogging(yrtr::logging::LogSink::kDbgView);
      // TODO: add hotreload for debugging
      // Try to load scarlet.
      HMODULE scarlet = LoadLibraryA("scarlet.dll");
      if (scarlet == NULL) {
        DWORD err = GetLastError();
        std::string message = std::system_category().message(err);
        LOG_F(ERROR, "Failed to load scarlet, err=[{}]{}", err, message);
      }
      break;
    }
    case DLL_THREAD_ATTACH:
      break;
    case DLL_THREAD_DETACH:
      break;
    case DLL_PROCESS_DETACH:
      if (lpvReserved != nullptr) {
        break;
      }
      break;
  }
  return TRUE;
}
