#ifndef _CHIP8_ISA_MNEMONIC_H_
#define _CHIP8_ISA_MNEMONIC_H_

#include <defines.h>

typedef BYTE CHIP8_MNEMONIC;

#define CHIP8_MNEMONIC_NONE   ( 0x00 )
#define CHIP8_MNEMONIC_CLS    ( 0x01 )
#define CHIP8_MNEMONIC_RET    ( 0x02 )
#define CHIP8_MNEMONIC_JMP    ( 0x03 )
#define CHIP8_MNEMONIC_CALL   ( 0x04 )
#define CHIP8_MNEMONIC_SE     ( 0x05 )
#define CHIP8_MNEMONIC_SNE    ( 0x06 )
#define CHIP8_MNEMONIC_MOV    ( 0x07 )
#define CHIP8_MNEMONIC_OR     ( 0x08 )
#define CHIP8_MNEMONIC_AND    ( 0x09 )
#define CHIP8_MNEMONIC_XOR    ( 0x0A )
#define CHIP8_MNEMONIC_ADD    ( 0x0B )
#define CHIP8_MNEMONIC_SUB    ( 0x0C )
#define CHIP8_MNEMONIC_SHR    ( 0x0D )
#define CHIP8_MNEMONIC_SUBN   ( 0x0E )
#define CHIP8_MNEMONIC_SHL    ( 0x0F )
#define CHIP8_MNEMONIC_SKP    ( 0x10 )
#define CHIP8_MNEMONIC_SKNP   ( 0x11 )
#define CHIP8_MNEMONIC_HLT    ( 0x12 )
#define CHIP8_MNEMONIC_RAND   ( 0x13 )
#define CHIP8_MNEMONIC_DRAW   ( 0x14 )
#define CHIP8_MNEMONIC_VMSAVE ( 0x15 )
#define CHIP8_MNEMONIC_VMLOAD ( 0x16 )

#endif // _CHIP8_ISA_MNEMONIC_H_