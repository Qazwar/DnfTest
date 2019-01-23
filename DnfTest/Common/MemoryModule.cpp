//"MemoryModule.cpp"
/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
【】【】【】【】【】【】【】【】【】【】【】【】【】【】【】【】【】【】【】【】【】
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
** FileName : MemoryModule.cpp
** Version : 0.10
** Author : joachim 
** Date : 2011-03-22
** Comment : 加载模块，下载地址：http://www.joachim-bauch.de
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
【】【】【】【】【】【】【】【】【】【】【】【】【】【】【】【】【】【】【】【】【】
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
#include "stdafx.h"
#include <Windows.h>
#include <winnt.h>
#include "MemoryModule.h"

#ifndef __GNUC__
// disable warnings about pointer <-> DWORD conversions
#pragma warning( disable : 4311 4312 )
#endif

#ifdef _WIN64
#define POINTER_TYPE ULONGLONG
#else
#define POINTER_TYPE DWORD
#endif


#ifdef DEBUG_OUTPUT
#include <stdio.h>
#endif

#ifndef IMAGE_SIZEOF_BASE_RELOCATION
#define IMAGE_SIZEOF_BASE_RELOCATION (sizeof(IMAGE_BASE_RELOCATION))
#endif

typedef struct 
{
	PIMAGE_NT_HEADERS headers;
	unsigned char *codeBase;
	HMODULE *modules;
	int numModules;
	int initialized;
} MEMORYMODULE, *PMEMORYMODULE;

typedef BOOL (WINAPI *DllEntryProc)(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpReserved);

#define GET_HEADER_DICTIONARY(module, idx)	&(module)->headers->OptionalHeader.DataDirectory[idx]

static void CopySections(const unsigned char *data, PIMAGE_NT_HEADERS old_headers, PMEMORYMODULE module)
{
	int i, size;

	unsigned char *codeBase = module->codeBase;

	unsigned char *dest;

	PIMAGE_SECTION_HEADER section = IMAGE_FIRST_SECTION(module->headers);

	for (i=0; i<module->headers->FileHeader.NumberOfSections; i++, section++) 
	{
		if (section->SizeOfRawData == 0) 
		{
			size = old_headers->OptionalHeader.SectionAlignment;

			if (size > 0) 
			{
				dest = (unsigned char *)VirtualAlloc(codeBase + section->VirtualAddress,size,MEM_COMMIT,PAGE_READWRITE);

				section->Misc.PhysicalAddress = (POINTER_TYPE)dest;

				memset(dest, 0, size);
			}
			continue;
		}

		dest = (unsigned char *)VirtualAlloc(codeBase + section->VirtualAddress,section->SizeOfRawData,MEM_COMMIT,PAGE_READWRITE);

		memcpy(dest, data + section->PointerToRawData, section->SizeOfRawData);

		section->Misc.PhysicalAddress = (POINTER_TYPE)dest;
	}
}

static int ProtectionFlags[2][2][2] = {
	{
		{PAGE_NOACCESS, PAGE_WRITECOPY},
		{PAGE_READONLY, PAGE_READWRITE},
	}, 
	{
		{PAGE_EXECUTE, PAGE_EXECUTE_WRITECOPY},
		{PAGE_EXECUTE_READ, PAGE_EXECUTE_READWRITE},
		},
};

static void FinalizeSections(PMEMORYMODULE module)
{
	int i;

	PIMAGE_SECTION_HEADER section = IMAGE_FIRST_SECTION(module->headers);

#ifdef _WIN64
	POINTER_TYPE imageOffset = (module->headers->OptionalHeader.ImageBase & 0xffffffff00000000);
#else
#define imageOffset 0
#endif

	for (i=0; i<module->headers->FileHeader.NumberOfSections; i++, section++) 
	{
		DWORD protect, oldProtect, size;

		int executable = (section->Characteristics & IMAGE_SCN_MEM_EXECUTE) != 0;

		int readable = (section->Characteristics & IMAGE_SCN_MEM_READ) != 0;

		int writeable = (section->Characteristics & IMAGE_SCN_MEM_WRITE) != 0;

		if (section->Characteristics & IMAGE_SCN_MEM_DISCARDABLE) 
		{
			VirtualFree((LPVOID)((POINTER_TYPE)section->Misc.PhysicalAddress | imageOffset), section->SizeOfRawData, MEM_DECOMMIT);
			continue;
		}

		protect = ProtectionFlags[executable][readable][writeable];

		if (section->Characteristics & IMAGE_SCN_MEM_NOT_CACHED) 
		{
			protect |= PAGE_NOCACHE;
		}

		size = section->SizeOfRawData;

		if (size == 0) 
		{
			if (section->Characteristics & IMAGE_SCN_CNT_INITIALIZED_DATA) 
			{
				size = module->headers->OptionalHeader.SizeOfInitializedData;
			} else if (section->Characteristics & IMAGE_SCN_CNT_UNINITIALIZED_DATA) 
			{
				size = module->headers->OptionalHeader.SizeOfUninitializedData;
			}
		}
		if (size > 0) 
		{
			if (VirtualProtect((LPVOID)((POINTER_TYPE)section->Misc.PhysicalAddress | imageOffset), size, protect, &oldProtect) == 0)
			{
				OutputDebugString("Error protecting memory page");
			}
		}
	}
#ifndef _WIN64
#undef imageOffset
#endif
}

static void PerformBaseRelocation(PMEMORYMODULE module, SIZE_T delta)
{
	DWORD i;

	unsigned char *codeBase = module->codeBase;

	PIMAGE_DATA_DIRECTORY directory = GET_HEADER_DICTIONARY(module, IMAGE_DIRECTORY_ENTRY_BASERELOC);

	if (directory->Size > 0) 
	{
		PIMAGE_BASE_RELOCATION relocation = (PIMAGE_BASE_RELOCATION) (codeBase + directory->VirtualAddress);

		for (; relocation->VirtualAddress > 0; ) 
		{
			unsigned char *dest = codeBase + relocation->VirtualAddress;

			unsigned short *relInfo = (unsigned short *)((unsigned char *)relocation + IMAGE_SIZEOF_BASE_RELOCATION);

			for (i=0; i<((relocation->SizeOfBlock-IMAGE_SIZEOF_BASE_RELOCATION) / 2); i++, relInfo++) 
			{
				DWORD *patchAddrHL;
#ifdef _WIN64
				ULONGLONG *patchAddr64;
#endif
				int type, offset;

				type = *relInfo >> 12;

				offset = *relInfo & 0xfff;

				switch (type)
				{
				case IMAGE_REL_BASED_ABSOLUTE:
					break;
				case IMAGE_REL_BASED_HIGHLOW:
					// change complete 32 bit address
					patchAddrHL = (DWORD *) (dest + offset);
					*patchAddrHL += delta;
					break;
#ifdef _WIN64
				case IMAGE_REL_BASED_DIR64:
					patchAddr64 = (ULONGLONG *) (dest + offset);
					*patchAddr64 += delta;
					break;
#endif
				default:
					break;
				}
			}
			relocation = (PIMAGE_BASE_RELOCATION) (((char *) relocation) + relocation->SizeOfBlock);
		}
	}
}

static int BuildImportTable(PMEMORYMODULE module)
{
	int result = 1;

	unsigned char *codeBase = module->codeBase;

	PIMAGE_DATA_DIRECTORY directory = GET_HEADER_DICTIONARY(module, IMAGE_DIRECTORY_ENTRY_IMPORT);

	if (directory->Size > 0) 
	{
		PIMAGE_IMPORT_DESCRIPTOR importDesc = (PIMAGE_IMPORT_DESCRIPTOR) (codeBase + directory->VirtualAddress);

		for (; !IsBadReadPtr(importDesc, sizeof(IMAGE_IMPORT_DESCRIPTOR)) && importDesc->Name; importDesc++) 
		{
			POINTER_TYPE *thunkRef;

			FARPROC *funcRef;

			HMODULE handle = LoadLibraryA((LPCSTR) (codeBase + importDesc->Name));

			if (handle == INVALID_HANDLE_VALUE) 
			{
				OutputDebugString("Can't load library");
				result = 0;
				break;
			}

			module->modules = (HMODULE *)realloc(module->modules, (module->numModules+1)*(sizeof(HMODULE)));

			if (module->modules == NULL) 
			{
				result = 0;
				break;
			}

			module->modules[module->numModules++] = handle;

			if (importDesc->OriginalFirstThunk) 
			{
				thunkRef = (POINTER_TYPE *) (codeBase + importDesc->OriginalFirstThunk);
				funcRef = (FARPROC *) (codeBase + importDesc->FirstThunk);
			} 
			else 
			{
				thunkRef = (POINTER_TYPE *) (codeBase + importDesc->FirstThunk);
				funcRef = (FARPROC *) (codeBase + importDesc->FirstThunk);
			}
			for (; *thunkRef; thunkRef++, funcRef++) 
			{
				if (IMAGE_SNAP_BY_ORDINAL(*thunkRef)) 
				{
					*funcRef = (FARPROC)GetProcAddress(handle, (LPCSTR)IMAGE_ORDINAL(*thunkRef));
				} 
				else 
				{
					PIMAGE_IMPORT_BY_NAME thunkData = (PIMAGE_IMPORT_BY_NAME) (codeBase + (*thunkRef));
					*funcRef = (FARPROC)GetProcAddress(handle, (LPCSTR)&thunkData->Name);
				}
				if (*funcRef == 0) 
				{
					result = 0;
					break;
				}
			}
			if (!result) 
			{
				break;
			}
		}
	}
	return result;
}

//##############################################################################################################
//功能：加载模块
//##############################################################################################################
HMEMORYMODULE MemoryLoadLibrary(const void *data)
{
	PMEMORYMODULE result;

	PIMAGE_DOS_HEADER dos_header;

	PIMAGE_NT_HEADERS old_header;

	unsigned char *code, *headers;

	SIZE_T locationDelta;

	DllEntryProc DllEntry;

	BOOL successfull;

	dos_header = (PIMAGE_DOS_HEADER)data;

	if (dos_header->e_magic != IMAGE_DOS_SIGNATURE) 
	{
		OutputDebugString("Not a valid executable file.\n");
		return NULL;
	}

	old_header = (PIMAGE_NT_HEADERS)&((const unsigned char *)(data))[dos_header->e_lfanew];

	if (old_header->Signature != IMAGE_NT_SIGNATURE) 
	{
		OutputDebugString("No PE header found.\n");
		return NULL;
	}

	code = (unsigned char *)VirtualAlloc((LPVOID)(old_header->OptionalHeader.ImageBase),old_header->OptionalHeader.SizeOfImage,MEM_RESERVE,PAGE_READWRITE);

	if (code == NULL) 
	{
		code = (unsigned char *)VirtualAlloc(NULL,old_header->OptionalHeader.SizeOfImage,MEM_RESERVE,PAGE_READWRITE);

		if (code == NULL) 
		{
			OutputDebugString("Can't reserve memory");
			return NULL;
		}
	}

	result = (PMEMORYMODULE)HeapAlloc(GetProcessHeap(), 0, sizeof(MEMORYMODULE));

	result->codeBase = code;

	result->numModules = 0;

	result->modules = NULL;

	result->initialized = 0;

	VirtualAlloc(code,old_header->OptionalHeader.SizeOfImage,MEM_COMMIT,PAGE_READWRITE);

	headers = (unsigned char *)VirtualAlloc(code,old_header->OptionalHeader.SizeOfHeaders,MEM_COMMIT,PAGE_READWRITE);

	// 复制 PE文件头到代码
	memcpy(headers, dos_header, dos_header->e_lfanew + old_header->OptionalHeader.SizeOfHeaders);

	result->headers = (PIMAGE_NT_HEADERS)&((const unsigned char *)(headers))[dos_header->e_lfanew];

	// 更新位置
	result->headers->OptionalHeader.ImageBase = (POINTER_TYPE)code;

	// 复制DLL数据到新的内存
	CopySections((const unsigned char*)data, old_header, result);

	// 处理输入数据基址
	locationDelta = (SIZE_T)(code - old_header->OptionalHeader.ImageBase);

	if (locationDelta != 0) 
	{
		PerformBaseRelocation(result, locationDelta);
	}

	// 处理输入表
	if (!BuildImportTable(result)) 
	{
		goto error;
	}

	FinalizeSections(result);

	//调用DLL入口函数 DllMain
	if (result->headers->OptionalHeader.AddressOfEntryPoint != 0) 
	{
		DllEntry = (DllEntryProc) (code + result->headers->OptionalHeader.AddressOfEntryPoint);

		if (DllEntry == 0)
		{
			OutputDebugString("Library has no entry point.\n");
			goto error;
		}

		successfull = (*DllEntry)((HINSTANCE)code, DLL_PROCESS_ATTACH, 0); //调试发现加载MFC DLL直接停在这里，不向下运行了

		if (!successfull) 
		{
			OutputDebugString("Can't attach library.\n");
			goto error;
		}
		result->initialized = 1;
	}
	return (HMEMORYMODULE)result;

error:

	MemoryFreeLibrary(result);

	return NULL;
}
//##############################################################################################################
//功能：获取模块函数地址
//##############################################################################################################
FARPROC MemoryGetProcAddress(HMEMORYMODULE module, const char *name)
{
	unsigned char *codeBase = ((PMEMORYMODULE)module)->codeBase;

	int idx = -1;

	DWORD i,*nameRef;

	WORD *ordinal;

	PIMAGE_EXPORT_DIRECTORY exports;

	PIMAGE_DATA_DIRECTORY directory = GET_HEADER_DICTIONARY((PMEMORYMODULE)module, IMAGE_DIRECTORY_ENTRY_EXPORT);

	if (directory->Size == 0) 
	{
		return NULL;
	}

	exports = (PIMAGE_EXPORT_DIRECTORY) (codeBase + directory->VirtualAddress);

	if (exports->NumberOfNames == 0 || exports->NumberOfFunctions == 0) 
	{
		return NULL;
	}

	nameRef = (DWORD *) (codeBase + exports->AddressOfNames);

	ordinal = (WORD *) (codeBase + exports->AddressOfNameOrdinals);

	for (i=0; i<exports->NumberOfNames; i++, nameRef++, ordinal++)
	{
		if (stricmp(name, (const char *) (codeBase + (*nameRef))) == 0) 
		{
			idx = *ordinal;
			break;
		}
	}

	if (idx == -1) 
	{
		return NULL;
	}

	if ((DWORD)idx > exports->NumberOfFunctions) 
	{
		return NULL;
	}
	return (FARPROC) (codeBase + (*(DWORD *) (codeBase + exports->AddressOfFunctions + (idx*4))));
}
//##############################################################################################################
//功能：卸载模块
//##############################################################################################################
void MemoryFreeLibrary(HMEMORYMODULE mod)
{
	int i;
	PMEMORYMODULE module = (PMEMORYMODULE)mod;

	if (module != NULL) 
	{
		if (module->initialized != 0) 
		{
			DllEntryProc DllEntry = (DllEntryProc) (module->codeBase + module->headers->OptionalHeader.AddressOfEntryPoint);

			(*DllEntry)((HINSTANCE)module->codeBase, DLL_PROCESS_DETACH, 0);

			module->initialized = 0;
		}

		if (module->modules != NULL) 
		{
			for (i=0; i<module->numModules; i++) 
			{
				if (module->modules[i] != INVALID_HANDLE_VALUE) 
				{
					FreeLibrary(module->modules[i]);
				}
			}

			free(module->modules);
		}

		if (module->codeBase != NULL) 
		{
			VirtualFree(module->codeBase, 0, MEM_RELEASE);
		}
		HeapFree(GetProcessHeap(), 0, module);
	}
}