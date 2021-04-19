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
#include "tank.h"
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
    SDL_Point start = {.x = 100, .y = 1};
    Tank_Create("Nik", start);
    // 5 Sekunden lang simulieren
    for (int i = 0; i < 60 * 120; ++i) {
        EntityHandler_Update(NULL);
        // Stelle die AABBs visuell dar
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
