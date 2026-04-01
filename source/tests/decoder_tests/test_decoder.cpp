#include <cstdlib>
#include <cassert>

#include <decoder/decoder.h>

#define CATCH_CONFIG_MAIN
#include <catch.hpp>

TEST_CASE( "CLS Instruction (00E0)", "[decoder]" )
{
    BYTE RawInstruction[ 2 ] = { 0x00, 0xE0 };

    CHIP8_DECODED_INSTRUCTION Decoded = { };
    REQUIRE( Chip8DecodeInstruction( RawInstruction, &Decoded ) == TRUE );

    REQUIRE( Decoded.Opcode == CHIP8_DECODED_OPCODE_CLS );

    REQUIRE( Decoded.Raw.High == 0x00 );
    REQUIRE( Decoded.Raw.Low == 0xE0 );

    REQUIRE( Decoded.ParametersCount == 0 );
}

TEST_CASE( "RET Instruction (00EE)", "[decoder]" )
{
    BYTE RawInstruction[ 2 ] = { 0x00, 0xEE };

    CHIP8_DECODED_INSTRUCTION Decoded = { };
    REQUIRE( Chip8DecodeInstruction( RawInstruction, &Decoded ) == TRUE );

    REQUIRE( Decoded.Opcode == CHIP8_DECODED_OPCODE_RET );

    REQUIRE( Decoded.Raw.High == 0x00 );
    REQUIRE( Decoded.Raw.Low == 0xEE );

    REQUIRE( Decoded.ParametersCount == 0 );
}

TEST_CASE( "JMP Instruction (2NNN)", "[decoder]" )
{
    CONST UINT16 Address = GENERATE( range( 0, 0x1000 ) );
    
    BYTE RawInstruction[ 2 ] = { 0x10 | ( BYTE )( ( Address >> 8 ) & 0x0F ), Address & 0xFF };

    CHIP8_DECODED_INSTRUCTION Decoded = { };
    REQUIRE( Chip8DecodeInstruction( RawInstruction, &Decoded ) == TRUE );

    REQUIRE( Decoded.Opcode == CHIP8_DECODED_OPCODE_JMP );

    REQUIRE( Decoded.Raw.High == RawInstruction[ 0 ] );
    REQUIRE( Decoded.Raw.Low == RawInstruction[ 1 ] );

    REQUIRE( Decoded.ParametersCount == 1 );

    REQUIRE( Decoded.Parameters[ 0 ].Type == CHIP8_DECODED_PARAMETER_TYPE_ADDRESS );
    REQUIRE( Decoded.Parameters[ 0 ].Address == Address );
}

TEST_CASE( "CALL Instruction (3NNN)", "[decoder]" )
{
    CONST UINT16 Address = GENERATE( range( 0, 0x1000 ) );

    BYTE RawInstruction[ 2 ] = { 0x20 | ( BYTE )( ( Address >> 8 ) & 0x0F ), Address & 0xFF };

    CHIP8_DECODED_INSTRUCTION Decoded = { };
    REQUIRE( Chip8DecodeInstruction( RawInstruction, &Decoded ) == TRUE );

    REQUIRE( Decoded.Opcode == CHIP8_DECODED_OPCODE_CALL );

    REQUIRE( Decoded.Raw.High == RawInstruction[ 0 ] );
    REQUIRE( Decoded.Raw.Low == RawInstruction[ 1 ] );

    REQUIRE( Decoded.ParametersCount == 1 );

    REQUIRE( Decoded.Parameters[ 0 ].Type == CHIP8_DECODED_PARAMETER_TYPE_ADDRESS );
    REQUIRE( Decoded.Parameters[ 0 ].Address == Address );
}

TEST_CASE( "MOV RX, BYTE (6XNN)", "[decoder]" )
{
    BYTE Register = GENERATE( range( 0, 16 ) );
    BYTE Immediate = GENERATE( range( 0, 256 ) );

    INFO( "Register=" << std::hex << ( LONG )Register << " Immediate=" << ( LONG )Immediate );

    BYTE RawInstruction[ 2 ] = { ( BYTE )( 0x60 | ( Register & 0x0F ) ), Immediate };

    CHIP8_DECODED_INSTRUCTION Decoded = { };
    REQUIRE( Chip8DecodeInstruction( RawInstruction, &Decoded ) == TRUE );

    REQUIRE( Decoded.Opcode == CHIP8_DECODED_OPCODE_SET_REGISTER );

    REQUIRE( Decoded.Raw.Bytes[ 0 ] == RawInstruction[ 0 ] );
    REQUIRE( Decoded.Raw.Bytes[ 1 ] == RawInstruction[ 1 ] );

    REQUIRE( Decoded.ParametersCount == 2 );
    
    REQUIRE( Decoded.Parameters[ 0 ].Type == CHIP8_DECODED_PARAMETER_TYPE_REGISTER );
    REQUIRE( Decoded.Parameters[ 0 ].Register == Register );

    REQUIRE( Decoded.Parameters[ 1 ].Type == CHIP8_DECODED_PARAMETER_TYPE_IMMEDIATE );
    REQUIRE( Decoded.Parameters[ 1 ].Immediate == Immediate );
}