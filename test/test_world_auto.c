/**
 * @file test_world_auto.c
 * @author Stoll Simon (stols4@bfh.ch)
 * @brief Automatische Tests für world-Modul
 * @version 0.1
 * @date 2021-04-12
 * 
 * @copyright Copyright (c) 2021 Stoll Simon
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

#include "error.h"
#include "sdlWrapper.h"
#include "world.h"


/**
 * @brief Überprüft die Initialisierung und die Erkennung ob World nicht initialisiert wurde.
 * 
 * @param state unbenutzt
 */
void test_world_init_and_load(void **state) {
    (void)state;
    assert_int_equal(World_Init(), ERR_FAIL);
    assert_int_equal(SDLW_Init(1024, 576), ERR_OK);
    assert_int_equal(World_Init(), ERR_OK);
    // Error Welt nicht geladen
    assert_int_equal(World_DrawBackground(), ERR_FAIL);
    assert_int_equal(World_DrawForeground(), ERR_FAIL);
    assert_int_equal(World_CheckCollision((SDL_Rect){0}, (void *)1), ERR_FAIL);
    assert_int_equal(World_Modify((sprite_t){0}), ERR_FAIL);
    assert_int_equal(World_VerticalLineIntersection((SDL_Point){0}, (void *)1), ERR_FAIL);
    assert_int_equal(World_CreateStartLocations((SDL_Rect){0}, 0, (void *)1), ERR_FAIL);

    // Welt laden Fehler
    assert_int_equal(World_Load(NULL), ERR_NULLPARAMETER);
    assert_int_equal(World_Load("asdf"), ERR_FAIL);
    assert_int_equal(World_Load("world"), ERR_FAIL);
    // Laden einer Welt ohne gültige Musik
    assert_int_equal(SDLW_LoadResources("assets/test/config.cfg"), ERR_OK);
    assert_int_equal(World_Load("worldse"), ERR_FAIL);
    // Welt laden OK
    assert_int_equal(World_Load("worlds"), ERR_OK);
    assert_int_equal(World_Load("world"), ERR_OK);

    // Error Nullparamter
    assert_int_equal(World_CheckCollision((SDL_Rect){0}, NULL), ERR_NULLPARAMETER);
    assert_int_equal(World_VerticalLineIntersection((SDL_Point){0}, NULL), ERR_NULLPARAMETER);
    assert_int_equal(World_CreateStartLocations((SDL_Rect){0}, 0, NULL), ERR_NULLPARAMETER);

    // Funktionsaufrufe OK
    SDL_Point point;
    entityCollision_t info;
    assert_int_equal(World_DrawBackground(), ERR_OK);
    assert_int_equal(World_DrawForeground(), ERR_OK);
    assert_int_equal(World_CheckCollision((SDL_Rect){0}, &info), ERR_OK);
    assert_int_equal(World_Modify((sprite_t){0}), ERR_OK);
    assert_int_equal(World_VerticalLineIntersection((SDL_Point){0}, &point), ERR_OK);
    assert_int_equal(World_CreateStartLocations((SDL_Rect){0}, 0, (void *)1), ERR_OK);

    World_Quit();
    SDLW_Quit();
}

/**
 * @brief Überprüft ob die Kollisionserkennung richtig funktioniert und ob die Hitnormal und Hitflag richtig ausgegeben wird.
 * 
 * @param state unbenutzt
 */
static void test_checkCollision(void **state) {
    (void)state;
    SDL_Rect aabb;
    entityCollision_t info;

    assert_int_equal(SDLW_Init(1024, 576), ERR_OK);
    assert_int_equal(World_Init(), ERR_OK);
    assert_int_equal(SDLW_LoadResources("assets/test/config.cfg"), ERR_OK);
    assert_int_equal(World_Load("world"), ERR_OK);

    aabb = (SDL_Rect){-50, -50, 100, 100}; // Links oben
    assert_int_equal(World_CheckCollision(aabb, &info), ERR_OK);
    assert_int_equal(info.flags, ENTITY_COLLISION_WORLD | ENTITY_COLLISION_BORDER_LEFT | ENTITY_COLLISION_BORDER_TOP);
    assert_int_equal(info.normal.x, -2450);
    assert_int_equal(info.normal.y, -2450);
    aabb = (SDL_Rect){-50, 576 - 50, 100, 100}; // Links unten
    assert_int_equal(World_CheckCollision(aabb, &info), ERR_OK);
    assert_int_equal(info.flags, ENTITY_COLLISION_WORLD | ENTITY_COLLISION_BORDER_LEFT | ENTITY_COLLISION_BORDER_BOTTOM);
    assert_int_equal(info.normal.x, -2450);
    assert_int_equal(info.normal.y, 2450);
    aabb = (SDL_Rect){1024 - 50, -50, 100, 100}; // Rechts oben
    assert_int_equal(World_CheckCollision(aabb, &info), ERR_OK);
    assert_int_equal(info.flags, ENTITY_COLLISION_BORDER_RIGHT | ENTITY_COLLISION_BORDER_TOP);
    assert_int_equal(info.normal.x, 0);
    assert_int_equal(info.normal.y, 0);
    aabb = (SDL_Rect){1024 - 50, 576 - 50, 100, 100}; // Rechts unten
    assert_int_equal(World_CheckCollision(aabb, &info), ERR_OK);
    assert_int_equal(info.flags, ENTITY_COLLISION_BORDER_RIGHT | ENTITY_COLLISION_BORDER_BOTTOM);
    assert_int_equal(info.normal.x, 0);
    assert_int_equal(info.normal.y, 0);
    aabb = (SDL_Rect){100 + 576 / 2 - 50, 576 / 2 - 50, 100, 100};
    assert_int_equal(World_CheckCollision(aabb, &info), ERR_OK);
    assert_int_equal(info.flags, ENTITY_COLLISION_WORLD);
    assert_int_equal(info.normal.x, 2499);
    assert_int_equal(info.normal.y, -2499);

    World_Quit();
    SDLW_Quit();
}

/**
 * @brief Testet ob die Funktion World_VerticalLineIntersection() die richtigen Werte zurückgibt.
 * 
 * @param state unbenutzt
 */
static void test_verticalLineIntersection(void **state) {
    (void)state;
    assert_int_equal(SDLW_Init(1024, 576), ERR_OK);
    assert_int_equal(World_Init(), ERR_OK);
    assert_int_equal(SDLW_LoadResources("assets/test/config.cfg"), ERR_OK);
    assert_int_equal(World_Load("world"), ERR_OK);

    SDL_Point position;

    // Out of Bounds
    assert_int_equal(World_VerticalLineIntersection((SDL_Point){-1, 0}, &position), ERR_PARAMETER);
    assert_int_equal(World_VerticalLineIntersection((SDL_Point){1024, 0}, &position), ERR_PARAMETER);

    // Linie von ganz oben
    assert_int_equal(World_VerticalLineIntersection((SDL_Point){0, 0}, &position), ERR_OK);
    assert_int_equal(position.x, 0);
    assert_int_equal(position.y, 0);
    assert_int_equal(World_VerticalLineIntersection((SDL_Point){100 + 576 / 2, 0}, &position), ERR_OK);
    assert_int_equal(position.x, 388);
    assert_int_equal(position.y, 289);
    assert_int_equal(World_VerticalLineIntersection((SDL_Point){1023, 0}, &position), ERR_OK);
    assert_int_equal(position.x, 1023);
    assert_int_equal(position.y, 576);

    // Linie von bisschen über der Mitte
    assert_int_equal(World_VerticalLineIntersection((SDL_Point){0, 576 / 2 - 50}, &position), ERR_OK);
    assert_int_equal(position.x, 0);
    assert_int_equal(position.y, 238);
    assert_int_equal(World_VerticalLineIntersection((SDL_Point){100 + 576 / 2, 576 / 2 - 50}, &position), ERR_OK);
    assert_int_equal(position.x, 388);
    assert_int_equal(position.y, 289);
    assert_int_equal(World_VerticalLineIntersection((SDL_Point){1023, 576 / 2 - 50}, &position), ERR_OK);
    assert_int_equal(position.x, 1023);
    assert_int_equal(position.y, 576);

    World_Quit();
    SDLW_Quit();
}

/**
 * @brief Testet ob die Funktion World_CreateStartLocations() die richtigen Werte zurückgibt.
 * 
 * @param state unbenutzt
 */
static void test_createStartLocations(void **state) {
    (void)state;
    // Initialisierung
    assert_int_equal(SDLW_Init(1024, 576), ERR_OK);
    assert_int_equal(World_Init(), ERR_OK);
    assert_int_equal(SDLW_LoadResources("assets/test/config.cfg"), ERR_OK);
    assert_int_equal(World_Load("world"), ERR_OK);

    SDL_Point locations[5];

    // 0 Startpunkte, kein Fehler
    assert_int_equal(World_CreateStartLocations((SDL_Rect){-35, 0, 50, 50}, 0, locations), ERR_OK);

    // 1 Startpunkt, in der Mitte der Welt
    assert_int_equal(World_CreateStartLocations((SDL_Rect){-35, 0, 50, 50}, 1, locations), ERR_OK);
    assert_int_equal(locations[0].x, 512);
    assert_int_equal(locations[0].y, 328);

    // 2 Startpunkte, Welt wurde gedrittelt
    assert_int_equal(World_CreateStartLocations((SDL_Rect){-35, 0, 50, 50}, 2, locations), ERR_OK);
    assert_int_equal(locations[0].x, 341);
    assert_int_equal(locations[0].y, 157);
    assert_int_equal(locations[1].x, 682);
    assert_int_equal(locations[1].y, 498);

    // 3 Startpunkte, Welt wurde geviertelt
    assert_int_equal(World_CreateStartLocations((SDL_Rect){-35, 0, 50, 50}, 3, locations), ERR_OK);
    assert_int_equal(locations[0].x, 256);
    assert_int_equal(locations[0].y, 72);
    assert_int_equal(locations[1].x, 512);
    assert_int_equal(locations[1].y, 328);
    assert_int_equal(locations[2].x, 768);
    assert_int_equal(locations[2].y, 575);

    // 5 Startpunkte, der Erste Startpunkt ist über der Welt
    assert_int_equal(World_CreateStartLocations((SDL_Rect){-35, 0, 50, 50}, 5, locations), ERR_OK);
    assert_int_equal(locations[0].x, 170);
    assert_int_equal(locations[0].y, -1);
    assert_int_equal(locations[1].x, 340);
    assert_int_equal(locations[1].y, 156);
    assert_int_equal(locations[2].x, 510);
    assert_int_equal(locations[2].y, 326);
    assert_int_equal(locations[3].x, 680);
    assert_int_equal(locations[3].y, 496);
    assert_int_equal(locations[4].x, 850);
    assert_int_equal(locations[4].y, 575);

    World_Quit();
    SDLW_Quit();
}

/**
 * @brief Testprogramm
 * 
 * @return int Anzahl fehlgeschlagener Tests
 */
int main(void) {
    const struct CMUnitTest worldAutoTest[] = {
        cmocka_unit_test(test_world_init_and_load),
        cmocka_unit_test(test_checkCollision),
        cmocka_unit_test(test_verticalLineIntersection),
        cmocka_unit_test(test_createStartLocations)};
    return cmocka_run_group_tests(worldAutoTest, NULL, NULL);
}
