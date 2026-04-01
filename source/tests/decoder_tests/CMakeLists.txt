add_executable(decoder_tests
    "test_decoder.cpp"
)

target_link_libraries(decoder_tests
    PRIVATE chip8-decoder
)

target_include_directories(decoder_tests
    PRIVATE
        ${CMAKE_SOURCE_DIR}/source/tests/extern
)

add_test(NAME decoder_tests COMMAND decoder_tests)