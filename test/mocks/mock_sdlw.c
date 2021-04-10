/**
 * @file mock_sdlw.c
 * @author Leuenberger Niklaus (leuen4@bfh.ch)
 * @brief Mocks einiger SDLWrapper Funktionen
 * @version 0.1
 * @date 2021-04-10
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


/*
 * Mocks
 * 
 */

/**
 * @brief Mock-Ersatz für originales SDL_QueryTexture().
 * Ermöglicht das Testen ohne Laufzeitabhängigkeit von SDL.
 * 
 * @param texture unbenutzt
 * @param format unbenutzt
 * @param access unbenutzt
 * @param w wird immer auf 100 gesetzt
 * @param h wird immer auf 100 gesetzt
 * 
 * @return immer ERR_OK
 */
int SDL_QueryTexture(void *texture, unsigned int *format, int *access, int *w, int *h) {
    (*w) = 100;
    (*h) = 100;
    (void)access;
    (void)format;
    (void)texture;
    return 0;
}
