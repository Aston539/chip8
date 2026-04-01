cmake_minimum_required ( VERSION 3.10 )

project(chip8 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#
# libraries
#
add_subdirectory( source/decoder )
add_subdirectory( source/disassembler )

#
# tools
#
add_subdirectory( source/tools/chip8-disassembler )

#
# tests
#
enable_testing( )

add_subdirectory( source/tests )