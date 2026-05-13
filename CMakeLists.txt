cmake_minimum_required ( VERSION 3.10 )

project(chip8 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#
# libraries
#
add_subdirectory( source/decoder )
add_subdirectory( source/disassembler )
add_subdirectory( source/encoder )
add_subdirectory( source/assembler )
add_subdirectory( source/interpreter )

#
# tools
#
add_subdirectory( source/tools/chip8-disassembler )
add_subdirectory( source/tools/chip8-assembler )
add_subdirectory( source/tools/chip8-interpreter )
add_subdirectory( source/tools/chip8-studio )

#
# tests
#
enable_testing( )

add_subdirectory( source/tests )