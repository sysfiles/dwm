#pragma once
#include <windows.h>
#include <TlHelp32.h>
#include "ldasm.h"
#include "vehHook.h"
#include <stdio.h>
#include <stdarg.h>

union Dr7 {
	ULONG64 all;
	struct {
		unsigned l0 : 1;         //!< [0] Local Breakpoint Enable 0
		unsigned g0 : 1;         //!< [1] Global Breakpoint Enable 0
		unsigned l1 : 1;         //!< [2] Local Breakpoint Enable 1
		unsigned g1 : 1;         //!< [3] Global Breakpoint Enable 1
		unsigned l2 : 1;         //!< [4] Local Breakpoint Enable 2
		unsigned g2 : 1;         //!< [5] Global Breakpoint Enable 2
		unsigned l3 : 1;         //!< [6] Local Breakpoint Enable 3
		unsigned g3 : 1;         //!< [7] Global Breakpoint Enable 3
		unsigned le : 1;         //!< [8] Local Exact Breakpoint Enable
		unsigned ge : 1;         //!< [9] Global Exact Breakpoint Enable
		unsigned reserved1 : 1;  //!< [10] Always 1
		unsigned rtm : 1;        //!< [11] Restricted Transactional Memory
		unsigned reserved2 : 1;  //!< [12] Always 0
		unsigned gd : 1;         //!< [13] General Detect Enable
		unsigned reserved3 : 2;  //!< [14:15] Always 0
		unsigned rw0 : 2;        //!< [16:17] Read / Write 0
		unsigned len0 : 2;       //!< [18:19] Length 0
		unsigned rw1 : 2;        //!< [20:21] Read / Write 1
		unsigned len1 : 2;       //!< [22:23] Length 1
		unsigned rw2 : 2;        //!< [24:25] Read / Write 2
		unsigned len2 : 2;       //!< [26:27] Length 2
		unsigned rw3 : 2;        //!< [28:29] Read / Write 3
		unsigned len3 : 2;       //!< [30:31] Length 3
	} fields;
};
static_assert(sizeof(Dr7) == sizeof(ULONG64), "Size check");

void OutputDebugPrintf(const char * strOutputString, ...);
ULONG WINAPI VEH_CreateHook(LPVOID pTarget, LPVOID pDetour,LPVOID * ppOriginal);

LONG vehProc(PEXCEPTION_POINTERS ExceptionInfo);
BOOL ListProcessThreadsAndSetHwBreakPoint(BOOL bRemove);
//要注意 如果当前线程 是主线，那么主线程 将无法设置。解决方法 启动一个线程 去执行
void SetHwBreakPoint(HANDLE hHandle, ULONG type, ULONG_PTR dwAddress_set_to_dr0, ULONG_PTR dwAddress_set_to_dr1, ULONG_PTR dwAddress_set_to_dr2, ULONG_PTR dwAddress_set_to_dr3);
//要注意 如果当前线程 是主线，那么主线程 将无法设置。解决方法 启动一个线程 去执行
VOID VEH_Initialize();
//要注意 如果当前线程 是主线，那么主线程 将无法设置。解决方法 启动一个线程 去执行
BOOL VEH_EnableHook();
VOID VEH_Uninitialize();
void UnSetHwBreakPoint(HANDLE hHandle);