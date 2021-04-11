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
 * @brief Mockup des echten \ref SDLW_DrawTexture().
 * 
 * Ersetze das originale \ref SDLW_DrawTexture() mit dieser Funktion.
 * Somit kann isoliert getestet werden.
 * 
 * @param sprite zu zeichnendes Sprite
 * 
 * @return Fehlercode gemäss will_return() von CMocka
 */
int SDLW_DrawTexture(int sprite) {
    (void)sprite;
    function_called();
    return mock_type(int);
}
