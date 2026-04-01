#ifndef _CHIP8_INTERPRETER_DEFINES_H_
#define _CHIP8_INTERPRETER_DEFINES_H_

#include <wtypes.h>

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

#endif // _CHIP8_INTERPRETER_DEFINES_H_