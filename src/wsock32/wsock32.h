#pragma once
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>

extern "C" {
__declspec(dllexport) int WSAAPI _getsockopt(int sockfd, int level, int optname,
                                             char* optval, socklen_t* optlen);
__declspec(dllexport) u_long WSAAPI _ntohl(u_long netlong);
__declspec(dllexport) u_short WSAAPI _htons(u_short hostshort);
__declspec(dllexport) int WSAAPI _recvfrom(SOCKET s, char* buf, int len,
                                           int flags, sockaddr* from,
                                           int* fromlen);
__declspec(dllexport) u_long WSAAPI _htonl(u_long hostlong);
__declspec(dllexport) int WSAAPI _setsockopt(SOCKET s, int level, int optname,
                                             const char* optval, int optlen);
__declspec(dllexport) int WSAAPI _sendto(SOCKET s, const char* buf, int len,
                                         int flags, const sockaddr* to,
                                         int tolen);
__declspec(dllexport) u_short WSAAPI _ntohs(u_short netshort);
__declspec(dllexport) int WSAAPI _gethostname(char* name, int namelen);
__declspec(dllexport) int WSAAPI _WSAStartup(WORD wVersionRequested,
                                             LPWSADATA lpWSAData);
__declspec(dllexport) int WSAAPI _EnumProtocolsA(LPINT lpiProtocols,
                                                 LPVOID lpProtocolBuffer,
                                                 LPDWORD lpdwBufferLength);
__declspec(dllexport) int WSAAPI _WSAAsyncSelect(SOCKET s, HWND hWnd,
                                                 u_int wMsg, long lEvent);
__declspec(dllexport) int WSAAPI _WSACleanup();
__declspec(dllexport) int WSAAPI _WSAGetLastError();
__declspec(dllexport) char* WSAAPI _inet_ntoa(in_addr in);
__declspec(dllexport) SOCKET WSAAPI _socket(int af, int type, int protocol);
__declspec(dllexport) int WSAAPI _bind(SOCKET s, const sockaddr* name,
                                       int namelen);
__declspec(dllexport) int WSAAPI _closesocket(SOCKET s);
__declspec(dllexport) hostent* WSAAPI _gethostbyname(const char* name);
}
