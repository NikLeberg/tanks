/**
 * @file entityHandler.c
 * @author Leuenberger Niklaus (leuen4@bfh.ch)
 * @brief Verwaltet alle Entitäten
 * @version 0.1
 * @date 2021-03-19
 * 
 * @copyright Copyright (c) 2021 Leuenberger Niklaus
 * 
 */


/*
 * Includes
 * 
 */

#include "error.h"
#include "list.h"
#include "physics.h"
#include "entityHandler.h"

#include <assert.h>
#include <stdbool.h>


/*
 * Typdeklarationen
 * 
 */

/* ... */


/*
 * Variablendeklarationen
 * 
 */

/**
 * @brief Globale Variablen des EntityHandlers
 * 
 */
static struct {
    list_t *entityList;          //!< Liste der aktiven Entitäten
    inputEvent_t *currentEvents; //!< Aktive Events, nur während Update-Phase gültig
    entity_t *currentEntity;     //!< aktuelle Entität für List_Foreach - Callback
} entityHandler;


/*
 * Private Funktionsprototypen
 * 
 */

/**
 * @brief Validation einer Entitätsstruktur.
 * Validiert die Felder der Entitätsstruktur auf korrekte Parameter.
 * 
 * @param entity zu prüfende Entität
 *
 * @return ERR_OK oder ERR_FAIL
 */
int validateEntity(entity_t *entity);

/**
 * @brief Validation einer Entitätsteilstruktur.
 *
 * Validiert die Felder der Entitätsteilstruktur auf korrekte Parameter.
 * 
 * @param part zu prüfendes Einzelteil
 *
 * @return ERR_OK oder ERR_FAIL
 */
int validatePart(entityPart_t *part);

/**
 * @brief Rufe onUpdate Callbacks auf.
 *
 * Wird durch List_Foreach() für jede Entität der Liste aufgerufen und
 * führt dann \ref entityCallbacks_t.onUpdate aus, falls vorhanden.
 * 
 * @param data Pointer auf eine Entität
 *
 * @return ERR_OK oder Fehler des Entity-Callbacks
 */
int callOnUpdate(void *data);

/**
 * @brief Berechne die Positionen der Einzelteile.
 *
 * Jede Entität kann aus mehreren Einzelteilen aufgebaut sein. Diese
 * sind relativ zur Entität positioniert. Daher müssen deren abolute
 * Koordinaten aus den Grundkoordinaten der Entität und den relativen
 * Angaben berechnet werden.
 * 
 * @param data Pointer auf eine Entität
 *
 * @return ERR_OK oder ERR_FAIL falls Rechnung ungültig wurde
 */
int calculatePartsPositions(void *data);

/**
 * @brief Berechne die Position eines Einzelteils.
 *
 * Errechnet anhand der absoluten Position der gesamten Entität und den
 * relativen Angaben in den Einzelteilen die tatsächclichen absoluten
 * Koordinaten des Einzelteils.
 * 
 * @param data Pointer auf Entitätsteil
 *
 * @return immer ERR_OK
 */
int calculatePartPosition(void *data);

/**
 * @brief Rufe onDraw Callbacks auf.
 *
 * Wird durch List_Foreach() für jede Entität der Liste aufgerufen und
 * führt dann \ref entityCallbacks_t.onDraw aus, falls vorhanden. Ist der
 * Callback NULL, so wird \ref defaultDrawEntity() aufgerufen.
 * 
 * @param data Pointer auf eine Entität
 *
 * @return ERR_OK oder Fehler des Entity-Callbacks
 */
int callOnDraw(void *data);

/**
 * @brief Standart Zeichnungsfunktion.
 * 
 * @note Die Reihenfolde in der die Entitäten gezeichnet werden ist
 * durch ihre Position innerhalb der Liste vorgegeben.
 * 
 * @param entity zu zeichnende Entität
 *
 * @return ERR_OK oder ERR_FAIL
 */
int defaultDrawEntity(entity_t *entity);

/**
 * @brief Standart Zeichnungsfunktion für ein Einzelteil.
 * 
 * @note Die Reihenfolde in der die Einzelteile gezeichnet werden ist
 * durch ihre Position innerhalb der Liste vorgegeben.
 * 
 * @param data Pointer auf zu zeichnendes Einzelteil
 *
 * @return ERR_OK oder Fehler von \ref SDLW_DrawTexture()
 */
int defaultDrawEntityPart(void *data);


/*
 * Implementation Öffentlicher Funktionen
 * 
 */

int EntityHandler_Update(inputEvent_t *inputEvents) {
    // Alle Entitäten aktualisieren
    entityHandler.currentEvents = inputEvents;
    assert(List_Foreach(entityHandler.entityList, callOnUpdate) == 0);
    entityHandler.currentEvents = NULL;
    // Physik aktualisieren
    Physics_Update(entityHandler.entityList);
    // Positionen der Einzelteile neu berechnen
    assert(List_Foreach(entityHandler.entityList, callOnUpdate) == 0);
    return ERR_OK;
}

int EntityHandler_Draw() {
    assert(List_Foreach(entityHandler.entityList, callOnDraw) == 0);
    return ERR_OK;
}

int EntityHandler_AddEntity(entity_t *entity) {
    if (validateEntity(entity)) {
        return ERR_PARAMETER;
    }
    // Lazy-Init der Liste falls diese leer ist
    if (!entityHandler.entityList) {
        assert(List_Create(&entityHandler.entityList) == 0);
    }
    return List_Add(entityHandler.entityList, entity);
}

int EntityHandler_RemoveEntity(entity_t *entity) {
    return List_Remove(entityHandler.entityList, entity);
}

int EntityHandler_RemoveAllEntities() {
    entity_t *entity;
    do {
        entity = entityHandler.entityList->listHead->data;
        EntityHandler_RemoveAllEntityParts(entity);
        List_Remove(entityHandler.entityList, entity);
    } while (entity);
    List_Destroy(entityHandler.entityList);
    entityHandler.entityList = NULL;
    return ERR_OK;
}

int EntityHandler_AddEntityPart(entity_t *entity, entityPart_t *part) {
    if (validatePart(part)) {
        return ERR_PARAMETER;
    }
    // Lazy-Init der Liste falls diese leer ist
    if (!entity->parts) {
        assert(List_Create(&entity->parts) == 0);
    }
    return List_Add(entity->parts, part);
}

int EntityHandler_RemoveEntityPart(entity_t *entity, entityPart_t *part) {
    return List_Remove(entity->parts, part);
}

int EntityHandler_RemoveAllEntityParts(entity_t *entity) {
    if (entity) {
        List_Destroy(entity->parts);
        entity->parts = NULL;
    }
    return ERR_OK;
}


/*
 * Implementation Privater Funktionen
 * 
 */

int validateEntity(entity_t *entity) {
    if (!entity) {
        return ERR_PARAMETER;
    }
    int ret = ERR_OK;
    if (entity->state != ENTITY_STATE_CREATED // Zustand muss CREATED sein
    || !entity->owner || !entity->name) {  // Muss Eigentümer und Namen haben
        ret = ERR_FAIL;
    }
    return ret;
}

int validatePart(entityPart_t *part) {
    if (!part) {
        return ERR_PARAMETER;
    }
    int ret = ERR_OK;
    if (!part->sprite.destination->w || !part->sprite.destination->h // Grösse muss bekannt sein
    || !part->name) { // Muss Namen haben
        ret = ERR_FAIL;
    }
    return ret;
}

int callOnUpdate(void *data) {
    entity_t *entity = (entity_t*)data;
    int ret = ERR_OK;
    entity->state = ENTITY_STATE_ACTIVE;
    if (entity->callbacks.onUpdate) { // Callback darf NULL sein
        ret = entity->callbacks.onUpdate(entity, entityHandler.currentEvents);
    }
    return ret;
}

int calculatePartsPositions(void *data) {
    entity_t *entity = (entity_t*)data;
    if (entity->parts) {
        entityHandler.currentEntity = entity;
        assert(List_Foreach(entity->parts, calculatePartPosition) == 0);
        entityHandler.currentEntity = NULL;
    }
    return ERR_OK;
}

int calculatePartPosition(void *data) {
    entityPart_t *entityPart = (entityPart_t*)data;
    // Absolute Position des Sprites
    entityPart->sprite.positionX = entityHandler.currentEntity->physics.position.x;
    entityPart->sprite.positionY = entityHandler.currentEntity->physics.position.y;
    // Relative Position des Sprites
    entityPart->sprite.destination->x = entityPart->relativePosition.x; // Nötig?
    entityPart->sprite.destination->y = entityPart->relativePosition.y;
    // Rotation
    entityPart->sprite.rotation = entityPart->relativeRotation;
    entityPart->sprite.rotation += entityHandler.currentEntity->physics.rotation; // Nötig?
    // Rotationsachse
    entityPart->sprite.pivot->x = entityHandler.currentEntity->physics.position.x;
    entityPart->sprite.pivot->y = entityHandler.currentEntity->physics.position.y;
    return ERR_OK;
}

int callOnDraw(void *data) {
    entity_t *entity = (entity_t*)data;
    int ret = ERR_OK;
    if (entity->state == ENTITY_STATE_ACTIVE) {
        // Wenn Callback NULL ist, dann wird die Standard Zeichnen Funktion verwendet.
        if (entity->callbacks.onDraw) {
            ret = entity->callbacks.onDraw(entity);
        } else {
            ret = defaultDrawEntity(entity);
        }
    }
    return ret;
}

int defaultDrawEntity(entity_t *entity) {
    entityHandler.currentEntity = entity;
    assert(List_Foreach(entity->parts, defaultDrawEntityPart) == 0);
    entityHandler.currentEntity = NULL;
    return ERR_OK;
}

int defaultDrawEntityPart(void *data) {
    entityPart_t *part = (entityPart_t*)data;
    return SDLW_DrawTexture(part->sprite);
}
