#include "vehHook.h"
#define NEED_CODE_COUNT 20  //shellcode的空间

namespace VEH_DATA
{
	BOOL bRemove;
	ULONG index = 0;
	PVOID exceptHanle;
	ULONG_PTR orgFunc[4] = {0};    //原函数
	ULONG_PTR self_Func[4] = {0};   //自己的函数
	PVOID  pAllocateAddress[4] = { 0 };   //申请的 地址
};





BOOL ListProcessThreadsAndSetHwBreakPoint( BOOL bRemove)
{
	HANDLE hThreadSnap = INVALID_HANDLE_VALUE;
	THREADENTRY32 te32 = { 0 };
	HANDLE hThread = 0;
	// Take a snapshot of all running threads  
	hThreadSnap = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);
	if (hThreadSnap == INVALID_HANDLE_VALUE)
		return(FALSE);

	// Fill in the size of the structure before using it. 
	te32.dwSize = sizeof(THREADENTRY32);

	// Retrieve information about the first thread,
	// and exit if unsuccessful
	if (!Thread32First(hThreadSnap, &te32))
	{

		CloseHandle(hThreadSnap);     // Must clean up the snapshot object!
		return(FALSE);
	}

	// Now walk the thread list of the system,
	// and display information about each thread
	// associated with the specified process
	do
	{
		if (te32.th32OwnerProcessID == GetCurrentProcessId())
		{
			
			if (GetCurrentThreadId() != te32.th32ThreadID)
			{
				OutputDebugPrintf("hzw:SetHwBreakPoint by OpenThread failed %x %x", GetCurrentThreadId(), te32.th32ThreadID);
				hThread = 0;
				hThread = OpenThread(THREAD_ALL_ACCESS, FALSE, te32.th32ThreadID);
				if (hThread)
				{
					if (!bRemove) {
						SetHwBreakPoint(hThread, 0, VEH_DATA::orgFunc[0], VEH_DATA::orgFunc[1], VEH_DATA::orgFunc[2], VEH_DATA::orgFunc[3]);
					}
					else {
						UnSetHwBreakPoint(hThread);
					}

					CloseHandle(hThread);
				}
				else {
					OutputDebugPrintf("hzw:SetHwBreakPoint by OpenThread failed %x %d", GetCurrentThreadId(), GetLastError());
				}
			}


		}
	} while (Thread32Next(hThreadSnap, &te32));



	//  Don't forget to clean up the snapshot object.
	CloseHandle(hThreadSnap);
	return(TRUE);
}

VOID VEH_Initialize()
{
	VEH_DATA::exceptHanle = AddVectoredExceptionHandler(TRUE, (PVECTORED_EXCEPTION_HANDLER)vehProc);
}




//要注意 如果当前线程 是主线，那么主线程 将无法设置。解决方法 启动一个线程 去执行
BOOL VEH_EnableHook()
{
	
	return ListProcessThreadsAndSetHwBreakPoint(FALSE);;
}

VOID VEH_Uninitialize()
{


	if (VEH_DATA::exceptHanle)
	{
		
		ListProcessThreadsAndSetHwBreakPoint(TRUE);
		BOOLEAN bRet = FALSE;
		ULONG b = RemoveVectoredExceptionHandler(VEH_DATA::exceptHanle);
		
	
		//如果函数成功，则返回值非零。如果函数失败，则返回值为零。
		

		VEH_DATA::exceptHanle = 0;
		for (ULONG i = 0; i < VEH_DATA::index; i++)
		{
			if (VEH_DATA::pAllocateAddress[i])
			{
				bRet = VirtualFreeEx(GetCurrentProcess(), VEH_DATA::pAllocateAddress[i], NEED_CODE_COUNT * 5, MEM_DECOMMIT);
				VEH_DATA::pAllocateAddress[i] = 0;
				VEH_DATA::orgFunc[i] = 0;
				VEH_DATA::self_Func[i] = 0;
			}
		}

		VEH_DATA::index = 0;
	}

}



void SetHwBreakPoint(HANDLE hHandle, ULONG type, ULONG_PTR dwAddress_set_to_dr0, ULONG_PTR dwAddress_set_to_dr1, ULONG_PTR dwAddress_set_to_dr2, ULONG_PTR dwAddress_set_to_dr3)
{
	CONTEXT ctx;
	Dr7 dr7;
	RtlZeroMemory(&ctx, sizeof(CONTEXT));
	RtlZeroMemory(&dr7, sizeof(Dr7));
	ctx.ContextFlags = CONTEXT_ALL;

	if (SuspendThread(hHandle) == (DWORD)-1) {
		OutputDebugPrintf("hzw:SetHwBreakPoint by SuspendThread failed %x %d", GetCurrentThreadId(), GetLastError());
	}
	if (!GetThreadContext(hHandle, &ctx)) {
		OutputDebugPrintf("hzw:SetHwBreakPoint by GetThreadContext failed %x %d", GetCurrentThreadId(), GetLastError());
	}

	if (dwAddress_set_to_dr0) {
		dr7.fields.l0 = TRUE;
	}
	if (dwAddress_set_to_dr1) {
		dr7.fields.l1 = TRUE;
	}
	if (dwAddress_set_to_dr2) {
		dr7.fields.l2 = TRUE;
	}
	if (dwAddress_set_to_dr3) {
		dr7.fields.l3 = TRUE;
	}

	ULONG64 pTemoDr7 = 0;
	RtlCopyMemory(&pTemoDr7, &dr7, sizeof(ULONG64));
	ctx.Dr7 |= pTemoDr7;
	ctx.Dr0 = (ULONG_PTR)dwAddress_set_to_dr0;
	ctx.Dr1 = (ULONG_PTR)dwAddress_set_to_dr1;
	ctx.Dr2 = (ULONG_PTR)dwAddress_set_to_dr2;
	ctx.Dr3 = (ULONG_PTR)dwAddress_set_to_dr3;


	if (!SetThreadContext(hHandle, &ctx)) {
		OutputDebugPrintf("hzw:SetHwBreakPoint by SetThreadContext failed %x %d", GetCurrentThreadId(), GetLastError());
	}

	if (ResumeThread(hHandle) == (DWORD)-1) {
		OutputDebugPrintf("hzw:SetHwBreakPoint by ResumeThread failed %x %d", GetCurrentThreadId(), GetLastError());
	}

}

void UnSetHwBreakPoint(HANDLE hHandle)
{
	CONTEXT ctx;
	RtlZeroMemory(&ctx, sizeof(CONTEXT));
	ctx.ContextFlags = CONTEXT_ALL;

	if (SuspendThread(hHandle) == (DWORD)-1) {
		OutputDebugPrintf("hzw:SetHwBreakPoint by SuspendThread failed %x %d", GetCurrentThreadId(), GetLastError());
	}
	if (!GetThreadContext(hHandle, &ctx)) {
		OutputDebugPrintf("hzw:SetHwBreakPoint by GetThreadContext failed %x %d", GetCurrentThreadId(), GetLastError());
	}

	ctx.Dr7 = 0;
	ctx.Dr0 = 0;
	ctx.Dr1 = 0;
	ctx.Dr2 = 0;
	ctx.Dr3 = 0;

	if (!SetThreadContext(hHandle, &ctx)) {
		OutputDebugPrintf("hzw:SetHwBreakPoint by SetThreadContext failed %x %d", GetCurrentThreadId(), GetLastError());
	}

	if (ResumeThread(hHandle) == (DWORD)-1) {
		OutputDebugPrintf("hzw:SetHwBreakPoint by ResumeThread failed %x %d", GetCurrentThreadId(), GetLastError());
	}

}


void OutputDebugPrintf(const char * strOutputString, ...)
{
#define PUT_PUT_DEBUG_BUF_LEN   50
	char strBuffer[PUT_PUT_DEBUG_BUF_LEN] = { 0 };
	va_list vlArgs;
	va_start(vlArgs, strOutputString);
	_vsnprintf_s(strBuffer, sizeof(strBuffer) - 1, strOutputString, vlArgs);  //_vsnprintf_s  _vsnprintf
	//vsprintf(strBuffer,strOutputString,vlArgs);
	va_end(vlArgs);
	OutputDebugStringA(strBuffer);  //OutputDebugString    // OutputDebugStringW

}


#ifdef _WIN64
LONG vehProc(PEXCEPTION_POINTERS ExceptionInfo)
{

	if (ExceptionInfo->ExceptionRecord->ExceptionCode == EXCEPTION_SINGLE_STEP)
	{
		if (((ULONG_PTR)ExceptionInfo->ExceptionRecord->ExceptionAddress == VEH_DATA::orgFunc[0]) && VEH_DATA::orgFunc[0])
		{

			ExceptionInfo->ContextRecord->Rip = VEH_DATA::self_Func[0];
			return EXCEPTION_CONTINUE_EXECUTION;
		}

		if (((ULONG_PTR)ExceptionInfo->ExceptionRecord->ExceptionAddress == VEH_DATA::orgFunc[1]) && VEH_DATA::orgFunc[1])
		{

			ExceptionInfo->ContextRecord->Rip = VEH_DATA::self_Func[1];
			return EXCEPTION_CONTINUE_EXECUTION;
		}

		if (((ULONG_PTR)ExceptionInfo->ExceptionRecord->ExceptionAddress == VEH_DATA::orgFunc[2]) && VEH_DATA::orgFunc[2])
		{

			ExceptionInfo->ContextRecord->Rip = VEH_DATA::self_Func[2];;
			return EXCEPTION_CONTINUE_EXECUTION;
		}

		if (((ULONG_PTR)ExceptionInfo->ExceptionRecord->ExceptionAddress == VEH_DATA::orgFunc[3]) && VEH_DATA::orgFunc[3])
		{

			ExceptionInfo->ContextRecord->Rip = VEH_DATA::self_Func[3];
			return EXCEPTION_CONTINUE_EXECUTION;
		}

	}

	return EXCEPTION_CONTINUE_SEARCH;
}

ULONG WINAPI VEH_CreateHook(LPVOID pTarget, LPVOID pDetour, LPVOID * ppOriginal)
{
	if (VEH_DATA::index > 4)return -1;
	UCHAR shellcode[NEED_CODE_COUNT];

	RtlFillMemory(shellcode, NEED_CODE_COUNT, 0x90);


	PVOID pAllocate = NULL;

	pAllocate = VirtualAllocEx(GetCurrentProcess(), NULL, NEED_CODE_COUNT * 5, MEM_COMMIT, PAGE_EXECUTE_READWRITE);
	if (!pAllocate)return -2;
	RtlZeroMemory(pAllocate, NEED_CODE_COUNT * 5);


	ldasm_data ld;
	size_t len = 0;
	BOOL bOk = FALSE;
	ULONG_PTR pTargetJmpAddress = 0;
	RtlZeroMemory(&ld, sizeof(ldasm_data));
	len = ldasm(pTarget, &ld, TRUE);

	pTargetJmpAddress = (ULONG_PTR)pTarget + len;

	__try {
		//填充 shellcode  jmp dwordxxxxxxxxx
		RtlCopyMemory(shellcode, pTarget, len);
		shellcode[len] = 0xff;
		shellcode[len + 1] = 0x25;
		RtlFillMemory(&shellcode[len + 2], 4, 0);
		//构造跳转到我们函数的byte
		RtlCopyMemory(&shellcode[len + 2 + 4], &pTargetJmpAddress, sizeof(ULONG_PTR));

		RtlCopyMemory(pAllocate, shellcode, NEED_CODE_COUNT);

		bOk = TRUE;
	}
	__except (1)
	{
		VirtualFreeEx(GetCurrentProcess(), pAllocate, NEED_CODE_COUNT * 5, MEM_RELEASE);
		return -2;
	}

	__try {
		if (bOk)
		{
			*ppOriginal = pAllocate;
		}
	}
	__except (1)
	{
		VirtualFreeEx(GetCurrentProcess(), pAllocate, NEED_CODE_COUNT * 5, MEM_RELEASE);
		return -2;
	}

	if (VEH_DATA::index <= 4 && bOk)
	{
		VEH_DATA::orgFunc[VEH_DATA::index] = (ULONG_PTR)pTarget;
		VEH_DATA::self_Func[VEH_DATA::index] = (ULONG_PTR)pDetour;
		VEH_DATA::pAllocateAddress[VEH_DATA::index] = pAllocate;
		VEH_DATA::index++;
	}

	return 0;
}

#else

LONG vehProc(PEXCEPTION_POINTERS ExceptionInfo)
{

	if (ExceptionInfo->ExceptionRecord->ExceptionCode == EXCEPTION_SINGLE_STEP)
	{
		if (((ULONG_PTR)ExceptionInfo->ExceptionRecord->ExceptionAddress == VEH_DATA::orgFunc[0]) && VEH_DATA::orgFunc[0])
		{

			ExceptionInfo->ContextRecord->Eip = VEH_DATA::self_Func[0];


			return EXCEPTION_CONTINUE_EXECUTION;
		}

		if (((ULONG_PTR)ExceptionInfo->ExceptionRecord->ExceptionAddress == VEH_DATA::orgFunc[1]) && VEH_DATA::orgFunc[1])
		{

			ExceptionInfo->ContextRecord->Eip = VEH_DATA::self_Func[1];
			return EXCEPTION_CONTINUE_EXECUTION;
		}

		if (((ULONG_PTR)ExceptionInfo->ExceptionRecord->ExceptionAddress == VEH_DATA::orgFunc[2]) && VEH_DATA::orgFunc[2])
		{

			ExceptionInfo->ContextRecord->Eip = VEH_DATA::self_Func[2];;
			return EXCEPTION_CONTINUE_EXECUTION;
		}

		if (((ULONG_PTR)ExceptionInfo->ExceptionRecord->ExceptionAddress == VEH_DATA::orgFunc[3]) && VEH_DATA::orgFunc[3])
		{

			ExceptionInfo->ContextRecord->Eip = VEH_DATA::self_Func[3];
			return EXCEPTION_CONTINUE_EXECUTION;
		}

	}

	return EXCEPTION_CONTINUE_SEARCH;
}

ULONG WINAPI VEH_CreateHook(LPVOID pTarget, LPVOID pDetour, LPVOID * ppOriginal)
{
	if (VEH_DATA::index > 4)return -1;
	UCHAR shellcode[NEED_CODE_COUNT];

	RtlFillMemory(shellcode, NEED_CODE_COUNT, 0x90);


	PVOID pAllocate = NULL;

	pAllocate = VirtualAllocEx(GetCurrentProcess(), NULL, NEED_CODE_COUNT * 5, MEM_COMMIT, PAGE_EXECUTE_READWRITE);
	if (!pAllocate)return -2;
	RtlZeroMemory(pAllocate, NEED_CODE_COUNT * 5);


	ldasm_data ld;
	size_t len = 0;
	BOOL bOk = FALSE;
	ULONG_PTR pTargetJmpAddress = 0;
	RtlZeroMemory(&ld, sizeof(ldasm_data));
	len = ldasm(pTarget, &ld, FALSE);

	pTargetJmpAddress = (ULONG_PTR)pTarget + len;

	__try {
		//填充 shellcode  jmp dwordxxxxxxxxx
		RtlCopyMemory(shellcode, pTarget, len);
		shellcode[len] = 0xe9;

		ULONG_PTR offset = ((ULONG_PTR)pTarget + len) - ((ULONG_PTR)pAllocate + len + 5);

		//构造跳转到我们函数的
		memcpy(&shellcode[len + 1], &offset, sizeof(ULONG));

		RtlCopyMemory(pAllocate, shellcode, NEED_CODE_COUNT);

		bOk = TRUE;
	}
	__except (1)
	{
		VirtualFreeEx(GetCurrentProcess(), pAllocate, NEED_CODE_COUNT * 5, MEM_RELEASE);
		return -2;
	}

	__try {
		if (bOk)
		{
			*ppOriginal = pAllocate;
		}
	}
	__except (1)
	{
		VirtualFreeEx(GetCurrentProcess(), pAllocate, NEED_CODE_COUNT * 5, MEM_RELEASE);
		return -2;
	}

	if (VEH_DATA::index <= 4 && bOk)
	{
		VEH_DATA::orgFunc[VEH_DATA::index] = (ULONG_PTR)pTarget;
		VEH_DATA::self_Func[VEH_DATA::index] = (ULONG_PTR)pDetour;
		VEH_DATA::pAllocateAddress[VEH_DATA::index] = pAllocate;
		VEH_DATA::index++;
	}


	return 0;
}

#endif
