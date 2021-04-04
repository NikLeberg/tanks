list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/lib/cmocka/cmake/Modules")

# Makro um Test-Target mit cmocka und allen SDL Bibliotheken standartmässig zu verlinken
macro(add_custom_test _TEST_NAME _TEST_SOURCES)
    add_executable(${_TEST_NAME} ${_TEST_SOURCES})
    # Bibliotheken verlinken
    target_link_libraries(${_TEST_NAME}
        cmocka
        SDL2::Main
        SDL2::Image
        SDL2::Net
        SDL2::TTF
        SDL2::Mixer)
    # Includepfade hinzufügen
    target_include_directories(${_TEST_NAME} PRIVATE
        ${CMAKE_SOURCE_DIR}/include
        ${cmocka_BINARY_DIR}/include
        ${SDL2_INCLUDE_DIR}
        ${SDL2_IMAGE_INCLUDE_DIR}
        ${SDL2_NET_INCLUDE_DIR}
        ${SDL2_TTF_INCLUDE_DIR}
        ${SDL2_MIXER_INCLUDE_DIR})
    # Test als solchen hinzufügen
    add_test(NAME ${_TEST_NAME} COMMAND ${_TEST_NAME} WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
    # Compiler Warnungen aktivieren
    if(MSVC)
        target_compile_options(${_TEST_NAME} PRIVATE /W4 /WX)
        target_compile_definitions(${_TEST_NAME} PRIVATE -D_CRT_SECURE_NO_WARNINGS)
    else()
        target_compile_options(${_TEST_NAME} PRIVATE -Wall -Wextra -pedantic -Werror)
    endif()
    # Erkenne wenn wir im GitLab-CI laufen
    if(DEFINED ENV{CI})
        target_compile_definitions(${_TEST_NAME} PRIVATE -DCI_TEST)
    endif()
endmacro()

add_custom_test(test_sdl test_sdl.c)
add_custom_test(test_adder "test_adder.c;../src/adder.c")
