/**
 * @file test_sdl.c
 * @author Leuenberger Niklaus (leuen4@bfh.ch)
 * @brief Teste auf korrekte Funktion der SDL Bibliotheken
 * @version 0.1
 * @date 2021-03-01
 * 
 * @copyright Copyright (c) 2021 Leuenberger Niklaus
 * 
 */


/*
 * Includes
 * 
 */

#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>

#define SDL_MAIN_HANDLED
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include <SDL_net.h>


/*
 * Tests
 * 
 */

/**
 * @brief Teste ob SDL Bibliothek initialisiert werden kann
 * 
 * @param state unbenutzt
 */
static void sdl_can_init(void **state) {
    (void) state;
    assert_int_equal(SDL_Init(SDL_INIT_EVERYTHING), 0);
    SDL_Quit();
}

/**
 * @brief Teste ob SDL_image Bibliothek initialisiert werden kann
 * 
 * @param state unbenutzt
 */
static void sdl_can_init_image(void **state) {
    (void) state;
    int flags = IMG_INIT_JPG | IMG_INIT_PNG | IMG_INIT_TIF | IMG_INIT_WEBP;
    assert_int_equal(IMG_Init(flags), flags);
    IMG_Quit();
}

/**
 * @brief Teste ob SDL_ttf Bibliothek initialisiert werden kann
 * 
 * @param state unbenutzt
 */
static void sdl_can_init_ttf(void **state) {
    (void) state;
    assert_int_equal(TTF_Init(), 0);
    TTF_Quit();
}

/**
 * @brief Teste ob SDL_mixer Bibliothek initialisiert werden kann
 * 
 * @param state unbenutzt
 */
static void sdl_can_init_mixer(void **state) {
    (void) state;
    int flags = MIX_INIT_FLAC | MIX_INIT_MOD | MIX_INIT_MP3 | MIX_INIT_OGG | MIX_INIT_MID | MIX_INIT_OPUS;
    assert_int_equal(Mix_Init(flags), flags);
    Mix_Quit();
}

/**
 * @brief Teste ob SDL_net Bibliothek initialisiert werden kann
 * 
 * @param state unbenutzt
 */
static void sdl_can_init_net(void **state) {
    (void) state;
    assert_int_equal(SDLNet_Init(), 0);
    SDLNet_Quit();
}

/**
 * @brief Starte alle SDL-Systeme
 * 
 * @param state unbenutzt
 * 
 * @return immer 0
 */
static int initSDLSystems(void **state) {
    (void) state;
    SDL_Init(SDL_INIT_EVERYTHING);
    IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG | IMG_INIT_TIF | IMG_INIT_WEBP);
    TTF_Init();
    Mix_Init(MIX_INIT_FLAC | MIX_INIT_MOD | MIX_INIT_MP3 | MIX_INIT_OGG | MIX_INIT_MID | MIX_INIT_OPUS);
    SDLNet_Init();
    return 0;
}

/**
 * @brief Stoppe alle SDL-Systeme
 * 
 * @param state unbenutzt
 * 
 * @return immer 0
 */
static int deinitSDLSystems(void **state) {
    (void) state;
    SDLNet_Quit();
    Mix_Quit();
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
    return 0;
}

/**
 * @brief Teste ob Fenster erstellt werden kann
 * 
 * @param state unused
 * 
 * @note Für CI Tests in der GitLab Pipeline muss die Umgebungsvariable SDL_VIDEODRIVER=dummy gesetzt sein.
 * 
 */
static void sdl_can_create_window(void **state) {
    (void) state;
    SDL_Window *window = SDL_CreateWindow("test",
                                          SDL_WINDOWPOS_UNDEFINED,
                                          SDL_WINDOWPOS_UNDEFINED,
                                          800, 600,
                                          SDL_WINDOW_SHOWN);
    assert_non_null(window);
    SDL_Event e;
    SDL_WaitEventTimeout(&e, 1000);
    SDL_DestroyWindow(window);
}

/**
 * @brief Erstelle Fenster
 * 
 * @param[out] state Pointer auf SDL_Window 
 * 
 * @return 0 Setup erfolgreich, -1 Setup fehlgeschlagen
 */
static int createWindow(void **state) {
    SDL_Window *window = SDL_CreateWindow("test",
                                          SDL_WINDOWPOS_UNDEFINED,
                                          SDL_WINDOWPOS_UNDEFINED,
                                          800, 600,
                                          SDL_WINDOW_SHOWN);
    *state = window;
    return (window ? 0 : -1);
}

/**
 * @brief Zerstöre Fenster
 * 
 * @param[in] state Pointer auf SDL_Window
 * 
 * @return 0 Teardown erfolgreich, -1 Teardown fehlgeschlagen
 */
static int destroyWindow(void **state) {
    SDL_Event e;
    SDL_WaitEventTimeout(&e, 1000);
    SDL_Window *window = (SDL_Window*)*state;
    SDL_DestroyWindow(window);
    *state = NULL;
    return 0;
}

/**
 * @brief Teste ob Renderer erstellt werden kann
 * 
 * @param[in] state Pointer auf SDL_Window
 */
static void sdl_can_create_renderer(void **state) {
    SDL_Window *window = (SDL_Window*)*state;
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 0);
    assert_non_null(renderer);
    SDL_DestroyRenderer(renderer);
}


/**
 * @brief Testprogramm
 * 
 * @return int Anzahl fehlgeschlagener Tests
 */
int main(void) {
    int failed = 0;
    const struct CMUnitTest sdl_init[] = {
        cmocka_unit_test(sdl_can_init),
        cmocka_unit_test(sdl_can_init_image),
        cmocka_unit_test(sdl_can_init_ttf),
        cmocka_unit_test(sdl_can_init_mixer),
        cmocka_unit_test(sdl_can_init_net),
    };
    failed += cmocka_run_group_tests(sdl_init, NULL, NULL);
    const struct CMUnitTest sdl[] = {
        cmocka_unit_test(sdl_can_create_window),
        cmocka_unit_test_setup_teardown(sdl_can_create_renderer, createWindow, destroyWindow),
    };
    failed += cmocka_run_group_tests(sdl, initSDLSystems, deinitSDLSystems);
    return failed;
}
