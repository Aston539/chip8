
set(Qt6_DIR "C:/Qt/6.11.0/msvc2022_64/lib/cmake/Qt6")
find_package(Qt6 REQUIRED COMPONENTS Widgets)

qt_standard_project_setup()

qt_add_executable(chip8-studio
    main.cpp
    studio.cpp
    session.cpp
	displayview.cpp
	memorymodel.cpp
	disassemblymodel.cpp
    ${CMAKE_SOURCE_DIR}/include/tools/chip8-studio/session.h
    ${CMAKE_SOURCE_DIR}/include/tools/chip8-studio/sessionview.h
    ${CMAKE_SOURCE_DIR}/include/tools/chip8-studio/displayview.h
    ${CMAKE_SOURCE_DIR}/include/tools/chip8-studio/disassemblymodel.h
    ${CMAKE_SOURCE_DIR}/include/tools/chip8-studio/memorymodel.h
    ${CMAKE_SOURCE_DIR}/include/tools/chip8-studio/studio.h
)

target_include_directories(chip8-studio
    PRIVATE
        ${CMAKE_SOURCE_DIR}/include
)

target_link_libraries(chip8-studio
    PRIVATE
		Qt6::Widgets
        chip8-encoder
        chip8-decoder
        chip8-assembler
        chip8-disassembler
        chip8-interpreter
)