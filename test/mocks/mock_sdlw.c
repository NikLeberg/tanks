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

/**
 * @brief Mockup des echten \ref SDLW_DrawFilledRect().
 * 
 * Ersetze das originale \ref SDLW_DrawFilledRect() mit dieser Funktion.
 * Somit kann isoliert getestet werden.
 * 
 * @param rect Die Position und Grösse des zu zeichnenden Rechtecks
 * @param color Die Farbe des Rechtecks
 * 
 * @return Fehlercode gemäss will_return() von CMocka
 */
int SDLW_DrawFilledRect(int rect, int color) {
    (void)rect;
    (void)color;
    function_called();
    return mock_type(int);
}

/**
 * @brief Mockup des echten \ref SDLW_PlaySoundEffect().
 * 
 * Ersetze das originale \ref SDLW_PlaySoundEffect() mit dieser Funktion.
 * Somit kann isoliert getestet werden.
 * 
 * @param chunk Die Id des Effekts der gespielt werden soll
 * 
 * @return Fehlercode gemäss will_return() von CMocka
 */
int SDLW_PlaySoundEffect(int *chunk) {
    (void)chunk;
    function_called();
    return mock_type(int);
}

/**
 * @brief Mockup des echten \ref SDLW_CreateTextTexture().
 * 
 * Ersetze das originale \ref SDLW_CreateTextTexture() mit dieser Funktion.
 * Somit kann isoliert getestet werden.
 * 
 * @param text Der Text der ausgegeben wird
 * @param font Die ID der verwendeten Font
 * @param color Die Farbe in der der Text ausgegeben wird
 * @param texture Die erstellte Textur mit dem gegebenen Text darauf
 * 
 * @return Fehlercode gemäss will_return() von CMocka
 */
int SDLW_CreateTextTexture(char *text, char *font, int color, void **texture) {
    (void)text;
    (void)font;
    (void)color;
    (void)texture;
    function_called();
    return mock_type(int);
}
