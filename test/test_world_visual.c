/**
 * @file test_world_visual.c
 * @author Stoll Simon (stols4@bfh.ch)
 * @brief Visuelle Tests für world-Modul
 * @version 0.1
 * @date 2021-04-12
 * 
 * @copyright Copyright (c) 2021 Stoll Simon
 *
 * Überprüft die Zerstörung und Erstellung der Welt.
 * Die Ausgabe der Hitnormal kann am Ende mit der Maus überprüft werden, ob sie
 * so ausssieht wie erwartet.
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
#include "world.h"
#include "stdio.h"

/**
 * @brief Zeichnet die Welt und wartet 1s.
 * 
 */
static void drawWorld() {
    SDLW_Clear((SDL_Color){0});
    World_DrawBackground();
    World_DrawForeground();
    SDLW_Render();
    SDL_Delay(1000);
}

/**
 * @brief Testet das Zeichnen der Welt und die Ausgabe der Hitnormal
 * 
 * @param state Unbenutzt.
 */
static void test_world_draw(void **state) {
    (void)state;
#ifdef CI_TEST // SDLW kann nicht mit Gitlab test laufen, das weder audio noch hardware accelerated funktioniert
    skip();
#endif
    // Initialisierung
    SDLW_Init(1024, 576);
    SDLW_LoadResources("assets/test/config.cfg");
    SDLW_LoadResources("assets/world/config.cfg");
    World_Init();
    World_Load("aquaduct");

    // Resourcen holen
    sprite_t mask = {0};
    sprite_t stamp = {0};
    sprite_t *getter;
    SDLW_GetResource("mask", RESOURCETYPE_SPRITE, (void **)&getter);
    mask = *getter;
    SDLW_GetResource("stamp", RESOURCETYPE_SPRITE, (void **)&getter);
    stamp = *getter;

    // Stamp und Mask testen
    drawWorld(); // Alles grün
    mask.position = (SDL_Point){350, 300};
    World_Modify(mask);
    drawWorld(); // Ein blaues loch
    mask.position = (SDL_Point){450, 300};
    World_Modify(mask);
    drawWorld(); // Zwei blaue Löcher
    stamp.position = (SDL_Point){400, 300};
    World_Modify(stamp);
    drawWorld(); // Weisser Kreis hinter den blauen Löcher
    // Prüft das Automatische setzen von Panzerpositionen
    SDL_Rect aabb = (SDL_Rect){-25, 12, 50, 25};
    SDL_Point points[8];
    // Durchlaufen von setzen von positionen 1 - 8
    for (int c = 1; c <= 8; c++) {
        World_CreateStartLocations(aabb, c, points); // Erstellen von n Positionen
        SDLW_Clear((SDL_Color){0});
        World_DrawBackground();
        World_DrawForeground();
        for (int i = 0; i < c; i++) { // Zeichnet die Panzerpositionen als schwarze rechtecke
            SDLW_DrawFilledRect((SDL_Rect){aabb.x + points[i].x, aabb.y + points[i].y, aabb.w, aabb.h}, (SDL_Color){0, 0, 0, 255});
        }
        SDLW_Render();
        SDL_Delay(1000);
    }

    // Hitnormal vektor test
    int running = 1;
    int x, y;
    SDL_Event event;
    SDL_Renderer *renderer = SDLW_GetRenderer();
    char text[256] = {0};
    entityCollision_t info;
    sprite_t textSprite = {0};


    while (running) { // Schleife, damit die Mausposition aktualisiert werden keann
        SDL_GetMouseState(&x, &y);
        aabb = (SDL_Rect){x - 25, y - 25, 50, 50};    // Setzen des zu überprüfenden AABBs
        textSprite.position = (SDL_Point){x, y - 50}; // Setzen der Position des Textes

        if (textSprite.texture) // Zerstört die vorhergehige Text Textur
            SDL_DestroyTexture(textSprite.texture);
        // Numerischer Text erstellen
        World_CheckCollision(aabb, &info);
        sprintf(text, "(%d,%d)", (int)info.normal.x, (int)info.normal.y);
        Sprite_CreateText(text, "osans25", (SDL_Color){0, 0, 0, 255}, &textSprite);

        SDLW_Clear((SDL_Color){0});
        //Welt zeichnen
        World_DrawBackground();
        World_DrawForeground();
        //AABB zeichnen
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        SDL_RenderDrawRect(renderer, &aabb);
        // Normale zeichnen
        SDL_RenderDrawLine(renderer, x, y, x + (int)info.normal.x / 10, y + (int)info.normal.y / 10);
        // Numerischer Text normal
        SDLW_DrawTexture(textSprite);

        SDLW_Render();

        while (SDL_PollEvent(&event)) // Quit event lesen
            if (event.type == SDL_QUIT)
                running = 0;
    }
    if (textSprite.texture) // Zerstört den Text
        SDL_DestroyTexture(textSprite.texture);
    World_Quit();
    SDLW_Quit();
}

/**
 * @brief Testprogramm
 * 
 * @return int Anzahl fehlgeschlagener Tests
 */
int main(void) {
    const struct CMUnitTest worldVisualTest[] = {
        cmocka_unit_test(test_world_draw)
    };
    return cmocka_run_group_tests(worldVisualTest, NULL, NULL);
}
