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
    list_t *entityList; //!< Liste der aktiven Entitäten
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
static int validateEntity(entity_t *entity);

/**
 * @brief Validation einer Entitätsteilstruktur.
 *
 * Validiert die Felder der Entitätsteilstruktur auf korrekte Parameter.
 * 
 * @param part zu prüfendes Einzelteil
 *
 * @return ERR_OK oder ERR_FAIL
 */
static int validatePart(entityPart_t *part);

/**
 * @brief Rufe onUpdate Callbacks auf.
 *
 * Wird durch List_ForeachArg() für jede Entität der Liste aufgerufen und
 * führt dann \ref entityCallbacks_t.onUpdate aus, falls vorhanden.
 * 
 * @param data opaker Pointer auf eine Entität
 * @param userData opaker Pointer auf Eingabeevents
 *
 * @return ERR_OK oder Fehler des Entity-Callbacks
 */
static int callOnUpdate(void *data, void *userData);

/**
 * @brief Berechne die Positionen der Einzelteile.
 *
 * Jede Entität kann aus mehreren Einzelteilen aufgebaut sein. Diese
 * sind relativ zur Entität positioniert. Daher müssen deren abolute
 * Koordinaten aus den Grundkoordinaten der Entität und den relativen
 * Angaben berechnet werden.
 * 
 * @param data opaker Pointer auf eine Entität
 *
 * @return ERR_OK oder ERR_FAIL falls Rechnung ungültig wurde
 */
static int calculatePartsPositions(void *data);

/**
 * @brief Berechne die Position eines Einzelteils.
 *
 * Errechnet anhand der absoluten Position der gesamten Entität und den
 * relativen Angaben in den Einzelteilen die tatsächlichen absoluten
 * Koordinaten des Einzelteils.
 * 
 * @param data opaker Pointer auf Entitätsteil
 * @param userData Pointer auf übergeordnete Entität
 *
 * @return immer ERR_OK
 */
static int calculatePartPosition(void *data, void *userData);

/**
 * @brief Rufe onDraw Callbacks auf.
 *
 * Wird durch List_Foreach() für jede Entität der Liste aufgerufen und
 * führt dann \ref entityCallbacks_t.onDraw aus, falls vorhanden. Ist der
 * Callback NULL, so wird \ref defaultDrawEntity() aufgerufen.
 * 
 * @param data opaker Pointer auf eine Entität
 *
 * @return ERR_OK oder Fehler des Entity-Callbacks
 */
static int callOnDraw(void *data);

/**
 * @brief Standart Zeichnungsfunktion.
 * 
 * @note Die Reihenfolge in der die Entitäten gezeichnet werden ist durch
 * ihre Position innerhalb der Liste vorgegeben. Neue Entitäten werden am
 * Anfang der Liste hinzugefügt. In der Standardmässigen defaultDrawEntity()
 * wird über die Liste von vorne her iteriert. Daher werden die später
 * hinzugefügten Entitäten zuerst gezeichnet.
 * 
 * @param entity zu zeichnende Entität
 *
 * @return ERR_OK oder ERR_FAIL
 */
static int defaultDrawEntity(entity_t *entity);

/**
 * @brief Standart Zeichnungsfunktion für ein Einzelteil.
 * 
 * @note Die Reihenfolge in der die Einzelteile gezeichnet werden ist durch
 * ihre Position innerhalb der Liste vorgegeben. Neue Teile werden am Anfang
 * der Liste hinzugefügt. In der Standardmässigen defaultDrawEntity() wird über
 * die Liste von vorne her iteriert. Daher werden die ersten Teile zuerst
 * gezeichnet. Spätere überzeichnen vorher gezeichnete.
 * 
 * @param data Pointer auf zu zeichnendes Einzelteil
 * @param userData Pointer auf übergeordnete Entität
 *
 * @return ERR_OK oder Fehler von \ref SDLW_DrawTexture()
 */
static int defaultDrawEntityPart(void *data, void *userData);


/*
 * Implementation Öffentlicher Funktionen
 * 
 */

int EntityHandler_Update(inputEvent_t *inputEvents) {
    int ret = ERR_OK;
    // Alle Entitäten aktualisieren
    ret = List_ForeachArg(entityHandler.entityList, callOnUpdate, inputEvents);
    if (ret) {
        return ret;
    }
    // Physik aktualisieren
    ret = Physics_Update(entityHandler.entityList);
    if (ret) return ret;
    // Positionen der Einzelteile neu berechnen
    ret = List_Foreach(entityHandler.entityList, calculatePartsPositions);
    return ret;
}

int EntityHandler_Draw() {
    return List_Foreach(entityHandler.entityList, callOnDraw);
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
    if (!entityHandler.entityList || !entityHandler.entityList->listHead) {
        return ERR_OK;
    }
    do {
        entity_t *entity = entityHandler.entityList->listHead->data;
        EntityHandler_RemoveAllEntityParts(entity);
        List_Remove(entityHandler.entityList, entity);
    } while (entityHandler.entityList->listHead);
    List_Destroy(&entityHandler.entityList);
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
    if (entity && entity->parts) {
        List_Destroy(&entity->parts);
    }
    return ERR_OK;
}


/*
 * Implementation Privater Funktionen
 * 
 */

static int validateEntity(entity_t *entity) {
    if (!entity) {
        return ERR_PARAMETER;
    }
    int ret = ERR_OK;
    if (entity->state != ENTITY_STATE_CREATED // Zustand muss CREATED sein
        || !entity->owner || !entity->name) { // Muss Eigentümer und Namen haben
        ret = ERR_FAIL;
    }
    return ret;
}

static int validatePart(entityPart_t *part) {
    if (!part) {
        return ERR_PARAMETER;
    }
    int ret = ERR_OK;
    if (!part->sprite.texture          // Textur des Sprites muss gesetzt sein
        || !part->sprite.destination.w // Breite muss bekannt sein
        || !part->sprite.destination.h // Höhe muss bekannt sein
        || !part->name) {              // Muss Namen haben
        ret = ERR_FAIL;
    }
    return ret;
}

static int callOnUpdate(void *data, void *userData) {
    entity_t *entity = (entity_t *)data;
    inputEvent_t *inputEvents = (inputEvent_t *)userData;
    int ret = ERR_OK;
    entity->state = ENTITY_STATE_ACTIVE;
    if (entity->callbacks.onUpdate) { // Callback darf NULL sein
        ret = entity->callbacks.onUpdate(entity, inputEvents);
    }
    return ret;
}

static int calculatePartsPositions(void *data) {
    entity_t *entity = (entity_t *)data;
    int ret = ERR_OK;
    if (entity->parts) {
        ret = List_ForeachArg(entity->parts, calculatePartPosition, entity);
    }
    return ret;
}

static int calculatePartPosition(void *data, void *userData) {
    entityPart_t *entityPart = (entityPart_t *)data;
    entity_t *entity = (entity_t *)userData;
    // Aktualisiere Position des Teils mit Position der gesamten Entität.
    entityPart->sprite.position.x = entity->physics.position.x;
    entityPart->sprite.position.y = entity->physics.position.y;
    // Position des Teils gemäss des eigenen Pivots & Rotation und
    // der Rotation der gesamten Entität verschieben.
    int ret = ERR_OK;
    ret = Sprite_SetRelativeToPivot(entityPart->sprite,
                                    entity->physics.rotation,
                                    (SDL_Point){.x = 0, .y = 0},
                                    &entityPart->tempSprite);
    return ret;
}

static int callOnDraw(void *data) {
    entity_t *entity = (entity_t *)data;
    int ret = ERR_OK;
    if (entity->state == ENTITY_STATE_ACTIVE) {
        // Callback darf NULL sein, dann wird Standardfunktion verwendet.
        if (entity->callbacks.onDraw) {
            ret = entity->callbacks.onDraw(entity);
        } else {
            ret = defaultDrawEntity(entity);
        }
    }
    return ret;
}

static int defaultDrawEntity(entity_t *entity) {
    return List_ForeachArg(entity->parts, defaultDrawEntityPart, entity);
}

static int defaultDrawEntityPart(void *data, void *userData) {
    entityPart_t *part = (entityPart_t *)data;
    entity_t *entity = (entity_t *)userData;
    (void)entity;
    return SDLW_DrawTexture(part->tempSprite);
}
