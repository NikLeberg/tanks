/**
 * @file test_entityHandler.c
 * @author Leuenberger Niklaus (leuen4@bfh.ch)
 * @brief Tests für EntityHandler-Modul
 * @version 0.1
 * @date 2021-03-30
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

#include "error.h"
#include "list.h"
#define SDL_MAIN_HANDLED
#include "entityHandler.h"


/*
 * Tests
 * 
 */

/**
 * @brief Eine Entität kann hinzugefügt und entfernt werden.
 * 
 * @param state unbenutzt
 */
static void an_entity_can_be_added_and_removed(void **state) {
    (void) state;
    entity_t entity = {.owner = "Testuser", .name = "Test"};
    assert_int_equal(EntityHandler_AddEntity(&entity), ERR_OK);
    assert_int_equal(EntityHandler_RemoveEntity(&entity), ERR_OK);
}

/**
 * @brief Ungültige Entitäten können nicht hinzugefügt werden.
 * 
 * @param state unbenutzt
 */
static void invalid_entities_cant_be_added(void **state) {
    (void) state;
    entity_t entityOk = {.state = ENTITY_STATE_CREATED, .owner = "Testuser", .name = "Test"};
    // NULL als Entität
    assert_int_not_equal(EntityHandler_AddEntity(NULL), ERR_OK);
    // Entität mit falschem Zustand
    entity_t entityWrongState = entityOk;
    entityWrongState.state = ENTITY_STATE_ACTIVE;
    assert_int_not_equal(EntityHandler_AddEntity(&entityWrongState), ERR_OK);
    // Entität ohne Eigentümer
    entity_t entityNoOwner = entityOk;
    entityNoOwner.owner = NULL;
    assert_int_not_equal(EntityHandler_AddEntity(&entityNoOwner), ERR_OK);
    // Entität ohne Namen
    entity_t entityNoName = entityOk;
    entityNoName.name = NULL;
    assert_int_not_equal(EntityHandler_AddEntity(&entityNoName), ERR_OK);
}

/**
 * @brief Hundert Entitäten können hinzugefügt und entfernt werden.
 * 
 * @param state unbenutzt
 */
static void one_hundred_entities_can_be_added_and_removed(void **state) {
    (void) state;
    entity_t entityOk = {.owner = "Testuser", .name = "Test"};
    entity_t entityArray[100];
    for (int i = 0; i < 100; ++i) {
        entityArray[i] = entityOk;
    }
    // 100 mal hinzufügen und sofort wieder entfernen
    for (int i = 0; i < 100; ++i) {
        assert_int_equal(EntityHandler_AddEntity(&entityArray[i]), ERR_OK);
        assert_int_equal(EntityHandler_RemoveEntity(&entityArray[i]), ERR_OK);
    }
    // 100 mal hinzufügen und 1 mal alle gleichzeitig entfernen
    for (int i = 0; i < 100; ++i) {
        assert_int_equal(EntityHandler_AddEntity(&entityArray[i]), ERR_OK);
    }
    assert_int_equal(EntityHandler_RemoveAllEntities(), ERR_OK);
    // 100 mal hinzufügen und danach 100 mal entfernen
    for (int i = 0; i < 100; ++i) {
        assert_int_equal(EntityHandler_AddEntity(&entityArray[i]), ERR_OK);
    }
    for (int i = 0; i < 100; ++i) {
        assert_int_equal(EntityHandler_RemoveEntity(&entityArray[i]), ERR_OK);
    }
}

/**
 * @brief Setup: Entität erstellen und hinzufügen
 * 
 * @param[out] state Pointer auf entity_t
 */
static int setupOneEntity(void **state) {
    static entity_t entity = {.owner = "Testuser", .name = "Test", .parts = NULL};
    *state = (void*)&entity;
    return (EntityHandler_AddEntity(&entity) != ERR_OK);
}

/**
 * @brief Teardown: Entität entfernen
 * 
 * @param[in] state Pointer auf entity_t*
 */
static int teardownOneEntity(void **state) {
    entity_t *entity = (entity_t*)*state;
    int ret = EntityHandler_RemoveEntity(entity);
    entity = NULL;
    return ret != ERR_OK;
}

/**
 * @brief Ein Entitätsteil kann hinzugefügt und entfernt werden.
 * 
 * @param state Pointer auf entity_t*
 */
static void an_entitypart_can_be_added_and_removed(void **state) {
    entity_t *entity = (entity_t*)*state;
    entityPart_t part = {.name = "Test", .sprite.destination.h = 10, .sprite.destination.w = 10};
    assert_int_equal(EntityHandler_AddEntityPart(entity, &part), ERR_OK);
    assert_int_equal(EntityHandler_RemoveEntityPart(entity, &part), ERR_OK);
}

/**
 * @brief Ungültige Entitätsteile können nicht hinzugefügt werden.
 * 
 * @param state Pointer auf entity_t*
 */
static void invalid_entityparts_cant_be_added(void **state) {
    entity_t *entity = (entity_t*)*state;
    entityPart_t partOk = {.name = "Test", .sprite.destination.h = 10, .sprite.destination.w = 10};
    // NULL
    assert_int_not_equal(EntityHandler_AddEntityPart(entity, NULL), ERR_OK);
    // Entität ohne Namen
    entityPart_t partNoName = partOk;
    partNoName.name = NULL;
    assert_int_not_equal(EntityHandler_AddEntityPart(entity, &partNoName), ERR_OK);
}

/**
 * @brief Hundert Entitätsteile können hinzugefügt und entfernt werden.
 * 
 * @param state Pointer auf entity_t*
 */
static void one_hundred_entityparts_can_be_added_and_removed(void **state) {
    entity_t *entity = (entity_t*)*state;
    entityPart_t partOk = {.name = "Test", .sprite.destination.h = 10, .sprite.destination.w = 10};
    entityPart_t partArray[100];
    for (int i = 0; i < 100; ++i) {
        partArray[i] = partOk;
    }
    // 100 mal hinzufügen und sofort wieder entfernen
    for (int i = 0; i < 100; ++i) {
        assert_int_equal(EntityHandler_AddEntityPart(entity, &partArray[i]), ERR_OK);
        assert_int_equal(EntityHandler_RemoveEntityPart(entity, &partArray[i]), ERR_OK);
    }
    // 100 mal hinzufügen und 1 mal alle gleichzeitig entfernen
    for (int i = 0; i < 100; ++i) {
        assert_int_equal(EntityHandler_AddEntityPart(entity, &partArray[i]), ERR_OK);
    }
    assert_int_equal(EntityHandler_RemoveAllEntityParts(entity), ERR_OK);
    // 100 mal hinzufügen und danach 100 mal entfernen
    for (int i = 0; i < 100; ++i) {
        assert_int_equal(EntityHandler_AddEntityPart(entity, &partArray[i]), ERR_OK);
    }
    for (int i = 0; i < 100; ++i) {
        assert_int_equal(EntityHandler_RemoveEntityPart(entity, &partArray[i]), ERR_OK);
    }
}

/**
 * @brief Mockup eines onUpdate Callbacks
 * 
 * @param self Pointer auf die Entität dessen Callback gerade aufgerufen wird
 * @param inputEvents Eingabeevents für die Entität (bsp.: nach links oder nach rechts)
 * @return Fehlercode gemäss \ref will_return() von CMocka
 */
static int onUpdate(entity_t *self, inputEvent_t *inputEvents) {
    check_expected_ptr(self);
    check_expected_ptr(inputEvents);
    function_called();
    return mock_type(int);
}

/**
 * @brief Mockup eines onCollision Callbacks
 * 
 * @param self Pointer auf die Entität dessen Callback gerade aufgerufen wird
 * @return Fehlercode gemäss \ref will_return() von CMocka
 */
static int onCollision(entity_t *self) {
    check_expected_ptr(self);
    function_called();
    return mock_type(int);
}

/**
 * @brief Mockup eines onDraw Callbacks
 * 
 * @param self Pointer auf die Entität dessen Callback gerade aufgerufen wird
 * @return Fehlercode gemäss \ref will_return() von CMocka
 */
static int onDraw(entity_t *self) {
    check_expected_ptr(self);
    function_called();
    return mock_type(int);
}

int __wrap_SDLW_DrawTexture(sprite_t sprite) {
    function_called();
    return mock_type(int);
}

/**
 * @brief Setup: Entität und Einzelteil erstellen und hinzufügen
 * 
 * @param[out] state Pointer auf entity_t
 */
static int setupOneEntityAndOnePart(void **state) {
    static entity_t entity = {0};
    entity.owner = "Testuser";
    entity.name = "Test";
    entity.callbacks.onUpdate = onUpdate;
    entity.callbacks.onCollision = onCollision;
    entity.callbacks.onDraw = onDraw;
    entity.state = ENTITY_STATE_CREATED;
    static entityPart_t part = {0};
    part.name = "Test";
    part.sprite.destination.h = 10;
    part.sprite.destination.w = 10;
    *state = (void*)&entity;
    return (EntityHandler_AddEntity(&entity) != ERR_OK) || (EntityHandler_AddEntityPart(&entity, &part) != ERR_OK);
}

/**
 * @brief Teardown: Entität und Einzelteil entfernen
 * 
 * @param[in] state Pointer auf entity_t*
 */
static int teardownOneEntityAndOnePart(void **state) {
    entity_t *entity = (entity_t*)*state;
    int ret = EntityHandler_RemoveAllEntityParts(entity);
    if (!ret) {
        ret = EntityHandler_RemoveEntity(entity);
    }
    entity = NULL;
    return ret != ERR_OK;
}

/**
 * @brief Übergebenes Event per \ref EntityHandler_Update() wird an 
 * \ref entityCallback_t.onUpdate weitergereicht.
 * 
 * @param state Pointer auf entity_t*
 */
static void update_callback_gets_called_with_input(void **state) {
    entity_t *entity = (entity_t*)*state;
    inputEvent_t *inputEvents = (inputEvent_t*)0xDEADBEEF;
    expect_function_call(onUpdate);
    expect_value(onUpdate, self, entity);
    expect_value(onUpdate, inputEvents, inputEvents);
    will_return(onUpdate, ERR_OK);
    assert_int_equal(EntityHandler_Update(inputEvents), ERR_OK);
}

/**
 * @brief Fehler im \ref entityCallback_t.onUpdate wird von
 * \ref EntityHandler_Update() zurückgemeldet.
 *
 * @param state Pointer auf entity_t*
 */
static void error_in_update_callback_is_cascaded_up(void **state) {
    entity_t *entity = (entity_t*)*state;
    inputEvent_t *inputEvents = (inputEvent_t*)0xDEADBEEF;
    expect_function_call(onUpdate);
    expect_value(onUpdate, self, entity);
    expect_value(onUpdate, inputEvents, inputEvents);
    will_return(onUpdate, ERR_FAIL);
    assert_int_equal(EntityHandler_Update(inputEvents), ERR_FAIL);
}

/**
 * @brief Wird \ref EntityHandler_Draw() aufgerufen wird auch
 * \ref entityCallback_t.onDraw aufgerufen.
 * 
 * @param state Pointer auf entity_t*
 */
static void draw_callback_gets_called(void **state) {
    entity_t *entity = (entity_t*)*state;
    entity->state = ENTITY_STATE_ACTIVE; // Entität aktiv schalten
    // onDraw wird aufgerufen
    expect_function_call(onDraw);
    expect_value(onDraw, self, entity);
    will_return(onDraw, ERR_OK);
    assert_int_equal(EntityHandler_Draw(), ERR_OK);
}

/**
 * @brief Wird \ref EntityHandler_Draw() aufgerufen und ist kein onDraw
 * Callback definiert, wird die Standard-Zeichnen-Funktion verwendet. Diese
 * würde dann normalerweise \ref SDLW_DrawTexture() aufrufen.
 * 
 * @param state Pointer auf entity_t*
 */
static void default_draw_gets_called_if_no_callback_defined(void **state) {
    entity_t *entity = (entity_t*)*state;
    entity->state = ENTITY_STATE_ACTIVE; // Entität aktiv schalten
    entity->callbacks.onDraw = NULL; // Callback entfernen
    // SDLW_DrawTexture wird aufgerufen (durch die Standard Zeichnen Funktion)
    expect_function_call(__wrap_SDLW_DrawTexture);
    will_return(__wrap_SDLW_DrawTexture, ERR_OK);
    assert_int_equal(EntityHandler_Draw(), ERR_OK);
}

/**
 * @brief Fehler im \ref entityCallback_t.onDraw wird von
 * \ref EntityHandler_Draw() zurückgemeldet.
 *
 * @param state Pointer auf entity_t*
 */
static void error_in_draw_callback_is_cascaded_up(void **state) {
    entity_t *entity = (entity_t*)*state;
    entity->state = ENTITY_STATE_ACTIVE; // Entität aktiv schalten
    expect_function_call(onDraw);
    expect_value(onDraw, self, entity);
    will_return(onDraw, ERR_FAIL);
    assert_int_equal(EntityHandler_Draw(), ERR_FAIL);
}

/**
 * @brief Fehler in \ref entityCallback_t.onDraw wird von
 * \ref EntityHandler_Draw() zurückgemeldet.
 *
 * @param state Pointer auf entity_t*
 */
static void error_in_default_draw_is_cascaded_up(void **state) {
    entity_t *entity = (entity_t*)*state;
    entity->state = ENTITY_STATE_ACTIVE; // Entität aktiv schalten
    entity->callbacks.onDraw = NULL; // Callback entfernen
    expect_function_call(__wrap_SDLW_DrawTexture);
    will_return(__wrap_SDLW_DrawTexture, ERR_FAIL);
    assert_int_equal(EntityHandler_Draw(), ERR_FAIL);
}

/**
 * @brief Ist \ref entityState_t im Zustand \ref ENTITY_STATE_CREATED
 * so wird \ref entityCallback_t.onDraw nicht aufgerufen.
 * 
 * @param state Pointer auf entity_t*
 */
static void draw_callback_doesnt_get_called_if_state_is_equal_to_created(void **state) {
    entity_t *entity = (entity_t*)*state;
    assert_int_equal(EntityHandler_Draw(), ERR_OK);
}

/**
 * @brief Testprogramm
 * 
 * @return int Anzahl fehlgeschlagener Tests
 */
int main(void) {
    const struct CMUnitTest entityHandler[] = {
        cmocka_unit_test(an_entity_can_be_added_and_removed),
        cmocka_unit_test(invalid_entities_cant_be_added),
        cmocka_unit_test(one_hundred_entities_can_be_added_and_removed),

        cmocka_unit_test_setup_teardown(an_entitypart_can_be_added_and_removed, setupOneEntity, teardownOneEntity),
        cmocka_unit_test_setup_teardown(invalid_entityparts_cant_be_added, setupOneEntity, teardownOneEntity),
        cmocka_unit_test_setup_teardown(one_hundred_entityparts_can_be_added_and_removed, setupOneEntity, teardownOneEntity),

        cmocka_unit_test_setup_teardown(update_callback_gets_called_with_input, setupOneEntityAndOnePart, teardownOneEntityAndOnePart),
        cmocka_unit_test_setup_teardown(error_in_update_callback_is_cascaded_up, setupOneEntityAndOnePart, teardownOneEntityAndOnePart),
        cmocka_unit_test_setup_teardown(draw_callback_gets_called, setupOneEntityAndOnePart, teardownOneEntityAndOnePart),
        cmocka_unit_test_setup_teardown(default_draw_gets_called_if_no_callback_defined, setupOneEntityAndOnePart, teardownOneEntityAndOnePart),
        cmocka_unit_test_setup_teardown(error_in_draw_callback_is_cascaded_up, setupOneEntityAndOnePart, teardownOneEntityAndOnePart),
        cmocka_unit_test_setup_teardown(error_in_default_draw_is_cascaded_up, setupOneEntityAndOnePart, teardownOneEntityAndOnePart),
        cmocka_unit_test_setup_teardown(draw_callback_doesnt_get_called_if_state_is_equal_to_created, setupOneEntityAndOnePart, teardownOneEntityAndOnePart),
    };
    return cmocka_run_group_tests(entityHandler, NULL, NULL);
}
