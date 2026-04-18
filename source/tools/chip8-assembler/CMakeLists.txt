add_executable(chip8-assembler-cli
    main.cpp
)

target_include_directories(chip8-assembler-cli
    PRIVATE
        ${CMAKE_SOURCE_DIR}/include
)

target_link_libraries(chip8-assembler-cli
    PRIVATE
        chip8-assembler
)