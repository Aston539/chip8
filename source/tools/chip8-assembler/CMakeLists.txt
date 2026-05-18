
set(RE2C_EXECUTABLE "C:/msys64/ucrt64/bin/re2c.exe")

set(LEXER_SRC ${CMAKE_CURRENT_SOURCE_DIR}/lexer.re)
set(LEXER_OUT ${CMAKE_CURRENT_BINARY_DIR}/lexer.cpp)

add_custom_command(
    OUTPUT ${LEXER_OUT}
    COMMAND ${RE2C_EXECUTABLE} -o ${LEXER_OUT} ${LEXER_SRC}
    DEPENDS ${LEXER_SRC}
    COMMENT "Generating lexer.cpp with re2c"
)

add_custom_target(generate_lexer DEPENDS ${LEXER_OUT})

add_executable(chip8-assembler-cli
    main.cpp
    parser.cpp
    ${LEXER_OUT})

target_include_directories(chip8-assembler-cli
    PRIVATE
        ${CMAKE_SOURCE_DIR}/include
)

target_link_libraries(chip8-assembler-cli
    PRIVATE
        chip8-assembler
)