#pragma  once

DWORD GetFunc(char *szFunc);

#ifdef _DEBUG
   #pragma comment(lib,"GetFunc_D.lib")
#else
   #pragma comment(lib,"GetFunc.lib")
#endif // _DEBUG