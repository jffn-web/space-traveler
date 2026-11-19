cmake_minimum_required(VERSION 3.24)

project(SpaceTraveler LANGUAGES C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

include(FetchContent)

# Compila a Raylib junto com o jogo.
set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)

FetchContent_Declare(
    raylib
    URL https://github.com/raysan5/raylib/archive/refs/tags/6.0.tar.gz
    DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)

FetchContent_MakeAvailable(raylib)

# Define o código que será compilado.
add_executable(space-traveler src/main.c)

# Liga o jogo à Raylib.
target_link_libraries(space-traveler PRIVATE raylib)

# Copia as imagens para perto do executável.
add_custom_command(
    TARGET space-traveler POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_CURRENT_SOURCE_DIR}/assets"
        "$<TARGET_FILE_DIR:space-traveler>/assets"
    VERBATIM
)