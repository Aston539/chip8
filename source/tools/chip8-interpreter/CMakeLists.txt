
add_executable( chip8-interpreter-cli main.cpp )

target_include_directories( chip8-interpreter-cli PRIVATE ${CMAKE_SOURCE_DIR}/include )

target_link_libraries( chip8-interpreter-cli PRIVATE chip8-interpreter )