file(GLOB SRC_FILES
    src/*.c
)

add_executable(midi-app
    ${SRC_FILES}
)

target_include_directories(midi-app
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

find_package(SDL2 REQUIRED)
target_link_libraries(midi-app
    PRIVATE
        midi-lib
        SDL2::SDL2
)
