
add_library( chip8-disassembler STATIC disassembler.cpp )

target_include_directories( chip8-disassembler PUBLIC ${CMAKE_SOURCE_DIR}/include )

target_link_libraries( chip8-disassembler PUBLIC chip8-decoder )