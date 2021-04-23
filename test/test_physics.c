/**
 * @file test_physics.c
 * @author Leuenberger Niklaus (leuen4@bfh.ch)
 * @brief Tests für Physics-Modul
 * @version 0.1
 * @date 2021-04-13
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
#include "physics.h"


/*
 * Mocks
 * 
 */

/**
 * @brief Das echte \ref World_CheckCollision().
 * 
 * @param[in] aabb Die AABB-Kollisionsbox
 * @param[out] collision Die Kollisionsdaten
 * 
 * @return gemäss Implementation \ref world.c
 */
extern int __real_World_CheckCollision(SDL_Rect aabb, entityCollision_t *collision);

/**
 * @brief Mock-Ersatz für originales \ref World_CheckCollision().
 * 
 * Einige Tests sollten unabhängig von einer geladenen Welt funktionieren.
 * Andere testen die Integration mit der Welt. Daher ist es notwendig diese
 * Funktion nicht als eigenständige Mock-Datei wie z.B. \ref mock_sdlw.c zu
 * ersetzen. Denn damit könnte die originale Funktion nicht aufgerufen werden.
 * @note Wird die echte Funktion benötigt, sollte per will_return eine 1 gesetzt
 * werden.
 * 
 * @param[in] aabb Die AABB-Kollisionsbox
 * @param[out] collision Die Kollisionsdaten
 *
 * @return immer 0 falls gemockt, sonst den Fehlercode von der echten Funktion
 */
int __wrap_World_CheckCollision(SDL_Rect aabb, entityCollision_t *collision) {
    if (mock_type(int)) {
        return __real_World_CheckCollision(aabb, collision);
    } else {
        return ERR_OK;
    }
}


/*
 * Tests
 * 
 */

#define EPSILON 0.001f //!< Fehlerschranke für Fliesskommavergleich

/**
 * @brief Setup: Entität erstellen und hinzufügen
 * 
 * @param[out] state Pointer auf entity_t
 * 
 * @return 0 Setup erfolgreich
 */
static int setupOneEntity(void **state) {
    static entity_t entity;
    entity_t entityOk = {0};
    entity = entityOk;
    *state = (void *)&entity;
    return 0;
}

/**
 * @brief Setter der Position funtionieren.
 * 
 * @param state Pointer auf entity_t
 */
static void physics_can_set_position_of_entity(void **state) {
    entity_t *entity = (entity_t *)*state;
    // Entität ist NULL -> Parameterfehler
    assert_int_equal(Physics_SetPosition(NULL, 0.0f, 0.0f), ERR_PARAMETER);
    assert_int_equal(Physics_SetRelativePosition(NULL, 0.0f, 0.0f), ERR_PARAMETER);
    // Teste verschiedene Zahlenpaare
    struct {
        int relative; // 0 = nutze SetPosition, 1 = nutze SetRelativePosition
        SDL_FPoint given; // welcher Wert der Funktion übergeben wird
        SDL_FPoint expected; // welcher Wert die Position nun haben sollte
    } testPoints[] = {
        {.relative = 0, .given = {0.0f, 0.0f}, .expected = {0.0f, 0.0f}},
        {.relative = 0, .given = {-1.0f, -1.0f}, .expected = {-1.0f, -1.0f}},
        {.relative = 0, .given = {1234.5f, -7.6543}, .expected = {1234.5f, -7.6543}},
        {.relative = 0, .given = {INFINITY, -INFINITY}, .expected = {INFINITY, -INFINITY}},
        {.relative = 0, .given = {NAN, 3.4f}, .expected = {INFINITY, 3.4f}},
        {.relative = 0, .given = {1.2f, NAN}, .expected = {1.2f, 3.4f}},
        {.relative = 1, .given = {-1.2f, -3.4f}, .expected = {0.0f, 0.0f}},
        {.relative = 1, .given = {1.0f, 1.0f}, .expected = {1.0f, 1.0f}},
        {.relative = 1, .given = {1.0f, 1.0f}, .expected = {2.0f, 2.0f}},
        {.relative = 1, .given = {NAN, -10.0f}, .expected = {2.0f, -8.0f}},
        {.relative = 1, .given = {-11.0f, NAN}, .expected = {-9.0f, -8.0f}},
    };
    for (unsigned int i = 0; i < sizeof(testPoints) / sizeof(testPoints[0]); ++i) {
        if (testPoints[i].relative) {
            assert_int_equal(Physics_SetRelativePosition(entity, testPoints[i].given.x, testPoints[i].given.y), ERR_OK);
        } else {
            assert_int_equal(Physics_SetPosition(entity, testPoints[i].given.x, testPoints[i].given.y), ERR_OK);
        }
        assert_float_equal(entity->physics.position.x, testPoints[i].expected.x, EPSILON);
        assert_float_equal(entity->physics.position.y, testPoints[i].expected.y, EPSILON);
    }
}

/**
 * @brief Setter der Geschwindigkeit funtionieren.
 * 
 * @param state Pointer auf entity_t
 */
static void physics_can_set_velocity_of_entity(void **state) {
    entity_t *entity = (entity_t *)*state;
    // Entität ist NULL -> Parameterfehler
    assert_int_equal(Physics_SetVelocity(NULL, 0.0f, 0.0f), ERR_PARAMETER);
    assert_int_equal(Physics_SetRelativeVelocity(NULL, 0.0f, 0.0f), ERR_PARAMETER);
    // Teste verschiedene Zahlenpaare
    struct {
        int relative; // 0 = nutze SetVelocity, 1 = nutze SetRelativeVelocity
        SDL_FPoint given; // welcher Wert der Funktion übergeben wird
        SDL_FPoint expected; // welcher Wert die Geschwindigkeit nun haben sollte
    } testPoints[] = {
        {.relative = 0, .given = {0.0f, 0.0f}, .expected = {0.0f, 0.0f}},
        {.relative = 0, .given = {-1.0f, -1.0f}, .expected = {-1.0f, -1.0f}},
        {.relative = 0, .given = {1234.5f, -7.6543}, .expected = {1234.5f, -7.6543}},
        {.relative = 0, .given = {INFINITY, -INFINITY}, .expected = {INFINITY, -INFINITY}},
        {.relative = 0, .given = {NAN, 3.4f}, .expected = {INFINITY, 3.4f}},
        {.relative = 0, .given = {1.2f, NAN}, .expected = {1.2f, 3.4f}},
        {.relative = 1, .given = {-1.2f, -3.4f}, .expected = {0.0f, 0.0f}},
        {.relative = 1, .given = {1.0f, 1.0f}, .expected = {1.0f, 1.0f}},
        {.relative = 1, .given = {1.0f, 1.0f}, .expected = {2.0f, 2.0f}},
        {.relative = 1, .given = {NAN, -10.0f}, .expected = {2.0f, -8.0f}},
        {.relative = 1, .given = {-11.0f, NAN}, .expected = {-9.0f, -8.0f}},
    };
    for (unsigned int i = 0; i < sizeof(testPoints) / sizeof(testPoints[0]); ++i) {
        if (testPoints[i].relative) {
            assert_int_equal(Physics_SetRelativeVelocity(entity, testPoints[i].given.x, testPoints[i].given.y), ERR_OK);
        } else {
            assert_int_equal(Physics_SetVelocity(entity, testPoints[i].given.x, testPoints[i].given.y), ERR_OK);
        }
        assert_float_equal(entity->physics.velocity.x, testPoints[i].expected.x, EPSILON);
        assert_float_equal(entity->physics.velocity.y, testPoints[i].expected.y, EPSILON);
    }
}

/**
 * @brief Setter der Geschwindigkeit in Polarform funtionieren.
 * 
 * @param state Pointer auf entity_t
 */
static void physics_can_set_velocity_in_polar_of_entity(void **state) {
    entity_t *entity = (entity_t *)*state;
    // Entität ist NULL -> Parameterfehler
    assert_int_equal(Physics_SetVelocityPolar(NULL, 0.0f, 0.0), ERR_PARAMETER);
    assert_int_equal(Physics_SetRelativeVelocityPolar(NULL, 0.0f, 0.0), ERR_PARAMETER);
    // Teste verschiedene Zahlenpaare
    struct {
        int relative; // 0 = nutze SetVelocityPolar, 1 = nutze SetRelativeVelocityPolar
        struct {
            float v;
            double angle;
        } given; // welcher Wert der Funktion übergeben wird
        SDL_FPoint expected; // welcher Wert die Geschwindigkeit nun haben sollte
    } testPoints[] = {
        // 0.0f, 0.0 -> Ursprung
        {.relative = 0, .given = {0.0f, 0.0}, .expected = {0.0f, 0.0f}},
        // Länge 1.0 mit 0.0° -> nach rechts, horizontal
        {.relative = 0, .given = {1.0f, 0.0}, .expected = {1.0f, 0.0f}},
        // Länge 1.0 mit 90.0° -> nach unten
        {.relative = 0, .given = {1.0f, 90.0}, .expected = {0.0f, 1.0f}},
        // Länge 1.0 mit 180.0° -> nach links
        {.relative = 0, .given = {1.0f, 180.0}, .expected = {-1.0f, 0.0f}},
        // Länge 1.0 mit 270.0° -> nach oben
        {.relative = 0, .given = {1.0f, 270.0}, .expected = {0.0f, -1.0f}},
        // Länge 1.0 mit 45.0° -> nach rechts unten
        {.relative = 0, .given = {1.0f, 45.0}, .expected = {1.0f / sqrt(2.0), 1.0f / sqrt(2.0)}},
        // Relative Länge 1.0 mit 225.0° -> nach links oben
        {.relative = 1, .given = {1.0f, 225.0}, .expected = {0.0f, 0.0f}},
        // Relative Länge 1.0 mit 0.0° -> nach rechts, horizontal
        {.relative = 1, .given = {1.0f, 0.0}, .expected = {1.0f, 0.0f}},
        // Relative Länge 1.0 mit 90.0° -> nach unten
        {.relative = 1, .given = {1.0f, 90.0}, .expected = {1.0f, 1.0f}},
    };
    for (unsigned int i = 0; i < sizeof(testPoints) / sizeof(testPoints[0]); ++i) {
        if (testPoints[i].relative) {
            assert_int_equal(Physics_SetRelativeVelocityPolar(entity, testPoints[i].given.v, testPoints[i].given.angle), ERR_OK);
        } else {
            assert_int_equal(Physics_SetVelocityPolar(entity, testPoints[i].given.v, testPoints[i].given.angle), ERR_OK);
        }
        assert_float_equal(entity->physics.velocity.x, testPoints[i].expected.x, EPSILON);
        assert_float_equal(entity->physics.velocity.y, testPoints[i].expected.y, EPSILON);
    }
}

/**
 * @brief Setter der Rotation funtionieren.
 * 
 * @param state Pointer auf entity_t
 */
static void physics_can_set_rotation_of_entity(void **state) {
    entity_t *entity = (entity_t *)*state;
    // Entität ist NULL -> Parameterfehler
    assert_int_equal(Physics_SetRotation(NULL, 0.0), ERR_PARAMETER);
    assert_int_equal(Physics_SetRelativeRotation(NULL, 0.0), ERR_PARAMETER);
    // Teste verschiedene Zahlenpaare
    struct {
        int relative; // 0 = nutze SetRotation, 1 = nutze SetRelativeRotation
        double given; // welcher Wert der Funktion übergeben wird
        double expected; // welcher Wert die Rotation nun haben sollte
    } testPoints[] = {
        {.relative = 0, .given = 0.0, .expected = 0.0},
        {.relative = 0, .given = 1.0, .expected = 1.0},
        {.relative = 0, .given = -1.0, .expected = -1.0},
        {.relative = 0, .given = 123456.7, .expected = 123456.7},
        {.relative = 0, .given = -7.654321, .expected = -7.654321},
        {.relative = 0, .given = INFINITY, .expected = INFINITY},
        {.relative = 0, .given = -INFINITY, .expected = -INFINITY},
        {.relative = 0, .given = 0.0, .expected = 0.0},
        {.relative = 1, .given = 1.0, .expected = 1.0},
        {.relative = 1, .given = 1.0, .expected = 2.0},
        {.relative = 1, .given = -10.0, .expected = -8.0},
    };
    for (unsigned int i = 0; i < sizeof(testPoints) / sizeof(testPoints[0]); ++i) {
        if (testPoints[i].relative) {
            assert_int_equal(Physics_SetRelativeRotation(entity, testPoints[i].given), ERR_OK);
        } else {
            assert_int_equal(Physics_SetRotation(entity, testPoints[i].given), ERR_OK);
        }
        assert_float_equal(entity->physics.rotation, testPoints[i].expected, EPSILON);
    }
}

/**
 * @brief Mockup eines onCollision Callbacks
 * 
 * @param self Pointer auf die Entität dessen Callback gerade aufgerufen wird
 * @param collision Pointer auf Informationen zur aufgetretenen Kollision
 * 
 * @return Fehlercode gemäss will_return() von CMocka
 */
static int onCollision(entity_t *self, entityCollision_t *collision) {
    function_called();
    check_expected_ptr(self);
    // Callback wird nie mit leeren Flags aufgerufen
    assert_int_not_equal(collision->flags, 0);
    // Wenn eine Kollision mit anderen Entitäten stattfindet sollte immer eine
    // Kollisionsnormale existieren.
    if (collision->flags & ENTITY_COLLISION_ENTITY) {
        float normalSum = collision->normal.x + collision->normal.y;
        assert_float_not_equal(normalSum, 0.0f, EPSILON);
    }
    // Zu löschende Flags für bereits reagierte Kollisionen auf die die Physik
    // nicht mehr reagieren soll.
    collision->flags &= ~mock_type(int);
    return mock_type(int);
}

/**
 * @brief Testzustand mit Entitäten und einer Liste der Entitäten
 * 
 */
typedef struct {
    entity_t entity0;   //!< Erste Entität mit allen Werten = 0, aber w & h = 10
    entity_t entity1;   //!< Zweite Entität mit allen Werten = 0, aber w & h = 10
    list_t *entityList; //!< Liste der Entitäten
} testState_t;

/**
 * @brief Setup: Testzustand mit Entitäten und Liste erstellen
 * 
 * @param[out] state Pointer auf testState_t*
 * 
 * @return 0 Setup erfolgreich, 1 Setup fehlgeschlagen
 */
static int setupTestState(void **state) {
    static testState_t testState = {0};
    // Gültige Entitäten erstellen
    entity_t entityOk = {
        .callbacks.onCollision = onCollision,
        .physics.aabb = {.w = 10, .h = 10}
    };
    testState.entity0 = entityOk;
    testState.entity1 = entityOk;
    // Entitäten der Liste hinzufügen
    List_Create(&testState.entityList);
    List_Add(testState.entityList, &testState.entity0);
    List_Add(testState.entityList, &testState.entity1);
    // Testzustand erstellen
    *state = (void *)&testState;
    return (testState.entityList == NULL);
}

/**
 * @brief Teardown: Testzustand mit Entitäten und Liste löschen.
 * 
 * @param[in] state Pointer auf testState_t*
 * 
 * @return 0 Teardown erfolgreich, -1 Teardown fehlgeschlagen
 */
static int teardownTestState(void **state) {
    testState_t *testState = (testState_t *)*state;
    List_Destroy(&testState->entityList);
    return (testState->entityList != NULL);
}

/**
 * @brief Entität fällt mit der Erdbeschleunigung
 * 
 * @param state Pointer auf testState_t*
 */
static void physics_applies_gravity_to_entity(void **state) {
    testState_t *testState = (testState_t *)*state;
    // Positionen so setzen, dass keine Kollisionen auftreten
    Physics_SetPosition(&testState->entity0, 0.0f, 0.0f);
    Physics_SetPosition(&testState->entity1, 10.0f, 0.0f);
    // keine Kollisionen mit der Welt abfragen
    will_return_always(__wrap_World_CheckCollision, 0);
    // Nach mindestens 10 Iterationen wurde Entität in y Richtung verschoben
    assert_float_equal(testState->entity0.physics.position.y, 0.0f, EPSILON);
    assert_float_equal(testState->entity1.physics.position.y, 0.0f, EPSILON);
    for (int i = 0; i < 10; ++i) {
        assert_int_equal(Physics_Update(testState->entityList), ERR_OK);
    }
    assert_float_not_equal(testState->entity0.physics.position.y, 0.0f, EPSILON);
    assert_float_not_equal(testState->entity1.physics.position.y, 0.0f, EPSILON);
}

/**
 * @brief onCollision Callback wird aufgerufen für identische AABBs
 * 
 * @param state Pointer auf testState_t*
 */
static void physics_on_collision_callback_is_called_for_identical_aabbs(void **state) {
    testState_t *testState = (testState_t *)*state;
    // Überlappen sich vollständig
    Physics_SetPosition(&testState->entity0, 0.0f, 0.0f);
    Physics_SetPosition(&testState->entity1, 0.0f, 0.0f);
    // keine Kollisionen mit der Welt abfragen
    will_return_always(__wrap_World_CheckCollision, 0);
    // Callback der zweiten Entität wird aufgerufen (da diese in der Liste zuerst ist)
    expect_function_call(onCollision);
    expect_value(onCollision, self, &testState->entity1);
    will_return(onCollision, 0);      // keine Flags zurücksetzen
    will_return(onCollision, ERR_OK); // Callback hat keinen Fehler
    // Callback der ersten Entität wird aufgerufen (da diese in der Liste zuletzt ist)
    expect_function_call(onCollision);
    expect_value(onCollision, self, &testState->entity0);
    will_return(onCollision, 0);      // keine Flags zurücksetzen
    will_return(onCollision, ERR_OK); // Callback hat keinen Fehler
    // Update durchführen
    assert_int_equal(Physics_Update(testState->entityList), ERR_OK);
}

/**
 * @brief onCollision Callback wird aufgerufen für sich berührende AABBs
 * 
 * @param state Pointer auf testState_t*
 */
static void physics_on_collision_callback_is_called_for_touching_aabbs(void **state) {
    testState_t *testState = (testState_t *)*state;
    // Überlappen sich rechts mit 1 Pixel
    Physics_SetPosition(&testState->entity0, 0.0f, 0.0f);
    Physics_SetPosition(&testState->entity1, 9.0f, 0.0f);
    // keine Kollisionen mit der Welt abfragen
    will_return_always(__wrap_World_CheckCollision, 0);
    // Callback der zweiten Entität wird aufgerufen (da diese in der Liste zuerst ist)
    expect_function_call(onCollision);
    expect_value(onCollision, self, &testState->entity1);
    will_return(onCollision, 0);      // keine Flags zurücksetzen
    will_return(onCollision, ERR_OK); // Callback hat keinen Fehler
    // Callback der ersten Entität wird aufgerufen (da diese in der Liste zuletzt ist)
    expect_function_call(onCollision);
    expect_value(onCollision, self, &testState->entity0);
    will_return(onCollision, 0);      // keine Flags zurücksetzen
    will_return(onCollision, ERR_OK); // Callback hat keinen Fehler
    // Update durchführen
    assert_int_equal(Physics_Update(testState->entityList), ERR_OK);
}

/**
 * @brief onCollision Callback wird nicht aufgerufen für sich nicht berührende AABBs
 * 
 * @param state Pointer auf testState_t*
 */
static void physics_on_collision_callback_is_not_called_for_side_by_side_aabbs(void **state) {
    testState_t *testState = (testState_t *)*state;
    // Seite an Seite aber ohne Kollision, ohne Pixel dazwischen
    Physics_SetPosition(&testState->entity0, 0.0f, 0.0f);
    Physics_SetPosition(&testState->entity1, 11.0f, 0.0f);
    // keine Kollisionen mit der Welt abfragen
    will_return_always(__wrap_World_CheckCollision, 0);
    // Callback der zweiten Entität wird nicht aufgerufen
    // Callback der ersten Entität wird nicht aufgerufen
    // Update durchführen
    assert_int_equal(Physics_Update(testState->entityList), ERR_OK);
}

/**
 * @brief onCollision Callback kann Flags der Kollision löschen.
 * Löscht der Callback Flags, so sollte die Physik nicht mehr ihre eigene
 * standard Reaktion ausführen.
 * 
 * @param state Pointer auf testState_t*
 */
static void physics_on_collision_callback_can_clear_flags(void **state) {
    testState_t *testState = (testState_t *)*state;
    // Horizontale Geschwindigkeit setzen
    float horizontalVelocity = 1.0f;
    Physics_SetVelocity(&testState->entity0, horizontalVelocity, 0.0f);
    Physics_SetVelocity(&testState->entity1, horizontalVelocity, 0.0f);
    // keine Kollisionen mit der Welt abfragen
    will_return_always(__wrap_World_CheckCollision, 0);
    // Callback der zweiten Entität wird aufgerufen (da diese in der Liste zuerst ist)
    expect_function_call(onCollision);
    expect_value(onCollision, self, &testState->entity1);
    will_return(onCollision, ENTITY_COLLISION_ENTITY); // "handle" die Kollision
    will_return(onCollision, ERR_OK);                  // Callback hat keinen Fehler
    // Callback der ersten Entität wird aufgerufen (da diese in der Liste zuletzt ist)
    expect_function_call(onCollision);
    expect_value(onCollision, self, &testState->entity0);
    will_return(onCollision, 0);      // keine Flags zurücksetzen
    will_return(onCollision, ERR_OK); // Callback hat keinen Fehler
    // Update durchführen
    assert_int_equal(Physics_Update(testState->entityList), ERR_OK);
    // Für die erste Entität wurde die Standardaktion durchgeführt
    assert_float_not_equal(testState->entity0.physics.velocity.x, horizontalVelocity, EPSILON);
    // Für die zweite Entität wurde nicht Standardmässig reagiert
    assert_float_equal(testState->entity1.physics.velocity.x, horizontalVelocity, EPSILON);
}

/**
 * @brief Simuliere zwei fallende Rechecke.
 * Das rechte bewegt sich zusätzlich nach links auf das andere Rechteck zu. Es
 * muss eine Kollision stattfinden die die beiden voneinander weg bewegt. Am
 * Ende sollte das rechte Rechteck immernoch rechts sein und sich nicht durch
 * das linke hindurch bewegt haben.
 * @note Schlägt dieser Test fehl, dann ist entweder die Gravitationskonstante
 * \ref GRAVITY oder der Rückstossfaktor \ref ENTITY_SCALE_FACTOR falsch
 * eingestellt.
 * @note In der GitLab-Pipeline wird lediglich simuliert, aber nichts angezeigt.
 * 
 * @param state Pointer auf testState_t*
 */
static void physics_two_falling_entities_that_are_aproaching_do_not_cross(void **state) {
    testState_t *testState = (testState_t *)*state;
    // Startzustand setzen, zwei Rechtecke am oberen Rand, 60 Pixel entfernt
    Physics_SetPosition(&testState->entity0, (800.0f / 2.0f) - 30.0f, 0.0f);
    Physics_SetPosition(&testState->entity1, (800.0f / 2.0f) + 30.0f, 0.0f);
    // rechtes Rechteck bewegt sich nach links
    Physics_SetVelocity(&testState->entity1, -20.0f, 0.0f);
    // keine Kollisionen mit der Welt abfragen
    will_return_always(__wrap_World_CheckCollision, 0);
    // Der Kollisionscallback soll mindestens 1 mal aufgerufen werden
    expect_function_call_any(onCollision);
    // Die Funktionsargumente nicht prüfen
    expect_not_value_count(onCollision, self, 0, -1);
    // alle Mockwerte sollen 0 sein
    will_return_always(onCollision, 0);
    // In der GitLab-Pipeline wird nur auf die Kollision geprüft, nicht aber die
    // visuelle Ausgabe.
#ifndef CI_TEST
    SDLW_Init(800, 600);
#endif
    // 5 Sekunden lang simulieren
    for (int i = 0; i < 60 * 5; ++i) {
        Physics_Update(testState->entityList);
#ifndef CI_TEST
        // Stelle die AABBs visuell dar
        SDL_Color black = {.r = 255, .g = 255, .b = 255};
        SDLW_Clear(black);
        SDL_Color red = {.r = 255, .g = 0, .b = 0};
        SDLW_DrawFilledRect(testState->entity0.physics.aabb, red);
        SDL_Color green = {.r = 0, .g = 255, .b = 0};
        SDLW_DrawFilledRect(testState->entity1.physics.aabb, green);
        SDLW_Render();
#endif
    }
    // Das rechte Rechteck ist nicht durch das linke hindurch gegangen
    assert_true(testState->entity0.physics.position.x < testState->entity1.physics.position.x);
#ifndef CI_TEST
    SDLW_Quit();
#endif
}

/**
 * @brief Simuliere ein Rechteck welches auf einem anderen ruht.
 * Das untere Rechteck verhält sich als "Boden" und bewegt sich nicht. Das obere
 * Rechteck sollte nicht zittern oder hochhüpfen.
 * @note Schlägt dieser Test fehl, dann ist entweder die Gravitationskonstante
 * \ref GRAVITY oder der Rückstossfaktor \ref ENTITY_SCALE_FACTOR falsch
 * eingestellt.
 * @note In der GitLab-Pipeline wird lediglich simuliert, aber nichts angezeigt.
 * 
 * @param state Pointer auf testState_t*
 */
static void physics_resting_entity_ontop_of_another_entity_does_not_fall_through(void **state) {
    testState_t *testState = (testState_t *)*state;
    // Startzustand setzen, oberes Rechteck direkt über dem unteren
    Physics_SetPosition(&testState->entity0, (800.0f / 2.0f), 100.0f - 10.0f);
    // unteres Rechteck ist sehr breit
    testState->entity1.physics.aabb.w = 600;
    testState->entity1.physics.aabb.h = 10;
    // keine Kollisionen mit der Welt abfragen
    will_return_always(__wrap_World_CheckCollision, 0);
    // Der Kollisionscallback soll mindestens 1 mal aufgerufen werden
    expect_function_call_any(onCollision);
    // Die Funktionsargumente nicht prüfen
    expect_not_value_count(onCollision, self, 0, -1);
    // alle Mockwerte sollen 0 sein
    will_return_always(onCollision, 0);
    // In der GitLab-Pipeline wird nur auf die Kollision geprüft, nicht aber die
    // visuelle Ausgabe.
#ifndef CI_TEST
    SDLW_Init(800, 600);
#endif
    // 5 Sekunden lang simulieren
    for (int i = 0; i < 60 * 5; ++i) {
        // Position und Geschwindigkeit des unteren Rechtecks fixieren
        Physics_SetPosition(&testState->entity1, (800.0f / 2.0f), 100.0f);
        Physics_SetVelocity(&testState->entity1, 0.0f, 0.0f);
        Physics_Update(testState->entityList);
#ifndef CI_TEST
        // Stelle die AABBs visuell dar
        SDL_Color black = {.r = 255, .g = 255, .b = 255};
        SDLW_Clear(black);
        SDL_Color red = {.r = 255, .g = 0, .b = 0};
        SDLW_DrawFilledRect(testState->entity0.physics.aabb, red);
        SDL_Color green = {.r = 0, .g = 255, .b = 0};
        SDLW_DrawFilledRect(testState->entity1.physics.aabb, green);
        SDLW_Render();
#endif
    }
    // Das fallende Rechteck ist nicht zur Seite bewegt worden.
    assert_float_equal(testState->entity0.physics.position.x, (800.0f / 2.0f), EPSILON);
    // Es ist in etwa an Ort und Stelle geblieben (+- 2 Pixel)
    assert_float_equal(testState->entity0.physics.position.y, 90.0f, 2.0f);
#ifndef CI_TEST
    SDLW_Quit();
#endif
}

/**
 * @brief Simuliere ein Rechteck welches auf ein anderes fällt.
 * Das untere Rechteck verhält sich als "Boden" und bewegt sich nicht. Bei der
 * Kollision darf das fallende Rechteck nicht durch den "Boden" fallen.
 * @note Schlägt dieser Test fehl, dann ist entweder die Gravitationskonstante
 * \ref GRAVITY oder der Rückstossfaktor \ref ENTITY_SCALE_FACTOR falsch
 * eingestellt.
 * @note In der GitLab-Pipeline wird lediglich simuliert, aber nichts angezeigt.
 * 
 * @param state Pointer auf testState_t*
 */
static void physics_falling_entity_stops_on_collision_with_another_entity_and_does_not_fall_through(void **state) {
    testState_t *testState = (testState_t *)*state;
    // Startzustand setzen, oberes Rechteck ist oben in der Mitte
    Physics_SetPosition(&testState->entity0, (800.0f / 2.0f), 50.0f);
    // unteres Rechteck ist sehr breit
    testState->entity1.physics.aabb.w = 600;
    testState->entity1.physics.aabb.h = 10;
    // keine Kollisionen mit der Welt abfragen
    will_return_always(__wrap_World_CheckCollision, 0);
    // Der Kollisionscallback soll mindestens 1 mal aufgerufen werden
    expect_function_call_any(onCollision);
    // Die Funktionsargumente nicht prüfen
    expect_not_value_count(onCollision, self, 0, -1);
    // alle Mockwerte sollen 0 sein
    will_return_always(onCollision, 0);
    // In der GitLab-Pipeline wird nur auf die Kollision geprüft, nicht aber die
    // visuelle Ausgabe.
#ifndef CI_TEST
    SDLW_Init(800, 600);
#endif
    // 5 Sekunden lang simulieren
    for (int i = 0; i < 60 * 5; ++i) {
        // Position und Geschwindigkeit des unteren Rechtecks fixieren
        Physics_SetPosition(&testState->entity1, (800.0f / 2.0f), 100.0f);
        Physics_SetVelocity(&testState->entity1, 0.0f, 0.0f);
        Physics_Update(testState->entityList);
#ifndef CI_TEST
        // Stelle die AABBs visuell dar
        SDL_Color black = {.r = 255, .g = 255, .b = 255};
        SDLW_Clear(black);
        SDL_Color red = {.r = 255, .g = 0, .b = 0};
        SDLW_DrawFilledRect(testState->entity0.physics.aabb, red);
        SDL_Color green = {.r = 0, .g = 255, .b = 0};
        SDLW_DrawFilledRect(testState->entity1.physics.aabb, green);
        SDLW_Render();
#endif
    }
    // Das fallende Rechteck ist höchstens nach oben, aber nicht zur Seite bewegt worden.
    assert_float_equal(testState->entity0.physics.position.x, (800.0f / 2.0f), EPSILON);
    // Es ist nicht durch das untere Rechteck gefallen
    assert_true(testState->entity0.physics.position.y < testState->entity1.physics.position.y);
#ifndef CI_TEST
    SDLW_Quit();
#endif
}

/**
 * @brief Simuliere ein Rechteck welches auf eines fällt das nach unten bewegt.
 * Das untere Rechteck verhält sich als "Boden" und bewegt sich nach unten. Bei
 * der Kollision darf das fallende Rechteck nicht durch den "Boden" fallen.
 * @note Schlägt dieser Test fehl, dann ist entweder die Gravitationskonstante
 * \ref GRAVITY oder der Rückstossfaktor \ref ENTITY_SCALE_FACTOR falsch
 * eingestellt.
 * @note In der GitLab-Pipeline wird lediglich simuliert, aber nichts angezeigt.
 * 
 * @param state Pointer auf testState_t*
 */
static void physics_falling_entity_that_collides_with_another_falling_entity_does_not_fall_through(void **state) {
    testState_t *testState = (testState_t *)*state;
    // Startzustand setzen, oberes Rechteck ist oben in der Mitte
    Physics_SetPosition(&testState->entity0, (800.0f / 2.0f), 50.0f);
    // unteres Rechteck ist sehr breit
    Physics_SetPosition(&testState->entity1, (800.0f / 2.0f), 100.0f);
    testState->entity1.physics.aabb.w = 600;
    testState->entity1.physics.aabb.h = 10;
    // keine Kollisionen mit der Welt abfragen
    will_return_always(__wrap_World_CheckCollision, 0);
    // Der Kollisionscallback soll mindestens 1 mal aufgerufen werden
    expect_function_call_any(onCollision);
    // Die Funktionsargumente nicht prüfen
    expect_not_value_count(onCollision, self, 0, -1);
    // alle Mockwerte sollen 0 sein
    will_return_always(onCollision, 0);
    // In der GitLab-Pipeline wird nur auf die Kollision geprüft, nicht aber die
    // visuelle Ausgabe.
#ifndef CI_TEST
    SDLW_Init(800, 600);
#endif
    // 5 Sekunden lang simulieren
    for (int i = 0; i < 60 * 5; ++i) {
        // Geschwindigkeit des unteren Rechtecks fixieren
        Physics_SetVelocity(&testState->entity1, 0.0f, 20.0f);
        Physics_Update(testState->entityList);
#ifndef CI_TEST
        // Stelle die AABBs visuell dar
        SDL_Color black = {.r = 255, .g = 255, .b = 255};
        SDLW_Clear(black);
        SDL_Color red = {.r = 255, .g = 0, .b = 0};
        SDLW_DrawFilledRect(testState->entity0.physics.aabb, red);
        SDL_Color green = {.r = 0, .g = 255, .b = 0};
        SDLW_DrawFilledRect(testState->entity1.physics.aabb, green);
        SDLW_Render();
#endif
    }
    // Das fallende Rechteck ist höchstens nach oben, aber nicht zur Seite bewegt worden.
    assert_float_equal(testState->entity0.physics.position.x, (800.0f / 2.0f), EPSILON);
    // Es ist nicht durch das untere Rechteck gefallen
    assert_true(testState->entity0.physics.position.y < testState->entity1.physics.position.y);
#ifndef CI_TEST
    SDLW_Quit();
#endif
}

/**
 * @brief Simuliere ein Rechteck welches auf eines fällt das nach oben bewegt.
 * Das untere Rechteck verhält sich als "Boden" und bewegt sich nach oben. Bei
 * der Kollision darf das fallende Rechteck nicht durch den "Boden" fallen.
 * @note Schlägt dieser Test fehl, dann ist entweder die Gravitationskonstante
 * \ref GRAVITY oder der Rückstossfaktor \ref ENTITY_SCALE_FACTOR falsch
 * eingestellt.
 * @note In der GitLab-Pipeline wird lediglich simuliert, aber nichts angezeigt.
 * 
 * @param state Pointer auf testState_t*
 */
static void physics_falling_entity_that_collides_with_another_rising_entity_does_not_fall_through(void **state) {
    testState_t *testState = (testState_t *)*state;
    // Startzustand setzen, oberes Rechteck ist oben in der Mitte
    Physics_SetPosition(&testState->entity0, (800.0f / 2.0f), 50.0f);
    // unteres Rechteck ist sehr breit
    Physics_SetPosition(&testState->entity1, (800.0f / 2.0f), 100.0f);
    testState->entity1.physics.aabb.w = 600;
    testState->entity1.physics.aabb.h = 10;
    // keine Kollisionen mit der Welt abfragen
    will_return_always(__wrap_World_CheckCollision, 0);
    // Der Kollisionscallback soll mindestens 1 mal aufgerufen werden
    expect_function_call_any(onCollision);
    // Die Funktionsargumente nicht prüfen
    expect_not_value_count(onCollision, self, 0, -1);
    // alle Mockwerte sollen 0 sein
    will_return_always(onCollision, 0);
    // In der GitLab-Pipeline wird nur auf die Kollision geprüft, nicht aber die
    // visuelle Ausgabe.
#ifndef CI_TEST
    SDLW_Init(800, 600);
#endif
    // 5 Sekunden lang simulieren
    for (int i = 0; i < 60 * 5; ++i) {
        // Geschwindigkeit des unteren Rechtecks fixieren
        Physics_SetVelocity(&testState->entity1, 0.0f, -30.0f);
        Physics_Update(testState->entityList);
#ifndef CI_TEST
        // Stelle die AABBs visuell dar
        SDL_Color black = {.r = 255, .g = 255, .b = 255};
        SDLW_Clear(black);
        SDL_Color red = {.r = 255, .g = 0, .b = 0};
        SDLW_DrawFilledRect(testState->entity0.physics.aabb, red);
        SDL_Color green = {.r = 0, .g = 255, .b = 0};
        SDLW_DrawFilledRect(testState->entity1.physics.aabb, green);
        SDLW_Render();
#endif
    }
    // Das fallende Rechteck ist höchstens nach oben, aber nicht zur Seite bewegt worden.
    assert_float_equal(testState->entity0.physics.position.x, (800.0f / 2.0f), EPSILON);
#ifndef CI_TEST
    SDLW_Quit();
#endif
}

/**
 * @brief Setup: Welt laden und Testzustand mit Entitäten und Liste erstellen.
 * 
 * @note in der GitLap-Pipeline wird keine Welt geladen
 * 
 * @param[out] state Pointer auf testState_t*
 * 
 * @return 0 Setup erfolgreich, 1 Setup fehlgeschlagen
 */
static int setupTestStateAndWorld(void **state) {
    static testState_t testState = {0};
    // Gültige Entitäten erstellen
    entity_t entityOk = {
        .callbacks.onCollision = onCollision,
        .physics.aabb = {.w = 10, .h = 10}
    };
    testState.entity0 = entityOk;
    testState.entity1 = entityOk;
    // Entitäten der Liste hinzufügen
    List_Create(&testState.entityList);
    List_Add(testState.entityList, &testState.entity0);
    List_Add(testState.entityList, &testState.entity1);
    // Testzustand erstellen
    *state = (void *)&testState;
    // SDLW und Welt laden
    int ret = ERR_OK;
#ifndef CI_TEST
    ret |= SDLW_Init(1024, 576);
    ret |= World_Init();
    ret |= SDLW_LoadResources("assets/test/config.cfg");
    ret |= World_Load("world");
#endif
    // Erfolgreich wenn Entitätsliste erstellt und Welt geladen
    if (testState.entityList && ret == ERR_OK) { // cppcheck-suppress knownConditionTrueFalse
        return 0;
    }
    return 1;
}

/**
 * @brief Teardown: Welt entladen und Testzustand löschen.
 * 
 * @param[in] state Pointer auf testState_t*
 * 
 * @return 0 Teardown erfolgreich, -1 Teardown fehlgeschlagen
 */
static int teardownTestStateAndWorld(void **state) {
    testState_t *testState = (testState_t *)*state;
    // Testzustand löschen
    List_Destroy(&testState->entityList);
    // Welt entladen
    int ret = ERR_OK;
#ifndef CI_TEST
    World_Quit();
    SDLW_Quit();
#endif
    // Erfolgreich wenn Entitätsliste gelöscht und Welt entladen
    if (!testState->entityList && ret == ERR_OK) { // cppcheck-suppress knownConditionTrueFalse
        return 0;
    }
    return 1;
}

/**
 * @brief Simuliere Rechtecke welche auf der Welt liegen.
 * Die Rechtecke sollten nicht zittern oder hochhüpfen.
 * @note Schlägt dieser Test fehl, dann ist entweder die Gravitationskonstante
 * \ref GRAVITY oder der Rückstossfaktor \ref WORLD_SCALE_FACTOR.
 * @note In der GitLab-Pipeline wird der Test übersprungen.
 * 
 * @param state Pointer auf testState_t*
 */
static void physics_resting_entity_ontop_of_the_world_does_not_fall_through(void **state) {
    testState_t *testState = (testState_t *)*state;
    // Läuft nicht im CI da die Welt von SDL abhängig ist.
#ifdef CI_TEST
    skip();
#endif
    // Startpunkte abfragen
    const int points = 10;
    SDL_Point startPoints[points];
    assert_int_equal(World_CreateStartLocations(testState->entity0.physics.aabb, points, startPoints), ERR_OK);
    // Kollisionen mit der Welt aktivieren
    will_return_always(__wrap_World_CheckCollision, 1);
    // Der Kollisionscallback soll mindestens 1 mal aufgerufen werden
    expect_function_call_any(onCollision);
    // Die Funktionsargumente nicht prüfen
    expect_not_value_count(onCollision, self, 0, -1);
    // alle Mockwerte sollen 0 sein
    will_return_always(onCollision, 0);
    // für alle Startpunkte testen
    for (int p = 0; p < points; p += 2) {
        // Startpunkt setzen (+5 Pixel um die Höhe der Entität zu berücksichtigen)
        Physics_SetPosition(&testState->entity0, startPoints[p].x + 5, startPoints[p].y + 5);
        Physics_SetPosition(&testState->entity1, startPoints[p + 1].x + 5, startPoints[p + 1].y + 5);
        // 2 Sekunden lang simulieren
        for (int i = 0; i < 60 * 2; ++i) {
            Physics_Update(testState->entityList);
            // Stelle die AABBs und die Welt visuell dar
            SDL_Color black = {.r = 255, .g = 255, .b = 255};
            SDLW_Clear(black);
            World_DrawBackground();
            World_DrawForeground();
            SDL_Color red = {.r = 255, .g = 0, .b = 0};
            SDLW_DrawFilledRect(testState->entity0.physics.aabb, red);
            SDL_Color green = {.r = 0, .g = 255, .b = 0};
            SDLW_DrawFilledRect(testState->entity1.physics.aabb, green);
            SDLW_Render();
        }
        // Das Rechteck ist nicht zur Seite bewegt worden.
        assert_int_equal(testState->entity0.physics.aabb.x, startPoints[p].x);
        assert_int_equal(testState->entity1.physics.aabb.x, startPoints[p + 1].x);
        // Es ist in etwa an Ort und Stelle geblieben (+- 2 Pixel)
        assert_in_range(testState->entity0.physics.aabb.y, startPoints[p].y - 2, startPoints[p].y + 2);
        assert_in_range(testState->entity1.physics.aabb.y, startPoints[p + 1].y - 2, startPoints[p + 1].y + 2);
    }
}

/**
 * @brief Simuliere ein Rechteck welches sich auf der Weltoberfläche bewegt.
 * Des Rechteck sollte nicht zittern oder hüpfen und nicht in die Welt hinein.
 * Per Pfeiltasten Links / Rechts kann manuell bewegt werden.
 * @note Schlägt dieser Test fehl, dann ist entweder die Gravitationskonstante
 * \ref GRAVITY oder der Rückstossfaktor \ref WORLD_SCALE_FACTOR.
 * @note In der GitLab-Pipeline wird der Test übersprungen.
 * 
 * @param state Pointer auf testState_t*
 */
static void physics_manually_moving_entity_ontop_of_the_world_does_not_fall_through(void **state) {
    testState_t *testState = (testState_t *)*state;
    // Läuft nicht im CI da die Welt von SDL abhängig ist.
#ifdef CI_TEST
    skip();
#endif
    // Zweite Entität weit weg setzen, wird nicht benötigt
    testState->entity1.physics.position.x = -1000.0;
    // Startpunkt
    SDL_Point startPoint;
    assert_int_equal(World_CreateStartLocations(testState->entity0.physics.aabb, 1, &startPoint), ERR_OK);
    // Kollisionen mit der Welt aktivieren
    will_return_always(__wrap_World_CheckCollision, 1);
    // Der Kollisionscallback soll mindestens 1 mal aufgerufen werden
    expect_function_call_any(onCollision);
    // Die Funktionsargumente nicht prüfen
    expect_not_value_count(onCollision, self, 0, -1);
    // alle Mockwerte sollen 0 sein
    will_return_always(onCollision, 0);
    // Startpunkt setzen (+5 Pixel um die Höhe der Entität zu berücksichtigen)
    Physics_SetPosition(&testState->entity0, startPoint.x + 5, startPoint.y + 5);
    // 30 Sekunden lang simulieren
    for (int i = 0; i < 60 * 30; ++i) {
        // per Pfeiltasten kann bewegt werden
        SDL_Event e;
        SDL_PollEvent(&e);
        float *x = &testState->entity0.physics.velocity.x;
        if (e.type == SDL_KEYDOWN) {
            // Geschwindigkeit gemäss Pfeiltasten verändern
            if (e.key.keysym.sym == SDLK_LEFT) {
                if (*x > -50.0f) *x -= 10.0f;
            } else if (e.key.keysym.sym == SDLK_RIGHT) {
                if (*x < 50.0f) *x += 10.0f;
            }
        }
        *x *= 0.99; // dämpfen
        // Physik berechnen
        Physics_Update(testState->entityList);
        // Stelle die AABBs und die Welt visuell dar
        SDL_Color black = {.r = 255, .g = 255, .b = 255};
        SDLW_Clear(black);
        World_DrawBackground();
        World_DrawForeground();
        SDL_Color red = {.r = 255, .g = 0, .b = 0};
        SDLW_DrawFilledRect(testState->entity0.physics.aabb, red);
        SDLW_Render();
    }
}


/**
 * @brief Testprogramm
 * 
 * @return int Anzahl fehlgeschlagener Tests
 */
int main(void) {
    const struct CMUnitTest physics[] = {
        cmocka_unit_test_setup(
            physics_can_set_position_of_entity, setupOneEntity),
        cmocka_unit_test_setup(
            physics_can_set_velocity_of_entity, setupOneEntity),
        cmocka_unit_test_setup(
            physics_can_set_velocity_in_polar_of_entity, setupOneEntity),
        cmocka_unit_test_setup(
            physics_can_set_rotation_of_entity, setupOneEntity),

        cmocka_unit_test_setup_teardown(
            physics_applies_gravity_to_entity,
            setupTestState, teardownTestState),
        cmocka_unit_test_setup_teardown(
            physics_on_collision_callback_is_called_for_identical_aabbs,
            setupTestState, teardownTestState),
        cmocka_unit_test_setup_teardown(
            physics_on_collision_callback_is_called_for_touching_aabbs,
            setupTestState, teardownTestState),
        cmocka_unit_test_setup_teardown(
            physics_on_collision_callback_is_not_called_for_side_by_side_aabbs,
            setupTestState, teardownTestState),
        cmocka_unit_test_setup_teardown(
            physics_on_collision_callback_can_clear_flags,
            setupTestState, teardownTestState),

        cmocka_unit_test_setup_teardown(
            physics_two_falling_entities_that_are_aproaching_do_not_cross,
            setupTestState, teardownTestState),
        cmocka_unit_test_setup_teardown(
            physics_resting_entity_ontop_of_another_entity_does_not_fall_through,
            setupTestState, teardownTestState),
        cmocka_unit_test_setup_teardown(
            physics_falling_entity_stops_on_collision_with_another_entity_and_does_not_fall_through,
            setupTestState, teardownTestState),
        cmocka_unit_test_setup_teardown(
            physics_falling_entity_that_collides_with_another_falling_entity_does_not_fall_through,
            setupTestState, teardownTestState),
        cmocka_unit_test_setup_teardown(
            physics_falling_entity_that_collides_with_another_rising_entity_does_not_fall_through,
            setupTestState, teardownTestState),

        cmocka_unit_test_setup_teardown(
            physics_resting_entity_ontop_of_the_world_does_not_fall_through,
            setupTestStateAndWorld, teardownTestStateAndWorld),
        cmocka_unit_test_setup_teardown(
            physics_manually_moving_entity_ontop_of_the_world_does_not_fall_through,
            setupTestStateAndWorld, teardownTestStateAndWorld),
    };
    return cmocka_run_group_tests(physics, NULL, NULL);
}
