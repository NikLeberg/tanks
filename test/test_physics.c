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

#include "error.h"
#include "physics.h"


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
 * @return 0 Setup erfolgreich, -1 Setup fehlgeschlagen
 */
static int setupOneEntity(void **state) {
    static entity_t entity;
    entity_t entityOk = {0};
    entity = entityOk;
    *state = (void *)&entity;
    return 0;
}

/**
 * @brief Teardown: Entität entfernen
 * 
 * @param[in] state Pointer auf entity_t*
 * 
 * @return 0 Teardown erfolgreich, -1 Teardown fehlgeschlagen
 */
static int teardownOneEntity(void **state) {
    entity_t *entity = (entity_t *)*state;
    (void)entity;
    return 0;
}

/**
 * @brief Setter der Position funtioniert.
 * 
 * @param state Pointer auf entity_t
 */
static void physics_can_set_position_of_entity(void **state) {
    entity_t *entity = (entity_t *)*state;
    // NULL
    assert_int_equal(Physics_SetPosition(NULL, 0.0f, 0.0f), ERR_PARAMETER);
    // 0.0f, 0.0f
    assert_int_equal(Physics_SetPosition(entity, 0.0f, 0.0f), ERR_OK);
    assert_float_equal(entity->physics.position.x, 0.0f, EPSILON);
    assert_float_equal(entity->physics.position.y, 0.0f, EPSILON);
    // -1.0f, -1.0f
    assert_int_equal(Physics_SetPosition(entity, -1.0f, -1.0f), ERR_OK);
    assert_float_equal(entity->physics.position.x, -1.0f, EPSILON);
    assert_float_equal(entity->physics.position.y, -1.0f, EPSILON);
    // 123456.7f, -7.654321f
    assert_int_equal(Physics_SetPosition(entity, 123456.7f, -7.654321f), ERR_OK);
    assert_float_equal(entity->physics.position.x, 123456.7f, EPSILON);
    assert_float_equal(entity->physics.position.y, -7.654321f, EPSILON);
    // Unendlich und NaN sind erlaubt
    assert_int_equal(Physics_SetPosition(entity, 1.0f/0.0f, 0.0f/0.0f), ERR_OK);
}

/**
 * @brief Setter der Geschwindigkeit funtioniert.
 * 
 * @param state Pointer auf entity_t
 */
static void physics_can_set_velocity_of_entity(void **state) {
    entity_t *entity = (entity_t *)*state;
    // NULL
    assert_int_equal(Physics_SetVelocity(NULL, 0.0f, 0.0f), ERR_PARAMETER);
    // 0.0f, 0.0f
    assert_int_equal(Physics_SetVelocity(entity, 0.0f, 0.0f), ERR_OK);
    assert_float_equal(entity->physics.velocity.x, 0.0f, EPSILON);
    assert_float_equal(entity->physics.velocity.y, 0.0f, EPSILON);
    // -1.0f, -1.0f
    assert_int_equal(Physics_SetVelocity(entity, -1.0f, -1.0f), ERR_OK);
    assert_float_equal(entity->physics.velocity.x, -1.0f, EPSILON);
    assert_float_equal(entity->physics.velocity.y, -1.0f, EPSILON);
    // 123456.7f, -7.654321f
    assert_int_equal(Physics_SetVelocity(entity, 123456.7f, -7.654321f), ERR_OK);
    assert_float_equal(entity->physics.velocity.x, 123456.7f, EPSILON);
    assert_float_equal(entity->physics.velocity.y, -7.654321f, EPSILON);
    // Unendlich und NaN sind erlaubt
    assert_int_equal(Physics_SetVelocity(entity, 1.0f/0.0f, 0.0f/0.0f), ERR_OK);
}

/**
 * @brief Setter der Geschwindigkeit in Polarform funtioniert.
 * 
 * @param state Pointer auf entity_t
 */
static void physics_can_set_velocity_in_polar_of_entity(void **state) {
    entity_t *entity = (entity_t *)*state;
    // NULL
    assert_int_equal(Physics_SetVelocityPolar(NULL, 0.0f, 0.0f), ERR_PARAMETER);
    // 0.0f, 0.0 -> Ursprung
    assert_int_equal(Physics_SetVelocityPolar(entity, 0.0f, 0.0f), ERR_OK);
    assert_float_equal(entity->physics.velocity.x, 0.0f, EPSILON);
    assert_float_equal(entity->physics.velocity.y, 0.0f, EPSILON);
    // Länge 1.0 mit 0.0° -> nach rechts, horizontal
    assert_int_equal(Physics_SetVelocityPolar(entity, 1.0f, 0.0), ERR_OK);
    assert_float_equal(entity->physics.velocity.x, 1.0f, EPSILON);
    assert_float_equal(entity->physics.velocity.y, 0.0f, EPSILON);
    // Länge 1.0 mit 90.0° -> nach unten
    assert_int_equal(Physics_SetVelocityPolar(entity, 1.0f, 90.0), ERR_OK);
    assert_float_equal(entity->physics.velocity.x, 0.0f, EPSILON);
    assert_float_equal(entity->physics.velocity.y, 1.0f, EPSILON);
    // Länge 1.0 mit 180.0° -> nach links
    assert_int_equal(Physics_SetVelocityPolar(entity, 1.0f, 180.0), ERR_OK);
    assert_float_equal(entity->physics.velocity.x, -1.0f, EPSILON);
    assert_float_equal(entity->physics.velocity.y, 0.0f, EPSILON);
    // Länge 1.0 mit 270.0° -> nach oben
    assert_int_equal(Physics_SetVelocityPolar(entity, 1.0f, 270.0), ERR_OK);
    assert_float_equal(entity->physics.velocity.x, 0.0f, EPSILON);
    assert_float_equal(entity->physics.velocity.y, -1.0f, EPSILON);
    // Länge 1.0 mit 45.0° -> nach rechts unten
    assert_int_equal(Physics_SetVelocityPolar(entity, 1.0f, 45.0), ERR_OK);
    assert_float_equal(entity->physics.velocity.x, 1.0f / sqrt(2.0), EPSILON);
    assert_float_equal(entity->physics.velocity.y, 1.0f / sqrt(2.0), EPSILON);
    // Unendlich und NaN sind erlaubt
    assert_int_equal(Physics_SetVelocityPolar(entity, 1.0f/0.0f, 0.0/0.0), ERR_OK);
}

/**
 * @brief Setter der Rotation funtioniert.
 * 
 * @param state Pointer auf entity_t
 */
static void physics_can_set_rotation_of_entity(void **state) {
    entity_t *entity = (entity_t *)*state;
    // NULL
    assert_int_equal(Physics_SetRotation(NULL, 0.0), ERR_PARAMETER);
    // 0.0
    assert_int_equal(Physics_SetRotation(entity, 0.0), ERR_OK);
    assert_float_equal(entity->physics.rotation, 0.0, EPSILON);
    // 1.0
    assert_int_equal(Physics_SetRotation(entity, 1.0), ERR_OK);
    assert_float_equal(entity->physics.rotation, 1.0, EPSILON);
    // -1.0
    assert_int_equal(Physics_SetRotation(entity, -1.0), ERR_OK);
    assert_float_equal(entity->physics.rotation, -1.0, EPSILON);
    // 123456.7
    assert_int_equal(Physics_SetRotation(entity, 123456.7), ERR_OK);
    assert_float_equal(entity->physics.rotation, 123456.7, EPSILON);
    // -7.654321
    assert_int_equal(Physics_SetRotation(entity, -7.654321), ERR_OK);
    assert_float_equal(entity->physics.rotation, -7.654321, EPSILON);
    // Unendlich ist erlaubt
    assert_int_equal(Physics_SetRotation(entity, 1.0/0.0), ERR_OK);
    // NaN ist erlaubt
    assert_int_equal(Physics_SetRotation(entity, 0.0/0.0), ERR_OK);
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
    // zu löschende Flags für bereits reagierte Kollisionen
    collision->flags &= ~mock_type(int);
    return mock_type(int);
}

/**
 * @brief Testzustand mit Entitäten und einer Liste der Entitäten
 * 
 */
typedef struct {
    entity_t entity0; //!< Erste Entität mit allen Werten = 0
    entity_t entity1; //!< Zweite Entität mit allen Werten = 0
    list_t *entityList; //!< Liste der Entitäten
} testState_t;

/**
 * @brief Setup: Testzustand mit Entitäten und Liste erstellen
 * 
 * @param[out] state Pointer auf testState_t*
 * 
 * @return 0 Setup erfolgreich, -1 Setup fehlgeschlagen
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
 * @brief Teardown: Testzustand mit Entitäten und Liste löschen
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
    testState->entity0.physics.position = (SDL_FPoint){.x = 0.0f, .y = 0.0f};
    testState->entity1.physics.position = (SDL_FPoint){.x = 10.0f, .y = 0.0f};
    // Nach mindestens 10 Iterationen wurde y angepasst
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
    SDL_FPoint position = {.x = 0.0f, .y = 0.0f};
    testState->entity0.physics.position = position;
    testState->entity1.physics.position = position;
    // Callback der zweiten Entität wird aufgerufen (da diese in der Liste zuerst ist)
    expect_function_call(onCollision);
    expect_value(onCollision, self, &testState->entity1);
    will_return(onCollision, 0); // noch keine Flags zum zurücksetzen
    will_return(onCollision, ERR_OK); // Callback hat keinen Fehler
    // Callback der ersten Entität wird aufgerufen (da diese in der Liste zuletzt ist)
    expect_function_call(onCollision);
    expect_value(onCollision, self, &testState->entity0);
    will_return(onCollision, 0); // noch keine Flags zum zurücksetzen
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
    SDL_FPoint position0 = {.x = 0.0f, .y = 0.0f};
    testState->entity0.physics.position = position0;
    SDL_FPoint position1 = {.x = 9.0f, .y = 0.0f};
    testState->entity1.physics.position = position1;
    // Callback der zweiten Entität wird aufgerufen (da diese in der Liste zuerst ist)
    expect_function_call(onCollision);
    expect_value(onCollision, self, &testState->entity1);
    will_return(onCollision, 0); // noch keine Flags zum zurücksetzen
    will_return(onCollision, ERR_OK); // Callback hat keinen Fehler
    // Callback der ersten Entität wird aufgerufen (da diese in der Liste zuletzt ist)
    expect_function_call(onCollision);
    expect_value(onCollision, self, &testState->entity0);
    will_return(onCollision, 0); // noch keine Flags zum zurücksetzen
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
    SDL_FPoint position0 = {.x = 0.0f, .y = 0.0f};
    testState->entity0.physics.position = position0;
    SDL_FPoint position1 = {.x = 11.0f, .y = 0.0f};
    testState->entity1.physics.position = position1;
    // Callback der zweiten Entität wird nicht aufgerufen
    // Callback der ersten Entität wird nicht aufgerufen
    // Update durchführen
    assert_int_equal(Physics_Update(testState->entityList), ERR_OK);
}

/**
 * @brief Testprogramm
 * 
 * @return int Anzahl fehlgeschlagener Tests
 */
int main(void) {
    const struct CMUnitTest physics[] = {
        cmocka_unit_test_setup_teardown(
            physics_can_set_position_of_entity,
            setupOneEntity, teardownOneEntity),
        cmocka_unit_test_setup_teardown(
            physics_can_set_velocity_of_entity,
            setupOneEntity, teardownOneEntity),
        cmocka_unit_test_setup_teardown(
            physics_can_set_velocity_in_polar_of_entity,
            setupOneEntity, teardownOneEntity),
        cmocka_unit_test_setup_teardown(
            physics_can_set_rotation_of_entity,
            setupOneEntity, teardownOneEntity),
        
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
    };
    return cmocka_run_group_tests(physics, NULL, NULL);
}
