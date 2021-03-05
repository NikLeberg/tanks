list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/lib/cmocka/cmake/Modules")

# Makro um Test-Target mit cmocka und allen SDL Bibliotheken standartmässig zu verlinken
macro(add_custom_test _TEST_NAME _TEST_SOURCES _TEST_INCLUDE _TEST_LIBRARIES)
    add_executable(${_TEST_NAME} ${_TEST_SOURCES})
    target_link_libraries(${_TEST_NAME}
        cmocka
        SDL2::Main
        SDL2::Image
        SDL2::Net
        SDL2::TTF
        SDL2::Mixer
        ${_TEST_LIBRARIES})
    target_include_directories(${_TEST_NAME} PRIVATE
        ${CMAKE_SOURCE_DIR}/include
        ${cmocka_BINARY_DIR}/include
        ${SDL2_INCLUDE_DIR}
        ${SDL2_IMAGE_INCLUDE_DIR}
        ${SDL2_NET_INCLUDE_DIR}
        ${SDL2_TTF_INCLUDE_DIR}
        ${SDL2_MIXER_INCLUDE_DIR}
        ${_TEST_INCLUDE})
    add_test(NAME ${_TEST_NAME} COMMAND ${_TEST_NAME} WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
endmacro()

add_custom_test(test_sdl test_sdl.c "" "")
add_custom_test(test_adder "test_adder.c;../src/adder.c" "" "")
