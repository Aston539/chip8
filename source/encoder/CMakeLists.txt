
add_library( chip8-encoder STATIC encoder.cpp )

target_include_directories( chip8-encoder PUBLIC ${CMAKE_SOURCE_DIR}/include )