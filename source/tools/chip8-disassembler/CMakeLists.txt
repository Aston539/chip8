
add_executable( chip8-disassembler-cli main.cpp )

target_include_directories( chip8-disassembler-cli PRIVATE ${CMAKE_SOURCE_DIR}/include )

target_link_libraries( chip8-disassembler-cli PRIVATE chip8-disassembler )