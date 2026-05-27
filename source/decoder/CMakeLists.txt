
add_library( chip8-decoder STATIC decoder.cpp )

target_include_directories( chip8-decoder PUBLIC ${CMAKE_SOURCE_DIR}/include )