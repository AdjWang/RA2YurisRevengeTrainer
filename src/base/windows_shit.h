#pragma once

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <winsock2.h>

#ifdef YRTR_DEBUG
#include <Dbghelp.h>
#include <tlhelp32.h>
#include <winternl.h>
#endif

#include <CommCtrl.h>
#include <timeapi.h>
