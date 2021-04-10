/**
 * @file mock_sdl.c
 * @author Leuenberger Niklaus (leuen4@bfh.ch)
 * @brief Mocks einiger SDL Funktionen
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
 * @brief Mock-Ersatz für originales Mix_OpenAudio()
 * 
 * @param frequency unbenutzt
 * @param format unbenutzt
 * @param channels unbenutzt
 * @param chunksize unbenutzt
 * 
 * @return immer 0
 */
int Mix_OpenAudio(int frequency, void *format, int channels, int chunksize) {
    (void)frequency;
    (void)format;
    (void)channels;
    (void)chunksize;
    return 0;
}

/**
 * @brief Mock-Ersatz für originales Mix_Init()
 * 
 * @param flags unbenutzt
 * 
 * @return immer 1
 */
int Mix_Init(int flags) {
    (void)flags;
    return 1;
}

/**
 * @brief Mock-Ersatz für originales IMG_Init()
 * 
 * @param flags unbenutzt
 * 
 * @return immer 1
 */
int IMG_Init(int flags) {
    (void)flags;
    return 1;
}

/**
 * @brief Mock-Ersatz für originales IMG_LoadTexture()
 * 
 * @param renderer unbenutzt
 * @param file unbenutzt
 * 
 * @return immer 1
 */
void *IMG_LoadTexture(void *renderer, const char *file) {
    (void)renderer;
    (void)file;
    return (void *)1;
}

/**
 * @brief Mock-Ersatz für originales SDL_DestroyTexture()
 * 
 * @param texture unbenutzt
 */
void SDL_DestroyTexture(void *texture) {
    (void)texture;
}

/**
 * @brief Mock-Ersatz für originales SDL_SetTextureBlendMode()
 * 
 * @param texture unbenutzt
 * @param blendmode unbenutzt
 * 
 * @return immer 0
 */
int SDL_SetTextureBlendMode(void *texture, int blendmode) {
    (void)texture;
    (void)blendmode;
    return 0;
}

/**
 * @brief Mock-Ersatz für originales SDL_CreateRenderer()
 * 
 * @param window unbenutzt
 * @param index unbenutzt
 * @param flags unbenutzt
 * 
 * @return immer 1
 */
void *SDL_CreateRenderer(void *window, int index, unsigned int flags) {
    (void)window;
    (void)index;
    (void)flags;
    return (void *)1;
}

/**
 * @brief Mock-Ersatz für originales SDL_DestroyRenderer()
 * 
 * @param renderer unbenutzt
 */
void SDL_DestroyRenderer(void *renderer) {
    (void)renderer;
}

/**
 * @brief Mock-Ersatz für originales SDL_SetRenderDrawColor()
 * 
 * @param renderer unbenutzt
 * @param r unbenutzt
 * @param g unbenutzt
 * @param b unbenutzt
 * @param a unbenutzt
 * 
 * @return immer 0
 */
int SDL_SetRenderDrawColor(void *renderer, int r, int g, int b, int a) {
    (void)renderer;
    (void)r;
    (void)g;
    (void)b;
    (void)a;
    return 0;
}

/**
 * @brief Mock-Ersatz für originales SDL_RenderClear()
 * 
 * @param renderer unbenutzt
 * 
 * @return immer 0
 */
int SDL_RenderClear(void *renderer) {
    (void)renderer;
    return 0;
}

/**
 * @brief Mock-Ersatz für originales SDL_RenderPresent()
 * 
 * @param renderer unbenutzt
 */
void SDL_RenderPresent(void *renderer) {
    (void)renderer;
}

/**
 * @brief Mock-Ersatz für originales SDL_RenderFillRect()
 * 
 * @param renderer unbenutzt
 * @param rect unbenutzt
 * 
 * @return immer 0
 */
int SDL_RenderFillRect(void *renderer, const void *rect) {
    (void)renderer;
    (void)rect;
    return 0;
}

/**
 * @brief Mock-Ersatz für originales SDL_RenderCopyEx()
 * 
 * @param renderer unbenutzt
 * @param texture unbenutzt
 * @param srcrect unbenutzt
 * @param dstrect unbenutzt
 * @param angle unbenutzt
 * @param center unbenutzt
 * @param flip unbenutzt
 * 
 * @return immer 0
 */
int SDL_RenderCopyEx(void *renderer, void *texture, const void *srcrect, const void *dstrect, const double angle, const void *center, const int flip) {
    (void)renderer;
    (void)texture;
    (void)srcrect;
    (void)dstrect;
    (void)angle;
    (void)center;
    (void)flip;
    return 0;
}

/**
 * @brief Mock-Ersatz für originales Mix_PlayMusic()
 * 
 * @param music unbenutzt
 * @param loops unbenutzt
 * 
 * @return immer 0
 */
int Mix_PlayMusic(void *music, int loops) {
    (void)music;
    (void)loops;
    return 0;
}

/**
 * @brief Mock-Ersatz für originales Mix_PlayChannelTimed()
 * 
 * @param channel unbenutzt
 * @param chunk unbenutzt
 * @param loops unbenutzt
 * @param ticks unbenutzt
 * 
 * @return immer 0
 */
int Mix_PlayChannelTimed(int channel, void *chunk, int loops, int ticks) {
    (void)channel;
    (void)chunk;
    (void)loops;
    (void)ticks;
    return 0;
}

/**
 * @brief Mock-Ersatz für originales SDL_CreateTextureFromSurface()
 * 
 * @param renderer unbenutzt
 * @param surface unbenutzt
 * 
 * @return immer 1
 */
void *SDL_CreateTextureFromSurface(void *renderer, void *surface) {
    (void)renderer;
    (void)surface;
    return (void *)1;
}

/**
 * @brief Mock-Ersatz für originales Mix_LoadMUS()
 * 
 * @param file unbenutzt
 * 
 * @return immer 1
 */
void *Mix_LoadMUS(const char *file) {
    (void)file;
    return (void *)1;
}

/**
 * @brief Mock-Ersatz für originales Mix_LoadWAV_RW()
 * 
 * @param src unbenutzt
 * @param freesrc unbenutzt
 * 
 * @return immer 1
 */
void *Mix_LoadWAV_RW(void *src, int freesrc) {
    (void)freesrc;
    (void)src;
    return (void *)1;
}

/**
 * @brief Mock-Ersatz für originales Mix_FreeMusic()
 * 
 * @param music unbenutzt
 */
void Mix_FreeMusic(void *music) {
    (void)music;
}

/**
 * @brief Mock-Ersatz für originales Mix_FreeChunk()
 * 
 * @param chunk unbenutzt
 */
void Mix_FreeChunk(void *chunk) {
    (void)chunk;
}

/**
 * @brief Mock-Ersatz für originales SDL_QueryTexture()
 * 
 * @param texture unbenutzt
 * @param format unbenutzt
 * @param access unbenutzt
 * @param w unbenutzt
 * @param h unbenutzt
 * 
 * @return immer 0
 */
int SDL_QueryTexture(void *texture, int *format, int *access, int *w, int *h) {
    (*w) = 100;
    (*h) = 100;
    (void)access;
    (void)format;
    (void)texture;
    return 0;
}
