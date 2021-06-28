# Makro um Test-Target zu erstellen
macro(add_custom_test _TEST_NAME _TEST_SOURCES)
    add_executable(${_TEST_NAME} ${_TEST_SOURCES})
    # Bibliotheken verlinken
    target_link_libraries(${_TEST_NAME} main)
    # Möglicherweise definieren Mocks Symbole doppelt
    if("${_TEST_SOURCES}" MATCHES "mock")
        # Die Linker mögen keine doppelt definierten Symbole die durch Mocks
        # entstehen. Mit den Flags werden sie angewiesen jeweils nur die erste
        # Definition zu berücksichtigen.
        if(MSVC)
            target_link_options(${_TEST_NAME} PRIVATE "/FORCE:MULTIPLE")
            # Ignoriere die entstehenden Warnungen
            target_link_options(${_TEST_NAME} PRIVATE "/IGNORE:4075") # /INCREMENTAL wird aufgrund der Angabe von /FORCE ignoriert.
            target_link_options(${_TEST_NAME} PRIVATE "/IGNORE:4006") # [Symbol] ist bereits in "[Datei]" definiert; zweite Definition wird ignoriert.
            target_link_options(${_TEST_NAME} PRIVATE "/IGNORE:4088") # Image wird aufgrund der Option /FORCE generiert; Image kann möglicherweise nicht ausgeführt werden.
        else()
            target_link_options(${_TEST_NAME} PRIVATE "-Wl,--allow-multiple-definition")
        endif()
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
add_custom_test(test_sprite "test_sprite.c;mocks/mock_heap.c;mocks/mock_sdl.c")
add_custom_test(test_sdlw_visual "test_sdlw_visual.c;mocks/mock_heap.c")

# Automatischer World Test. Es werden alle Funktionen von SDL gemockt, die mit Texturen oder Audio zu tun haben
add_custom_test(test_world_auto "test_world_auto.c;mocks/mock_heap.c;mocks/mock_sdl.c")
add_custom_test(test_world_visual "test_world_visual.c;mocks/mock_heap.c")

add_custom_test(test_entityHandler "test_entityHandler.c;mocks/mock_sdlw.c;mocks/mock_physics.c")

if(NOT MSVC)
    add_custom_test(test_physics "test_physics.c")
    target_link_options(test_physics PRIVATE "-Wl,--wrap=World_CheckCollision")
endif()

add_custom_test(test_gui "test_gui.c;mocks/mock_sdlw.c;mocks/mock_sdl.c")

add_custom_test(test_tank "test_tank.c;mocks/mock_heap.c")
