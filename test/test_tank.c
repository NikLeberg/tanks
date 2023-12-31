/**
 * @file test_tank.c
 * @author Leuenberger Niklaus (leuen4@bfh.ch)
 * @brief Tests für Tank- und Shell Entität
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

/**
 * @brief Testumgebung um visuell das Verhalten von Panzer und Schuss zu testen.
 * 
 * @param state unbenutzt
 * 
 */
static void tank_in_world(void **state) {
    (void)*state;
    // Test kann nicht in der Gitlab-Pipeline laufen, denn es wird ein Audiogerät
    // und Hardwarebeschleunigung benötigt.
#ifdef CI_TEST
    skip();
#endif
    int ret = ERR_OK;
    ret |= SDLW_Init(1024, 576);
    ret |= World_Init();
    ret |= SDLW_LoadResources("assets/config.cfg");
    ret |= World_Load("aquaduct");
    assert_int_equal(ret, ERR_OK);
    entity_t *tankA;
    player_t playerA = {"Nik", 100, PLAYER_STEP_START};
    Tank_Create(&tankA, &playerA, 500.0f, 400.0f);
    entity_t *tankB;
    player_t playerB = {"Angelo", 100, PLAYER_STEP_START};
    Tank_Create(&tankB, &playerB, 200.0f, 200.0f);
    inputEvent_t input = {.currentPlayer = &playerA};
    // 20 Sekunden lang simulieren
    for (int i = 0; i < 20 * 60; ++i) {
        // Aktiver Spieler umschalten
        if (PLAYER_STEP_DONE == input.currentPlayer->step) {
            input.currentPlayer = (input.currentPlayer == &playerA) ? &playerB : &playerA;
            input.currentPlayer->step = PLAYER_STEP_START;
        }
        SDL_Event e = {0};
        SDL_PollEvent(&e);
        input.axisWASD.x = 0;
        input.axisWASD.y = 0;
        input.lastKey = 0;
        input.currentChar = 0;
        if (e.type == SDL_KEYDOWN) {
            // Geschwindigkeit gemäss Pfeiltasten verändern
            if (e.key.keysym.sym == SDLK_a) {
                input.axisWASD.x = -1;
            } else if (e.key.keysym.sym == SDLK_d) {
                input.axisWASD.x = 1;
            } else if (e.key.keysym.sym == SDLK_w) {
                input.axisWASD.y = -1;
            } else if (e.key.keysym.sym == SDLK_s) {
                input.axisWASD.y = 1;
            } else if (e.key.keysym.sym == SDLK_SPACE) {
                input.currentChar = ' ';
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
    Tank_Destroy(tankA);
    Tank_Destroy(tankB);
    EntityHandler_RemoveAllEntities();
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
