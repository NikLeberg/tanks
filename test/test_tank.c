/**
 * @file test_tank.c
 * @author Leuenberger Niklaus (leuen4@bfh.ch)
 * @brief Tests für Tank-Entität
 * @version 0.1
 * @date 2021-04-18
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

#include <math.h>

#include "sdlWrapper.h"
#include "error.h"
#include "entities/tank.h"
#include "entityHandler.h"


/*
 * Tests
 * 
 */

static void tank_in_world(void **state) {
    (void) *state;
    int ret = ERR_OK;
    ret |= SDLW_Init(1024, 576);
    ret |= World_Init();
    ret |= SDLW_LoadResources("assets/test/config.cfg");
    ret |= SDLW_LoadResources("assets/config.cfg");
    ret |= World_Load("world");
    Tank_Create("Nik", 500.0f, 400.0f);
    Tank_Create("Other", 100.0f, 100.0f);
    // 60 Sekunden lang simulieren
    for (int i = 0; i < 60 * 60; ++i) {
        inputEvent_t input = {0};
        SDL_Event e = {0};
        SDL_PollEvent(&e);
        if (e.type == SDL_KEYDOWN) {
            // Geschwindigkeit gemäss Pfeiltasten verändern
            if (e.key.keysym.sym == SDLK_LEFT) {
                input.dummy = -1;
            } else if (e.key.keysym.sym == SDLK_RIGHT) {
                input.dummy = 1;
            } else if (e.key.keysym.sym == SDLK_UP) {
                input.dummy = -2;
            } else if (e.key.keysym.sym == SDLK_DOWN) {
                input.dummy = 2;
            } else if (e.key.keysym.sym == SDLK_SPACE) {
                input.dummy = 4;
            }
        }
        EntityHandler_Update(&input);
        SDL_Color black = {.r = 255, .g = 255, .b = 255};
        SDLW_Clear(black);
        World_DrawBackground();
        World_DrawForeground();
        EntityHandler_Draw();
        SDLW_Render();
    }
    World_Quit();
    SDLW_Quit();
}


/**
 * @brief Testprogramm
 * 
 * @return int Anzahl fehlgeschlagener Tests
 */
int main(void) {
    const struct CMUnitTest tank[] = {
        cmocka_unit_test(tank_in_world),
    };
    return cmocka_run_group_tests(tank, NULL, NULL);
}
