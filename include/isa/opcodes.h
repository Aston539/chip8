#ifndef _CHIP8_ISA_OPCODES_H_
#define _CHIP8_ISA_OPCODES_H_

#include <defines.h>

#define CHIP8_OPCODE_SPECIAL                  0x00
#define CHIP8_OPCODE_JMP                      0x01
#define CHIP8_OPCODE_CALL                     0x02
#define CHIP8_OPCODE_SE                       0x03 // skip if equal
#define CHIP8_OPCODE_SNE                      0x04 // skip if not equal
#define CHIP8_OPCODE_SRE                      0x05 // skip if registers equal
#define CHIP8_OPCODE_SET_REGISTER             0x06 // set register
#define CHIP8_OPCODE_ADD_IMM                  0x07 // add immediate
#define CHIP8_OPCODE_ALU                      0x08
#define CHIP8_OPCODE_SRNE                     0x09 // skip if registers not equal
#define CHIP8_OPCODE_SET_IDX                  0x0A
#define CHIP8_OPCODE_JMP_REL                  0x0B
#define CHIP8_OPCODE_RAND                     0x0C
#define CHIP8_OPCODE_DRAW                     0x0D
#define CHIP8_OPCODE_INPUT                    0x0E
#define CHIP8_OPCODE_SYSTEM                   0x0F

#define CHIP8_SPECIAL_OPCODE_CLS              0xE0
#define CHIP8_SPECIAL_OPCODE_RET              0xEE

#define CHIP8_ALU_OPCODE_MOV                  0x00
#define CHIP8_ALU_OPCODE_OR                   0x01
#define CHIP8_ALU_OPCODE_AND                  0x02
#define CHIP8_ALU_OPCODE_XOR                  0x03
#define CHIP8_ALU_OPCODE_ADD                  0x04
#define CHIP8_ALU_OPCODE_SUB                  0x05
#define CHIP8_ALU_OPCODE_SHR                  0x06
#define CHIP8_ALU_OPCODE_SUBN                 0x07
#define CHIP8_ALU_OPCODE_SHL                  0x0E

#define CHIP8_INPUT_OPCODE_SKP                0x9E // skip if pressed
#define CHIP8_INPUT_OPCODE_SKNP               0xA1 // skip if not pressed

#define CHIP8_SYSTEM_OPCODE_GET_DELAY_TIMER   0x07
#define CHIP8_SYSTEM_OPCODE_WAIT_KEY_CLICK    0x0A
#define CHIP8_SYSTEM_OPCODE_SET_DELAY_TIMER   0x15
#define CHIP8_SYSTEM_OPCODE_SET_SOUND_TIMER   0x18
#define CHIP8_SYSTEM_OPCODE_ADD_IDX           0x1E
#define CHIP8_SYSTEM_OPCODE_SET_SPRITE        0x29
#define CHIP8_SYSTEM_OPCODE_STORE_BCD         0x33
#define CHIP8_SYSTEM_OPCODE_STORE_CONTEXT     0x55
#define CHIP8_SYSTEM_OPCODE_LOAD_CONTEXT      0x65


typedef UINT16 CHIP8_OPCODE_KEY;

#define CHIP8_OPCODE_KEY_SPECIAL_CLS            ( ( CHIP8_OPCODE_SPECIAL << 8  ) | CHIP8_SPECIAL_OPCODE_CLS            )
#define CHIP8_OPCODE_KEY_SPECIAL_RET            ( ( CHIP8_OPCODE_SPECIAL << 8  ) | CHIP8_SPECIAL_OPCODE_RET            )

#define CHIP8_OPCODE_KEY_ALU_MOV                ( ( CHIP8_OPCODE_ALU     << 12 ) | CHIP8_ALU_OPCODE_MOV                )
#define CHIP8_OPCODE_KEY_ALU_OR                 ( ( CHIP8_OPCODE_ALU     << 12 ) | CHIP8_ALU_OPCODE_OR                 )
#define CHIP8_OPCODE_KEY_ALU_AND                ( ( CHIP8_OPCODE_ALU     << 12 ) | CHIP8_ALU_OPCODE_AND                )
#define CHIP8_OPCODE_KEY_ALU_XOR                ( ( CHIP8_OPCODE_ALU     << 12 ) | CHIP8_ALU_OPCODE_XOR                )
#define CHIP8_OPCODE_KEY_ALU_ADD                ( ( CHIP8_OPCODE_ALU     << 12 ) | CHIP8_ALU_OPCODE_ADD                )
#define CHIP8_OPCODE_KEY_ALU_SUB                ( ( CHIP8_OPCODE_ALU     << 12 ) | CHIP8_ALU_OPCODE_SUB                )
#define CHIP8_OPCODE_KEY_ALU_SHR                ( ( CHIP8_OPCODE_ALU     << 12 ) | CHIP8_ALU_OPCODE_SHR                )
#define CHIP8_OPCODE_KEY_ALU_SUBN               ( ( CHIP8_OPCODE_ALU     << 12 ) | CHIP8_ALU_OPCODE_SUBN               )
#define CHIP8_OPCODE_KEY_ALU_SHL                ( ( CHIP8_OPCODE_ALU     << 12 ) | CHIP8_ALU_OPCODE_SHL                )

#define CHIP8_OPCODE_KEY_INPUT_SKP              ( ( CHIP8_OPCODE_INPUT   << 12 ) | CHIP8_INPUT_OPCODE_SKP              )
#define CHIP8_OPCODE_KEY_INPUT_SKNP             ( ( CHIP8_OPCODE_INPUT   << 12 ) | CHIP8_INPUT_OPCODE_SKNP             )

#define CHIP8_OPCODE_KEY_SYSTEM_GET_DELAY_TIMER ( ( CHIP8_OPCODE_SYSTEM << 12 ) | CHIP8_SYSTEM_OPCODE_GET_DELAY_TIMER  )
#define CHIP8_OPCODE_KEY_SYSTEM_WAIT_KEY_CLICK  ( ( CHIP8_OPCODE_SYSTEM << 12 ) | CHIP8_SYSTEM_OPCODE_WAIT_KEY_CLICK   )
#define CHIP8_OPCODE_KEY_SYSTEM_SET_DELAY_TIMER ( ( CHIP8_OPCODE_SYSTEM << 12 ) | CHIP8_SYSTEM_OPCODE_SET_DELAY_TIMER  )
#define CHIP8_OPCODE_KEY_SYSTEM_SET_SOUND_TIMER ( ( CHIP8_OPCODE_SYSTEM << 12 ) | CHIP8_SYSTEM_OPCODE_SET_SOUND_TIMER  )
#define CHIP8_OPCODE_KEY_SYSTEM_ADD_IDX         ( ( CHIP8_OPCODE_SYSTEM << 12 ) | CHIP8_SYSTEM_OPCODE_ADD_IDX          )
#define CHIP8_OPCODE_KEY_SYSTEM_SET_SPRITE      ( ( CHIP8_OPCODE_SYSTEM << 12 ) | CHIP8_SYSTEM_OPCODE_SET_SPRITE       )
#define CHIP8_OPCODE_KEY_SYSTEM_STORE_BCD       ( ( CHIP8_OPCODE_SYSTEM << 12 ) | CHIP8_SYSTEM_OPCODE_STORE_BCD        )
#define CHIP8_OPCODE_KEY_SYSTEM_STORE_CONTEXT   ( ( CHIP8_OPCODE_SYSTEM << 12 ) | CHIP8_SYSTEM_OPCODE_STORE_CONTEXT    )
#define CHIP8_OPCODE_KEY_SYSTEM_LOAD_CONTEXT    ( ( CHIP8_OPCODE_SYSTEM << 12 ) | CHIP8_SYSTEM_OPCODE_LOAD_CONTEXT     )

#define CHIP8_OPCODE_KEY_JMP                    ( CHIP8_OPCODE_JMP          << 12 )
#define CHIP8_OPCODE_KEY_CALL                   ( CHIP8_OPCODE_CALL         << 12 )
#define CHIP8_OPCODE_KEY_SE_VX_IMM              ( CHIP8_OPCODE_SE           << 12 )
#define CHIP8_OPCODE_KEY_SNE_VX_IMM             ( CHIP8_OPCODE_SNE          << 12 )
#define CHIP8_OPCODE_KEY_SE_VX_VY               ( CHIP8_OPCODE_SRE          << 12 )
#define CHIP8_OPCODE_KEY_SET_VX_IMM             ( CHIP8_OPCODE_SET_REGISTER << 12 )
#define CHIP8_OPCODE_KEY_ADD_VX_IMM             ( CHIP8_OPCODE_ADD_IMM      << 12 )
#define CHIP8_OPCODE_KEY_SNE_VX_VY              ( CHIP8_OPCODE_SRNE         << 12 )
#define CHIP8_OPCODE_KEY_DRAW_VX_VY_N           ( CHIP8_OPCODE_DRAW         << 12 )
#define CHIP8_OPCODE_KEY_SET_IDX                ( CHIP8_OPCODE_SET_IDX      << 12 )
#define CHIP8_OPCODE_KEY_JMP_REL                ( CHIP8_OPCODE_JMP_REL      << 12 )
#define CHIP8_OPCODE_KEY_RAND                   ( CHIP8_OPCODE_RAND         << 12 )

#endif // _CHIP8_ISA_OPCODES_H_