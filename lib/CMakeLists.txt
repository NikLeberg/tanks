# find_package-Module für SDL einbinden
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/SDL/cmake/sdl2)

# Warnungen für SDL2 <-> SDL2main Abweichung unterdrücken
set(CMAKE_SUPPRESS_DEVELOPER_WARNINGS 1)

# Makro um einen Hint des Pfads für find_package() von SDL zu setzen
macro(sdl_set_path_hint _SDL_PART _SDL_MINGW_32 _SDL_MINGW_64 _SDL_VS _SDL_APPLE)
    if(WIN32)
        if(MINGW)
            if("${CMAKE_SIZEOF_VOID_P}" STREQUAL "4")
                set(${_SDL_PART}_PATH "${CMAKE_CURRENT_SOURCE_DIR}${_SDL_MINGW_32}" CACHE STRING "")
            else()
                set(${_SDL_PART}_PATH "${CMAKE_CURRENT_SOURCE_DIR}${_SDL_MINGW_64}" CACHE STRING "")
            endif()
        else()
            set(${_SDL_PART}_PATH "${CMAKE_CURRENT_SOURCE_DIR}${_SDL_VS}" CACHE STRING "")
        endif()
    elseif(APPLE)
        # ungetestet
        set(${_SDL_PART}_PATH "${CMAKE_CURRENT_SOURCE_DIR}${_SDL_APPLE}" CACHE STRING "")
    endif()
endmacro()

# SDL DLLs nach build kopieren
macro(sdl_install_dlls _SDL_PART)
    if(WIN32)
        if(MINGW)
            file(GLOB ${_SDL_PART}_WINDOWS_DLLS "${${_SDL_PART}_PATH}/bin/*.dll")
        else()
            if("${CMAKE_SIZEOF_VOID_P}" STREQUAL "4")
                file(GLOB ${_SDL_PART}_WINDOWS_DLLS "${${_SDL_PART}_PATH}/lib/x86/*.dll")
            else()
                file(GLOB ${_SDL_PART}_WINDOWS_DLLS "${${_SDL_PART}_PATH}/lib/x64/*.dll")
            endif()
        endif()
        file(COPY ${${_SDL_PART}_WINDOWS_DLLS} DESTINATION ${CMAKE_BINARY_DIR})
    elseif(APPLE)
        # ungetestet
        file(GLOB ${_SDL_PART}_APPLE_FRAMEWORK "${${_SDL_PART}_PATH}/*.framework")
        file(COPY "${${_SDL_PART}_APPLE_FRAMEWORK}" DESTINATION /Library/Frameworks)
    endif()
endmacro()

# SDL2
sdl_set_path_hint("SDL2"
    "/SDL/windows/mingw/i686-w64-mingw32"
    "/SDL/windows/mingw/x86_64-w64-mingw32"
    "/SDL/windows/vs"
    "/SDL/apple")
find_package(SDL2 REQUIRED)
target_link_libraries(main SDL2::Main)
include_directories(${SDL2_INCLUDE_DIR})
sdl_install_dlls("SDL2")

# SDL2_image
sdl_set_path_hint("SDL2_IMAGE"
    "/SDL_image/windows/mingw/i686-w64-mingw32"
    "/SDL_image/windows/mingw/x86_64-w64-mingw32"
    "/SDL_image/windows/vs"
    "/SDL_image/apple")
find_package(SDL2_image REQUIRED)
target_link_libraries(main SDL2::Image)
sdl_install_dlls("SDL2_IMAGE")

# SDL2_net
sdl_set_path_hint("SDL2_NET"
    "/SDL_net/windows/mingw/i686-w64-mingw32"
    "/SDL_net/windows/mingw/x86_64-w64-mingw32"
    "/SDL_net/windows/vs"
    "/SDL_net/apple")
find_package(SDL2_net REQUIRED)
target_link_libraries(main SDL2::Net)
sdl_install_dlls("SDL2_NET")

# SDL2_ttf
sdl_set_path_hint("SDL2_TTF"
    "/SDL_ttf/windows/mingw/i686-w64-mingw32"
    "/SDL_ttf/windows/mingw/x86_64-w64-mingw32"
    "/SDL_ttf/windows/vs"
    "/SDL_ttf/apple")
find_package(SDL2_ttf REQUIRED)
target_link_libraries(main SDL2::TTF)
sdl_install_dlls("SDL2_TTF")

# SDL2_mixer
sdl_set_path_hint("SDL2_MIXER"
    "/SDL_mixer/windows/mingw/i686-w64-mingw32"
    "/SDL_mixer/windows/mingw/x86_64-w64-mingw32"
    "/SDL_mixer/windows/vs"
    "/SDL_mixer/apple")
find_package(SDL2_mixer REQUIRED)
target_link_libraries(main SDL2::Mixer)
sdl_install_dlls("SDL2_MIXER")

# CMocka
set(UNIT_TESTING OFF) # keine Tests von CMocka selber kompilieren
set(WITH_EXAMPLES OFF) # keine Beispiele kompilieren
set(WITH_CMOCKERY_SUPPORT OFF) # Mock-Support deaktivieren
add_subdirectory(cmocka)
target_link_libraries(main cmocka)

# Mathematik
include(CheckCSourceCompiles)
set(LIBM_TEST_SOURCE "#include<math.h>\nfloat f; int main(){sqrt(f);return 0;}")
check_c_source_compiles("${LIBM_TEST_SOURCE}" HAVE_MATH)
if(NOT HAVE_MATH)
    set(CMAKE_REQUIRED_LIBRARIES m)
    check_c_source_compiles("${LIBM_TEST_SOURCE}" HAVE_LIBM_MATH)
    unset(CMAKE_REQUIRED_LIBRARIES)
    if(HAVE_LIBM_MATH)
        # unter Linux wird libm nicht automatisch verlinkt
        target_link_libraries(main m)
    else()
        message(FATAL_ERROR "Unable to use C math library functions")
    endif()
endif()
