# Abgeändert von einem Basis Projekt von:
# https://gitlab.com/aminosbh/sdl2-image-sample/-/tree/master/cmake/sdl2

cmake_minimum_required(VERSION 3.16)

# Projektname und Sprache
project(tanks C)

# Nutze C-Standard c11
set(CMAKE_C_STANDARD 11)

# Generiere compile_commands.json für IDEs
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Binaries und DLLs im Buildordner platzieren
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

# Alle *.c Dateien im src Ordner als Quellcode hinzufügen
file(GLOB SOURCES "src/**.c")
add_executable(${PROJECT_NAME} ${SOURCES})

# Alle Dateien im Include Ordner verfügbar machen
target_include_directories(${PROJECT_NAME} PRIVATE include)

# Compiler Warnungen aktivieren
if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE /W4 /WX)
    target_compile_definitions(${PROJECT_NAME} PRIVATE -D_CRT_SECURE_NO_WARNINGS)
else()
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -pedantic -Werror)
endif()

# Bibliotheken laden
add_subdirectory(lib)

# Tests aktivieren
enable_testing()
add_subdirectory(test)

# Assets in Buildordner kopieren
file(COPY assets DESTINATION ${CMAKE_BINARY_DIR})
