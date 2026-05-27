
add_library( chip8-interpreter STATIC display.cpp
                                      machine.cpp
                                      processor.cpp )

target_include_directories( chip8-interpreter PUBLIC ${CMAKE_SOURCE_DIR}/include )

target_link_libraries( chip8-interpreter PUBLIC chip8-disassembler )