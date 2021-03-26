/**
 * @file sdlWrapper.c
 * @author Stoll Simon (stols4@bfh.ch)
 * @brief Wrapper der SDL Bibliothek
 * @version 0.0
 * @date 2021-03-19
 *
 * @copyright Copyright (c) 2021 Stoll Simon
 *
 */


/*
 * Includes
 *
 */

#include <SDL_image.h>

#include "sdlWrapper.h"

/*
 * Implementation öfentlicher Funktionen
 *
 */

int SDLW_Init(int windowWidth, int windowHeight) {
    (void)windowWidth;
    (void)windowHeight;
    return 0;
}

void SDLW_Quit() {

}

int SDLW_LoadResources(char *resourceConfigLocation) {
    (void)resourceConfigLocation;
    return 0;
}

int SDLW_GetTexture(char *id, SDL_Texture **texture) {
    (void)id;
    (void)texture;
    return 0;
}

int SDLW_DrawTexture(sprite_t sprite) {
    (void)sprite;
    return 0;
}

int SDLW_DrawFilledRect(SDL_Rect rect, SDL_Color color) {
    (void)rect;
    (void)color;
    return 0;
}

int SDLW_CreateTextTexture(char *text, char *font, SDL_Color color, SDL_Texture **texture) {
    (void)text;
    (void)font;
    (void)color;
    (void)texture;
    return 0;
}
