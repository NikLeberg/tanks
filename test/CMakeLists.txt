# Makro um Test-Target zu erstellen
macro(add_custom_test _TEST_NAME _TEST_SOURCES)
    add_executable(${_TEST_NAME} ${_TEST_SOURCES})
    # Bibliotheken verlinken
    target_link_libraries(${_TEST_NAME} main)
    # Test als solchen hinzufügen
    add_test(NAME ${_TEST_NAME} COMMAND ${_TEST_NAME} WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
    # Erkenne wenn wir im GitLab-CI laufen
    if(DEFINED ENV{CI})
        target_compile_definitions(${_TEST_NAME} PRIVATE -DCI_TEST)
    endif()
endmacro()

add_custom_test(test_sdl "test_sdl.c")
add_custom_test(test_adder "test_adder.c")
add_custom_test(test_list "test_list.c")
target_link_options(test_list PRIVATE "-Wl,--wrap=malloc,--wrap=free")
