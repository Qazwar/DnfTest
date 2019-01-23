#include "HookD3D.h"

LPVOID GetDirectDeviceMemberProc(LPVOID pDev, DWORD dwOffset)
{
	LPVOID pRet = NULL;
	__asm{
		mov eax, dword ptr [pDev] 
		mov ecx, dword ptr [eax] 
		mov ebx, dwOffset
			mov eax, dword ptr [ecx + ebx]
		mov pRet, eax
	}
	return pRet;
}


struct _d3dhookdev_type{
	LPVOID pD3D9;
	LPVOID pD3DDev;
};

bool InitD3D9(HWND hWnd, _d3dhookdev_type &dev)
{
	HMODULE hD3d9 = LoadLibraryA("d3d9.dll");
	LPVOID pD3D9 = NULL, pD3D9Dev = NULL;

	BYTE d3ddm[0x10];
	if(hD3d9)
	{
		LPVOID _Direct3DCreate9 = (LPVOID)GetProcAddress(hD3d9, "Direct3DCreate9");
		__asm{
			push 32
				call _Direct3DCreate9
				mov pD3D9, eax
		}

		if(!pD3D9)
			return false;

		LPVOID d3d9_Release = GetDirectDeviceMemberProc(pD3D9, 0x08);

		LPVOID _GetAdapterDisplayMode = GetDirectDeviceMemberProc(pD3D9, 0x20);

		HRESULT hr = NULL;
		__asm{
			lea eax, d3ddm
				push eax
				push 0
				push pD3D9
				call _GetAdapterDisplayMode
				mov hr, eax
		}

		if(FAILED(hr))
		{
			__asm{
				push pD3D9
					call d3d9_Release
			}
			return false;
		}

		BYTE d3dpp[0x38];
		memset(d3dpp, 0, 0x38);

		__asm{
			lea eax, d3dpp
				mov dword ptr [eax + 0x20], 1
				mov dword ptr [eax + 0x18], 1
				lea ebx, d3ddm
				mov ecx, dword ptr [ebx + 0x0C]
			mov dword ptr [eax + 8], ecx
		}


		LPVOID _CreateDevice = GetDirectDeviceMemberProc(pD3D9, 0x40);
		//0x40

		__asm{
			lea eax, pD3D9Dev
				push eax
				lea eax, d3dpp
				push eax
				push 0x20
				push hWnd
				push 1
				push 0
				push pD3D9
				call _CreateDevice
				mov hr, eax
		}

		//_DbgPrint("»æÖÆ¾ä±ú:%X\n", hr);

		if(FAILED(hr))
		{
			__asm{
				push pD3D9
				call d3d9_Release
			}

			return false;

		}

		dev.pD3D9 = pD3D9;
		dev.pD3DDev = pD3D9Dev;
		return true;
	}

	return false;
}

void ReleaseD3D9(_d3dhookdev_type &dev)
{
	if(dev.pD3D9)
	{
		LPVOID d3d_Release = GetDirectDeviceMemberProc(dev.pD3D9, 0x08);
		__asm{
			push dev.pD3D9
			call d3d_Release
		}
	}

	if(dev.pD3DDev)
	{
		LPVOID d3ddev_Release = GetDirectDeviceMemberProc(dev.pD3DDev, 0x08);
		__asm{
			push dev.pD3DDev
			call d3ddev_Release
		}
	}
}

void D3DPresentShellCodeHook(HWND hWnd, BYTE bTime)
{
	_d3dhookdev_type dev = {NULL, NULL};
	LPVOID pPresent = NULL;
	if(InitD3D9(hWnd, dev))
	{
		pPresent = GetDirectDeviceMemberProc(dev.pD3DDev, 0x44);
		ReleaseD3D9(dev);
		if(!pPresent)
			return;

		if(0xE9 == *(PBYTE)pPresent)
			return;
	}

	//_DbgPrint("Present:0x%X\n", pPresent);

	PBYTE psc = (PBYTE)VirtualAllocEx(GetCurrentProcess(), NULL, 4, MEM_COMMIT | MEM_RESERVE, 0x40);
	*(PDWORD)psc = (DWORD)Sleep;
	BYTE pCode[22] = {
		0x60,
		0x9C,
		0x6A, bTime,
		0xFF, 0x15, 0x00, 0x00, 0x00, 0x00,
		0x9D,
		0x61,
		0x8B, 0xFF,
		0x55,
		0x8B, 0xEC,
		0xE9, 0x00, 0x00, 0x00, 0x00, 
	};
	*((PDWORD)(pCode + 6)) = (DWORD)psc;
	PBYTE pShellCode = (PBYTE)VirtualAllocEx(GetCurrentProcess(), NULL, 22, MEM_COMMIT | MEM_RESERVE, 0x40);
	memcpy(pShellCode, pCode, 22);
	PDWORD pAddr = (PDWORD)(pShellCode + 18);
	*pAddr = (DWORD)pPresent - ((DWORD)pAddr - 1);
	DWORD dwProtect = 0;
	VirtualProtect(pPresent, 5, 0x40, &dwProtect);
	*(PBYTE)pPresent = 0xE9;
	*((PDWORD)((PBYTE)pPresent + 1)) = (DWORD)pShellCode - (DWORD)pPresent - 5;
	VirtualProtect(pPresent, 5, dwProtect, NULL);
}