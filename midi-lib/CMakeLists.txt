cmake_minimum_required(VERSION 3.16)

file(GLOB SRC_FILES
    src/*.c
)


add_library(midi-lib
    "src/midi.c"
    "src/midierr.c"
    "src/bytereader.c"
    src/midieventvector.c

)

target_include_directories(midi-lib
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_compile_features(midi-lib
    PUBLIC
    c_std_11
)

target_compile_options(midi-lib
    PRIVATE
        -Wall
        -pedantic
)

add_executable(midi-test
    tests/test_midi.c
)

target_link_libraries(midi-test
    PRIVATE
        midi-lib
)