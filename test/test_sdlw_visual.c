/**
 * @file test_sdlw_visual.c
 * @author Stoll Simon (stols4@bfh.ch)
 * @brief Visuelle Tests für sdlWrapper-Modul
 * @version 0.1
 * @date 2021-04-09
 * 
 * @copyright Copyright (c) 2021 Stoll Simon
 *
 * Die Tests sollten optimalerweise in kleinere und isoliertere Einzeltests
 * aufgesplittet werden. Allerdings ist die Funktion \ref SDLW_Quit() resp. das
 * dahinter liegende SDL_Quit() sehr langsam (~5 Sekunden). Daher werden "nur"
 * drei einzelne dafür aber grössere Tests gefahren.
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

#include "sdlWrapper.h"
#include "error.h"


/*
 * Hilfsfunktionen
 * 
 */

/**
 * @brief Zeichnet ein Grid mit Abstand von 100 Pixel.
 *
 */
static void draw_grid() {
    for (int x = 1; x < 8; x++) {
        SDLW_DrawFilledRect((SDL_Rect){x * 100 - 1, 0, 2, 600}, (SDL_Color){0, 0, 0, 255});
    }
    for (int y = 1; y < 6; y++) {
        SDLW_DrawFilledRect((SDL_Rect){0, y * 100 - 1, 800, 2}, (SDL_Color){0, 0, 0, 255});
    }
}

/**
 * @brief Zeichnet zuerst das Grid und dann eine Textur.
 * 
 * @param sprite zu zeichnende Textur / Sprite
 * 
 */
static void draw_textureOnGrid(sprite_t sprite) {
    SDLW_Clear((SDL_Color){255, 255, 255, 255});
    draw_grid();
    SDLW_DrawTexture(sprite);
    SDLW_Render();
}


/*
 * Tests
 * 
 */

/**
 * @brief Zeichnet Sprites auf verschiedene Arten und Weisen.
 * 
 * @param state unbenutzt
 */
static void test_drawSprite(void **state) {
    (void)state;

// SDLW kann nicht in der Gitlab-Pipeline laufen, denn es wird ein Audiogerät
// und Hardwarebeschleunigung bnötigt.
#ifdef CI_TEST
    skip();
#endif

    //Initialisierung
    SDLW_Init(800, 600);
    SDLW_LoadResources("assets/test/config.cfg");

    sprite_t testRect;
    sprite_t *getter;
    SDLW_GetResource("rect", RESOURCETYPE_SPRITE, (void **)&getter);
    testRect = *getter;
    testRect.position = (SDL_Point){400, 300};

    // Position und Offsetüberprüfung
    for (int y = 0; y < 100; y++) {
        testRect.position.y--;
        draw_textureOnGrid(testRect);
    }
    for (int x = 0; x < 100; x++) {
        testRect.position.x++;
        draw_textureOnGrid(testRect);
    }
    for (int y = 0; y < 100; y++) {
        testRect.destination.y++;
        draw_textureOnGrid(testRect);
    }
    for (int x = 0; x < 100; x++) {
        testRect.destination.x--;
        draw_textureOnGrid(testRect);
    }
    for (int r = 0; r < 90; r++) {
        testRect.rotation++;
        draw_textureOnGrid(testRect);
    }
    //Rotations und Pivot überprüfung
    testRect.rotation = 0;
    testRect.pivot = (SDL_Point){-50, 50};
    for (int r = 0; r < 90; r++) {
        testRect.rotation++;
        draw_textureOnGrid(testRect);
    }
    testRect.position = (SDL_Point){400, 300};
    testRect.destination.x = 0;
    testRect.destination.y = 0;
    for (int r = 1; r <= 90; r++) {
        sprite_t rotated;
        Sprite_SetRelativeToPivot(testRect, r, (SDL_Point){50, -50}, &rotated);
        draw_textureOnGrid(rotated);
    }

    // Multisprite überprüfung
    SDLW_GetResource("animation", RESOURCETYPE_SPRITE, (void **)&getter);
    testRect = *getter;
    SDLW_Clear((SDL_Color){255, 255, 255, 255});
    draw_grid();
    for (int y = 0; y < 3; y++) {
        for (int x = 0; x < 3; x++) {
            testRect.position = (SDL_Point){x * 50 + 350, y * 50 + 250};
            SDLW_DrawTexture(testRect);
            Sprite_NextFrame(&testRect);
        }
    }
    testRect.position = (SDL_Point){500, 300};
    SDLW_DrawTexture(testRect);
    SDLW_Render();
    SDL_Delay(1000);

    SDLW_Quit();
}

/**
 * @brief Spielt Musik ab.
 * 
 * @param state unbenutzt
 */
static void test_playSound(void **state) {
    (void)state;

// SDLW kann nicht in der Gitlab-Pipeline laufen, denn es wird ein Audiogerät
// und Hardwarebeschleunigung bnötigt.
#ifdef CI_TEST
    skip();
#endif

    SDLW_Init(800, 600);
    SDLW_LoadResources("assets/test/config.cfg");

    SDLW_PlaySoundEffect("peep");
    SDL_Delay(2000);
    SDLW_PlayMusic("jingle");
    SDL_Delay(2000);

    SDLW_Quit();
}

/**
 * @brief Zeigt einen Text an.
 * 
 * @param state unbenutzt
 */
static void test_drawText(void **state) {
    (void)state;

// SDLW kann nicht in der Gitlab-Pipeline laufen, denn es wird ein Audiogerät
// und Hardwarebeschleunigung bnötigt.
#ifdef CI_TEST
    skip();
#endif

    // Initialisierung
    SDLW_Init(800, 600);
    SDLW_LoadResources("assets/test/config.cfg");

    sprite_t testRect = {0};
    testRect.position = (SDL_Point){400, 300};
    Sprite_CreateText("Deus ex machina", "osans25", (SDL_Color){0, 0, 0, 255}, &testRect);

    // Zeichnen
    SDLW_Clear((SDL_Color){255, 255, 255, 255});
    SDLW_DrawTexture(testRect);
    SDLW_Render();
    SDL_Delay(2000);

    //Abschluss
    SDL_DestroyTexture(testRect.texture);
    SDLW_Quit();
}

/**
 * @brief Testprogramm
 * 
 * @return int Anzahl fehlgeschlagener Tests
 */
int main(void) {
    const struct CMUnitTest sdlwVisualTest[] = {
        cmocka_unit_test(test_drawSprite),
        cmocka_unit_test(test_playSound),
        cmocka_unit_test(test_drawText)
    };
    return cmocka_run_group_tests(sdlwVisualTest, NULL, NULL);
}
