#include <cstdlib>
#include <cassert>

#define CATCH_CONFIG_MAIN
#include <catch.hpp>

#include <decoder/decoder.h>

TEST_CASE( "CLS Instruction (00E0)", "[decoder]" )
{
    BYTE RawInstruction[ 2 ] = { 0x00, 0xE0 };

    CHIP8_DECODED_INSTRUCTION Decoded = { };
    REQUIRE( Chip8DecodeInstruction( ( UINT16* )RawInstruction, &Decoded ) == TRUE );

    REQUIRE( Decoded.Opcode == CHIP8_OPCODE_SPECIAL );
    REQUIRE( Decoded.ExtendedOpcode == CHIP8_SPECIAL_OPCODE_CLS );

    REQUIRE( Decoded.High == 0x00 );
    REQUIRE( Decoded.Low == 0xE0 );
}

TEST_CASE( "RET Instruction (00EE)", "[decoder]" )
{
    BYTE RawInstruction[ 2 ] = { 0x00, 0xEE };

    CHIP8_DECODED_INSTRUCTION Decoded = { };
    REQUIRE( Chip8DecodeInstruction( ( UINT16* )RawInstruction, &Decoded ) == TRUE );

    REQUIRE( Decoded.Opcode == CHIP8_OPCODE_SPECIAL );
    REQUIRE( Decoded.ExtendedOpcode == CHIP8_SPECIAL_OPCODE_RET );

    REQUIRE( Decoded.High == 0x00 );
    REQUIRE( Decoded.Low == 0xEE );
}

TEST_CASE( "JMP Instruction (2NNN)", "[decoder]" )
{
    CONST UINT16 Address = GENERATE( range( 0, 0x1000 ) );
    
    BYTE RawInstruction[ 2 ] = { 0x10 | ( BYTE )( ( Address >> 8 ) & 0x0F ), Address & 0xFF };

    CHIP8_DECODED_INSTRUCTION Decoded = { };
    REQUIRE( Chip8DecodeInstruction( ( UINT16* )RawInstruction, &Decoded ) == TRUE );

    REQUIRE( Decoded.Opcode == CHIP8_OPCODE_JMP );

    REQUIRE( Decoded.High == RawInstruction[ 0 ] );
    REQUIRE( Decoded.Low == RawInstruction[ 1 ] );

    REQUIRE( Decoded.Address == Address );
}

TEST_CASE( "CALL Instruction (3NNN)", "[decoder]" )
{
    CONST UINT16 Address = GENERATE( range( 0, 0x1000 ) );

    BYTE RawInstruction[ 2 ] = { 0x20 | ( BYTE )( ( Address >> 8 ) & 0x0F ), Address & 0xFF };

    CHIP8_DECODED_INSTRUCTION Decoded = { };
    REQUIRE( Chip8DecodeInstruction( ( UINT16* )RawInstruction, &Decoded ) == TRUE );

    REQUIRE( Decoded.Opcode == CHIP8_OPCODE_CALL );

    REQUIRE( Decoded.High == RawInstruction[ 0 ] );
    REQUIRE( Decoded.Low == RawInstruction[ 1 ] );

    REQUIRE( Decoded.Address == Address );
}

TEST_CASE( "MOV RX, BYTE (6XNN)", "[decoder]" )
{
    BYTE Register = GENERATE( range( 0, 16 ) );
    BYTE Immediate = GENERATE( range( 0, 256 ) );

    INFO( "Register=" << std::hex << ( LONG )Register << " Immediate=" << ( LONG )Immediate );

    BYTE RawInstruction[ 2 ] = { ( BYTE )( 0x60 | ( Register & 0x0F ) ), Immediate };

    CHIP8_DECODED_INSTRUCTION Decoded = { };
    REQUIRE( Chip8DecodeInstruction( ( UINT16* )RawInstruction, &Decoded ) == TRUE );

    REQUIRE( Decoded.Opcode == CHIP8_OPCODE_SET_REGISTER );

    REQUIRE( Decoded.High == RawInstruction[ 0 ] );
    REQUIRE( Decoded.Low == RawInstruction[ 1 ] );

    REQUIRE( Decoded.RegisterX == Register );
    //REQUIRE( Decoded.Immediate8 == Immediate );
}