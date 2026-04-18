add_library(chip8-assembler STATIC
    assembler.cpp
)

target_include_directories(chip8-assembler
    PUBLIC
        ${CMAKE_SOURCE_DIR}/include
)

target_link_libraries(chip8-assembler
    PUBLIC chip8-encoder
)