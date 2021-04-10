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

# Alle *.c Dateien im src Ordner ausser main.c als Bibliothek hinzufügen
file(GLOB SOURCES "src/**.c")
list(FILTER SOURCES EXCLUDE REGEX ".*main.c$")
add_library(main STATIC ${SOURCES})

# Alle Dateien im Include Ordner verfügbar machen
target_include_directories(main PUBLIC include)

# Compiler Warnungen aktivieren
if(MSVC)
    target_compile_options(main PUBLIC /W4 /WX)
    target_compile_definitions(main PUBLIC -D_CRT_SECURE_NO_WARNINGS)
else()
    target_compile_options(main PUBLIC -Wall -Wextra -pedantic -Werror)
endif()

# Bibliotheken laden
add_subdirectory(lib)

# Tests aktivieren
enable_testing()
add_subdirectory(test)

# Assets in Buildordner kopieren
file(COPY assets DESTINATION ${CMAKE_BINARY_DIR})

# EXE des Projekts erstellen
add_executable(${PROJECT_NAME} "src/main.c")
target_link_libraries(${PROJECT_NAME} main)
