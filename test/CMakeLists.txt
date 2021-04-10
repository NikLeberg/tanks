# Makro um Test-Target zu erstellen
macro(add_custom_test _TEST_NAME _TEST_SOURCES)
    add_executable(${_TEST_NAME} ${_TEST_SOURCES})
    # Bibliotheken verlinken
    target_link_libraries(${_TEST_NAME} main)
    # Möglicherweise definieren Mocks Symbole doppelt
    if(MSVC AND "${_TEST_SOURCES}" MATCHES "mock")
        # Der MSVC Linker mag keine doppelt definierten Symbole die durch Mocks
        # entstehen. Mit dem Force-Flag wird er angewiesen nur die erste
        # Definition zu berücksichtigen. GCC ist dies egal.
        target_link_options(${_TEST_NAME} PRIVATE "/FORCE:MULTIPLE")
        # Ignoriere die entstehenden Warnungen
        target_link_options(${_TEST_NAME} PRIVATE "/IGNORE:4075") # /INCREMENTAL wird aufgrund der Angabe von /FORCE ignoriert.
        target_link_options(${_TEST_NAME} PRIVATE "/IGNORE:4006") # [Symbol] ist bereits in "[Datei]" definiert; zweite Definition wird ignoriert.
        target_link_options(${_TEST_NAME} PRIVATE "/IGNORE:4088") # Image wird aufgrund der Option /FORCE generiert; Image kann möglicherweise nicht ausgeführt werden.
    endif()
    # Test als solchen hinzufügen
    add_test(NAME ${_TEST_NAME} COMMAND ${_TEST_NAME} WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
    # Erkenne wenn wir im GitLab-CI laufen
    if(DEFINED ENV{CI})
        target_compile_definitions(${_TEST_NAME} PRIVATE -DCI_TEST)
    endif()
    # Bounds-Checking aktivieren
    if(MSVC)
        target_compile_options(${_TEST_NAME} PUBLIC /RTCs)
    else()
        target_compile_options(${_TEST_NAME} PUBLIC -fbounds-check)
    endif()
endmacro()

add_custom_test(test_sdl "test_sdl.c")

add_custom_test(test_adder "test_adder.c")

add_custom_test(test_list "test_list.c;mocks/mock_heap.c")

# Automatischer SDLW Test. Es werden alle Funktionen von SDL gemockt, die mit Texturen oder Audio zu tun haben
add_custom_test(test_sdlw_auto "test_sdlw_auto.c;mocks/mock_heap.c;mocks/mock_sdl.c")

add_custom_test(test_sprite "test_sprite.c;mocks/mock_heap.c;mocks/mock_sdlw.c")

add_custom_test(test_sdlw_visual "test_sdlw_visual.c")
