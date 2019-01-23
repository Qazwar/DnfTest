#pragma  once
//#include "ntdll.h"
#pragma comment(lib,"ntdll.lib")

#ifdef _DEBUG
#pragma comment(lib,"Check_D.lib")
#else
#pragma comment(lib,"Check.lib")
#endif // _DEBUG

typedef LONG	NTSTATUS;

typedef struct _UNICODE_STRING {
	USHORT Length;
	USHORT MaximumLength;
#ifdef MIDL_PASS
	[size_is(MaximumLength / 2), length_is((Length) / 2) ] USHORT * Buffer;
#else // MIDL_PASS
	PWSTR  Buffer;
#endif // MIDL_PASS
} UNICODE_STRING;
typedef UNICODE_STRING *PUNICODE_STRING;
typedef const UNICODE_STRING *PCUNICODE_STRING;


typedef struct _OBJECT_ATTRIBUTES {
	ULONG Length;
	HANDLE RootDirectory;
	PUNICODE_STRING ObjectName;
	ULONG Attributes;
	PVOID SecurityDescriptor;        // Points to type SECURITY_DESCRIPTOR
	PVOID SecurityQualityOfService;  // Points to type SECURITY_QUALITY_OF_SERVICE
} OBJECT_ATTRIBUTES, *POBJECT_ATTRIBUTES;
typedef CONST OBJECT_ATTRIBUTES *PCOBJECT_ATTRIBUTES;

NTSYSAPI
	NTSTATUS
	NTAPI
	ZwQueryDirectoryObject(
	IN HANDLE DirectoryHandle,
	OUT PVOID Buffer,
	IN ULONG BufferLength,
	IN BOOLEAN ReturnSingleEntry,
	IN BOOLEAN RestartScan,
	IN OUT PULONG Context,
	OUT PULONG ReturnLength OPTIONAL
	);

NTSYSAPI
	NTSTATUS
	NTAPI
	ZwClose(
	IN HANDLE Handle
	);

NTSYSAPI
	NTSTATUS
	NTAPI
	ZwOpenDirectoryObject(
	OUT PHANDLE DirectoryHandle,
	IN ACCESS_MASK DesiredAccess,
	IN POBJECT_ATTRIBUTES ObjectAttributes
	);

extern "C" VOID NTAPI RtlInitUnicodeString (
	PUNICODE_STRING DestinationString,
	PCWSTR SourceString
	);



VOID Check();
VOID InjGameClient( char *strGameFile, char *strDllFile);


