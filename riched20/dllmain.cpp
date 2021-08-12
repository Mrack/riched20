// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "pch.h"


//////////////////////////////////////////////////////////////////////////////////////
// 导出函数
#pragma comment(linker, "/EXPORT:IID_IRichEditOle=_AheadLib_IID_IRichEditOle,@2")
#pragma comment(linker, "/EXPORT:IID_IRichEditOleCallback=_AheadLib_IID_IRichEditOleCallback,@3")
#pragma comment(linker, "/EXPORT:CreateTextServices=_AheadLib_CreateTextServices,@4")
#pragma comment(linker, "/EXPORT:IID_ITextServices=_AheadLib_IID_ITextServices,@5")
#pragma comment(linker, "/EXPORT:IID_ITextHost=_AheadLib_IID_ITextHost,@6")
#pragma comment(linker, "/EXPORT:IID_ITextHost2=_AheadLib_IID_ITextHost2,@7")
#pragma comment(linker, "/EXPORT:REExtendedRegisterClass=_AheadLib_REExtendedRegisterClass,@8")
#pragma comment(linker, "/EXPORT:RichEdit10ANSIWndProc=_AheadLib_RichEdit10ANSIWndProc,@9")
#pragma comment(linker, "/EXPORT:RichEditANSIWndProc=_AheadLib_RichEditANSIWndProc,@10")
//////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////
// 宏定义
#define EXTERNC extern "C"
#define NAKED __declspec(naked)
#define EXPORT __declspec(dllexport)

#define ALCPP EXPORT NAKED
#define ALSTD EXTERNC EXPORT NAKED void __stdcall
#define ALCFAST EXTERNC EXPORT NAKED void __fastcall
#define ALCDECL EXTERNC NAKED void __cdecl
//////////////////////////////////////////////////////////////////////////////////////


void writeMemory(LPVOID address, LPCVOID data, int size) {
	DWORD old;
	VirtualProtect(address, size, PAGE_EXECUTE_READWRITE, &old);
	memcpy(address, data, size);
	VirtualProtect(address, size, old, NULL);
	return;
}
//////////////////////////////////////////////////////////////////////////////////////
// AheadLib 命名空间
namespace AheadLib
{
	HMODULE m_hModule = NULL;	// 原始模块句柄


	// 加载原始模块
	inline BOOL WINAPI Load()
	{
		TCHAR tzPath[MAX_PATH];
		TCHAR tzTemp[MAX_PATH * 2];

		lstrcpy(tzPath, TEXT("C:\\Windows\\SysWOW64\\riched20.dll"));
		m_hModule = LoadLibrary(tzPath);
		if (m_hModule == NULL)
		{
			wsprintf(tzTemp, TEXT("无法加载 %s，程序无法正常运行。"), tzPath);
			MessageBox(NULL, tzTemp, TEXT("AheadLib"), MB_OK);
		}

		return (m_hModule != NULL);
	}

	// 释放原始模块
	inline VOID WINAPI Free()
	{
		if (m_hModule)
		{
			FreeLibrary(m_hModule);
			m_hModule = NULL;
		}
	}

	// 获取原始函数地址
	FARPROC WINAPI GetAddress(PCSTR pszProcName)
	{
		FARPROC fpAddress;
		CHAR szProcName[16];
		TCHAR tzTemp[MAX_PATH];

		fpAddress = GetProcAddress(m_hModule, pszProcName);

		if (fpAddress == NULL)
		{
			if (HIWORD(pszProcName) == 0)
			{
				wsprintfA(szProcName, "%d", pszProcName);
				pszProcName = szProcName;
			}

			wsprintf(tzTemp, TEXT("无法找到函数 %hs，程序无法正常运行。"), pszProcName);
			MessageBox(NULL, tzTemp, TEXT("AheadLib"), 0);
			ExitProcess(-2);
		}

		return fpAddress;
	}
}
using namespace AheadLib;
//////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////
// 导出函数
ALCDECL AheadLib_IID_IRichEditOle(void)
{
	// 调用原始函数
	GetAddress("IID_IRichEditOle");

	__asm JMP EAX;
}
//////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////
// 导出函数
ALCDECL AheadLib_IID_IRichEditOleCallback(void)
{
	// 调用原始函数
	GetAddress("IID_IRichEditOleCallback");

	__asm JMP EAX;
}
//////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////
// 导出函数
ALCDECL AheadLib_CreateTextServices(void)
{
	// 调用原始函数
	GetAddress("CreateTextServices");

	__asm JMP EAX;
}
//////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////
// 导出函数
ALCDECL AheadLib_IID_ITextServices(void)
{
	// 调用原始函数
	GetAddress("IID_ITextServices");

	__asm JMP EAX;
}
//////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////
// 导出函数
ALCDECL AheadLib_IID_ITextHost(void)
{
	// 调用原始函数
	GetAddress("IID_ITextHost");

	__asm JMP EAX;
}
//////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////
// 导出函数
ALCDECL AheadLib_IID_ITextHost2(void)
{
	// 调用原始函数
	GetAddress("IID_ITextHost2");

	__asm JMP EAX;
}
//////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////
// 导出函数
ALCDECL AheadLib_REExtendedRegisterClass(void)
{
	// 调用原始函数
	GetAddress("REExtendedRegisterClass");

	__asm JMP EAX;
}
//////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////
// 导出函数
ALCDECL AheadLib_RichEdit10ANSIWndProc(void)
{
	// 调用原始函数
	GetAddress("RichEdit10ANSIWndProc");

	__asm JMP EAX;
}
//////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////
// 导出函数
ALCDECL AheadLib_RichEditANSIWndProc(void)
{
	// 调用原始函数
	GetAddress("RichEditANSIWndProc");

	__asm JMP EAX;
}
//////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////
// 入口函数

BOOL APIENTRY DllMain(HMODULE hModule,
	DWORD  ul_reason_for_call,
	LPVOID lpReserved
)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		DisableThreadLibraryCalls(hModule);

		BOOL bRet;

		if (bRet = Load())
		{
			static Hook msgHook;
			msgHook.setHook("CreateWindowExA", "user32.dll", 4, Hook::POST_CALL, []() {

				//OEM
				unsigned char _4A4993[0x000A] =
				{
					0xb8, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90
				};
				char* text = (char*)malloc(128);
				strcpy(text, "Regsited to Mrack@qq.com");
				*(int*)(_4A4993 + 1) = (unsigned int)text;
				writeMemory((LPVOID)0x4A4993, _4A4993, 0x000A);

				const char* title = "GDAE3.95 Pro [Mrack@qq.com]\0";
				writeMemory((LPVOID)0x609728, title, strlen(title) + 1);

				//去除升级
				unsigned char _44CA70[1] =
				{
					0xc3
				};
				writeMemory((LPVOID)0x44CA70, _44CA70, 1);

				

				unsigned char _447DD0[3] = { 0x32, 0xc0, 0xc3 };
				writeMemory((LPVOID)0x447DD0, _447DD0, 0x3);

				unsigned char _453C00[0x0011] =
				{
					0xc7, 0x83, 0x1c, 0x0a, 0x00, 0x00, 0x01, 0x00,
					0x00, 0x00, 0xe9, 0x9c, 0x00, 0x00, 0x00, 0x90,
					0x90
				};
				writeMemory((LPVOID)0x453C00, _453C00, 17);

				unsigned char _46856A[3] = { 0x33, 0xc0, 0x40 };
				writeMemory((LPVOID)0x46856A, _46856A, 0x3);

				unsigned char _4685BD[5] = { 0x90, 0x90, 0x90, 0x90, 0x90 };
				writeMemory((LPVOID)0x4685BD, _4685BD, 0x5);

				unsigned char _4A498B[3] = { 0x83, 0xc2, 0x0C };
				writeMemory((LPVOID)0x46856A, _46856A, 0x3);


				msgHook.unsetHook();
				//MessageBox(NULL, L"补丁成功", L"Mrack", MB_OK);

				});
		}

		return bRet;

	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:

	case DLL_PROCESS_DETACH:
		Free();
		break;
	}
	return TRUE;
}
//////////////////////////////////////////////////////////////////////////////////////