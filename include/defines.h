#ifndef _CHIP8_DEFINES_H_
#define _CHIP8_DEFINES_H_

#if defined( _WIN32s )

#include <wtypes.h>

#else

//
// conforms to windows type defines as much as possible for consistency
//

#include <cstdint>
#include <cstddef>
#include <cstdlib>
#include <cstring>

typedef char CHAR;
typedef char CHAR8;
typedef signed char INT8;
typedef signed char LONG8;

typedef wchar_t WCHAR;

typedef char* PCHAR;
typedef char* PCHAR8;
typedef signed char* PINT8;
typedef signed char* PLONG8;

typedef wchar_t* PWCHAR;

typedef const char* LPCSTR;
typedef const wchar_t* LPCWSTR;

typedef unsigned char UCHAR;
typedef unsigned char UCHAR8;
typedef unsigned char BYTE;
typedef unsigned char UINT8;
typedef unsigned char ULONG8;

typedef BYTE BOOLEAN;
typedef BOOLEAN* PBOOLEAN;

typedef unsigned char* PUCHAR;
typedef unsigned char* PUCHAR8;
typedef unsigned char* PBYTE;
typedef unsigned char* PUINT8;
typedef unsigned char* PULONG8;

typedef short SHORT;
typedef short SHORT16;
typedef short INT16;
typedef short LONG16;

typedef short* PSHORT;
typedef short* PSHORT16;
typedef short* PINT16;
typedef short* PLONG16;

typedef unsigned short USHORT;
typedef unsigned short USHORT16;
typedef unsigned short WORD;
typedef unsigned short UINT16;
typedef unsigned short ULONG16;

typedef unsigned short* PUSHORT;
typedef unsigned short* PUSHORT16;
typedef unsigned short* PWORD;
typedef unsigned short* PUINT16;
typedef unsigned short* PULONG16;

typedef int INT;
typedef int INT32;
typedef int BOOL;

typedef int* PINT;
typedef int* PINT32;
typedef int* PBOOL;

typedef long LONG;
typedef long LONG32;

typedef long* PLONG;
typedef long* PLONG32;

typedef unsigned int UINT;
typedef unsigned int UINT32;
typedef unsigned int ULONG;
typedef unsigned int ULONG32;

typedef unsigned int* PUINT;
typedef unsigned int* PUINT32;
typedef unsigned int* PULONG;
typedef unsigned int* PULONG32;

typedef long long INT64;
typedef long long LONGLONG;
typedef long long LONG64;

typedef long long* PINT64;
typedef long long* PLONGLONG;
typedef long long* PLONG64;

typedef unsigned long long UINT64;
typedef unsigned long long ULONGLONG;
typedef unsigned long long ULONG64;

typedef unsigned long long* PUINT64;
typedef unsigned long long* PULONGLONG;
typedef unsigned long long* PULONG64;

typedef void* PVOID;

typedef unsigned long long SIZE_T;

#endif

#ifndef ARRAYSIZE
#define ARRAYSIZE(Array) ( sizeof( Array ) / sizeof( ( Array )[ 0 ] ) )
#endif

#ifndef TRUE
#define TRUE ( 1 )
#endif

#ifndef FALSE
#define FALSE ( 0 )
#endif

#ifndef VOID
#define VOID void
#endif

#ifndef CONST
#define CONST const
#endif

#ifndef STATIC
#define STATIC static
#endif

#ifndef VOLATILE
#define VOLATILE volatile
#endif

#ifdef __cplusplus

#ifndef CONSTEXPR
#define CONSTEXPR constexpr
#endif

#ifndef VIRTUAL
#define VIRTUAL virtual
#endif

#ifndef OVERRIDE
#define OVERRIDE override
#endif

#endif

#endif // _CHIP8_DEFINES_H_