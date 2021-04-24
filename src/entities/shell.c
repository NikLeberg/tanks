/**
 * @file shell.h
 * @author Leuenberger Niklaus (leuen4@bfh.ch)
 * @brief Panzerschussentität
 * @version 0.1
 * @date 2021-04-23
 * 
 * @copyright Copyright (c) 2021 Leuenberger Niklaus
 * 
 */


/*
 * Includes
 * 
 */

#include <math.h>

#include "entities/tank.h"
#include "error.h"
#include "physics.h"
#include "entityHandler.h"
#include "world.h"


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

static int updateCallback(entity_t *self, inputEvent_t *inputEvents);
static int collisionCallback(entity_t *self, entityCollision_t *collision);


/*
 * Implementation Öffentlicher Funktionen
 * 
 */

int Shell_Create(const char *player, float x, float y, float velocity, double angle) {
    SDL_Log("erstelle Shell\n");
    if (!player) {
        return ERR_PARAMETER;
    }
    entity_t *shell = calloc(1, sizeof(entity_t));
    if (!shell) {
        goto errorStep1;
    }
    // Hauptentität einrichten
    shell->owner = player;
    shell->name = "Panzerschuss";
    shell->callbacks.onUpdate = updateCallback;
    shell->callbacks.onCollision = collisionCallback;
    shell->physics.aabb.w = 5;
    shell->physics.aabb.h = 5;
    if (Physics_SetPosition(shell, x, y)
     || Physics_SetVelocityPolar(shell, velocity, angle)) {
        goto errorStep2;
    }
    if (EntityHandler_AddEntity(shell)) {
        goto errorStep2;
    }
    // Teil resp. Sprite laden und einrichten
    sprite_t *rawSprite;
    entityPart_t *part = calloc(1, sizeof(entityPart_t));
    if (!part) {
        goto errorStep2;
    }
    shell->data = part; // speichere Pointer auf Teil für späteren Zugriff
    SDLW_GetResource("shell", RESOURCETYPE_SPRITE, (void **)&rawSprite);
    if (!rawSprite) {
        goto errorStep3;
    }
    part->name = "Schuss";
    part->sprite = *rawSprite;
    if (EntityHandler_AddEntityPart(shell, part)) {
        goto errorStep3;
    }
    return ERR_OK;

    // Mache im Fehlerfall alle Schritte einzeln rückgängig
errorStep3:
    EntityHandler_RemoveEntity(shell);
    free(part);
errorStep2:
    free(shell);
errorStep1:
    return ERR_FAIL;
}

int Shell_Destroy(entity_t *shell) {
    SDL_Log("zerstöre Shell\n");
    EntityHandler_RemoveAllEntityParts(shell);
    free(shell->data);
    EntityHandler_RemoveEntity(shell);
    free(shell);
    return ERR_OK;
}


/*
 * Implementation Privater Funktionen
 * 
 */

static int updateCallback(entity_t *self, inputEvent_t *inputEvents) {
    (void)inputEvents;
    // richte Schuss der Flugbahn aus
    double angle = atan2(-self->physics.velocity.y, self->physics.velocity.x);
    angle = angle / M_PI * 180.0;
    Physics_SetRotation(self, -angle);
    return ERR_OK;
}

static int collisionCallback(entity_t *self, entityCollision_t *collision) {
    (void)self;
    // Ignoriere Kollisionen mit der oberen Bildkante, der Schuss wird von
    // alleine wieder herunter kommen.
    collision->flags &= ~ENTITY_COLLISION_BORDER_TOP;
    // Lösche Entität bei Aufprall mit der Welt
    if (collision->flags & ENTITY_COLLISION_WORLD) {
        Shell_Destroy(self);
    }
    // Kollisionen mit Entiäten die dem selben Spieler gehören ignorieren.
    if (collision->flags & ENTITY_COLLISION_ENTITY) {
        if (!strcmp(self->owner, collision->partner->owner)) {
            collision->flags &= ~ENTITY_COLLISION_ENTITY;
        }
    }
    return ERR_OK;
}
