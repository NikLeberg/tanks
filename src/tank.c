/**
 * @file tank.c
 * @author Leuenberger Niklaus (leuen4@bfh.ch)
 * @brief Panzerentität
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

#include <math.h>

#include "tank.h"
#include "error.h"
#include "physics.h"
#include "entityHandler.h"


/*
 * Typdeklarationen
 * 
 */

/* ... */


/*
 * Variablendeklarationen
 * 
 */

/* ... */


/*
 * Private Funktionsprototypen
 * 
 */

int updateCallback(entity_t *self, inputEvent_t *inputEvents);
int collisionCallback(entity_t *self, entityCollision_t *collision);


/*
 * Implementation Öffentlicher Funktionen
 * 
 */

int Tank_Create(const char *player, SDL_Point startPosition) {
    if (!player || !startPosition.x || !startPosition.y) {
        return ERR_PARAMETER;
    }
    entity_t *tank = calloc(1, sizeof(entity_t));
    if (!tank) {
        goto errorStep1;
    }
    // Hauptentität einrichten
    tank->owner = player;
    tank->name = "Panzer";
    tank->callbacks.onUpdate = updateCallback;
    tank->callbacks.onCollision = collisionCallback;
    tank->physics.aabb.w = 35;
    tank->physics.aabb.h = 20;
    if (Physics_SetPosition(tank, startPosition.x, startPosition.y)) {
        goto errorStep2;
    }
    if (EntityHandler_AddEntity(tank)) {
        goto errorStep2;
    }
    // Teile laden und einrichten
    sprite_t *rawSprite;
    entityPart_t *parts = calloc(3, sizeof(entityPart_t));
    if (!parts) {
        goto errorStep2;
    }
    tank->data = parts; // speichere Pointer für späteres löschen
    // Fahrgestell laden
    SDLW_GetResource("chassis", RESOURCETYPE_SPRITE, (void **)&rawSprite);
    parts[0].name = "Fahrgestell";
    parts[0].sprite = *rawSprite;
    if (EntityHandler_AddEntityPart(tank, &parts[0])) {
        goto errorStep3;
    }
    // Raupen laden
    SDLW_GetResource("tracks", RESOURCETYPE_SPRITE, (void **)&rawSprite);
    parts[1].name = "Raupen";
    parts[1].sprite = *rawSprite;
    if (EntityHandler_AddEntityPart(tank, &parts[1])) {
        goto errorStep4;
    }
    // Panzerrohr laden
    SDLW_GetResource("tube", RESOURCETYPE_SPRITE, (void **)&rawSprite);
    parts[2].name = "Rohr";
    parts[2].sprite = *rawSprite;
    if (EntityHandler_AddEntityPart(tank, &parts[2])) {
        goto errorStep5;
    }
    // Teile einrichten
    return ERR_OK;

    // Mache im Fehlerfall alle Schritte einzeln rückgängig
errorStep5:
    EntityHandler_RemoveEntityPart(tank, &parts[1]); // Raupen
errorStep4:
    EntityHandler_RemoveEntityPart(tank, &parts[0]); // Fahrgestell
errorStep3:
    EntityHandler_RemoveEntity(tank);
    free(parts);
errorStep2:
    free(tank);
errorStep1:
    return ERR_FAIL;
}

int Tank_Destroy(entity_t *tank) {
    EntityHandler_RemoveAllEntityParts(tank);
    free(tank->data);
    EntityHandler_RemoveEntity(tank);
    free(tank);
    return ERR_OK;
}


/*
 * Implementation Privater Funktionen
 * 
 */

int updateCallback(entity_t *self, inputEvent_t *inputEvents) {
    float *x = &self->physics.velocity.x;
    // Geschwindigkeit gemäss Pfeiltasten verändern
    if (inputEvents->dummy == -1) {
        if (*x > -50.0f) *x -= 10.0f;
    } else if (inputEvents->dummy == 1) {
        if (*x < 50.0f) *x += 10.0f;
    }
    *x *= 0.95; // dämpfen
    return ERR_OK;
}

int collisionCallback(entity_t *self, entityCollision_t *collision) {
    (void)self;
    (void)collision;
    // // Richte den Panzer nach der Welt aus
    // if (collision->flags & ENTITY_COLLISION_WORLD) {
    //     double angle = atan2(-collision->normal.y, collision->normal.x);
    //     angle = angle / M_PI * 180.0f;
    //     angle -= 90.0f;
    //     //double angle = atan(collision->normal.y / collision->normal.x);
    //     Physics_SetRotation(self, -angle);
    // }
    return ERR_OK;
}
