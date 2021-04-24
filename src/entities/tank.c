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

#include "entities/tank.h"
#include "error.h"
#include "physics.h"
#include "entityHandler.h"
#include "world.h"
#include "entities/shell.h"


/*
 * Typdeklarationen
 * 
 */

/**
 * @brief Enummeration der Panzer-Teile
 * 
 * Dient als Arrayindex für die Eintelteile des Panzers.
 */
typedef enum {
    TANK_PART_TRACKS = 0,
    TANK_PART_CHASSIS,
    TANK_PART_TUBE
} tankPartType_t;


/*
 * Variablendeklarationen
 * 
 */

/**
 * @brief Maximale horizontale Geschwindigkeit
 * 
 * Je schneller ein Panzer sich bewegen darf, dessto grösser die Steigungen der
 * Welt die er zurücklegen kann.
 */
#define TANK_MAX_HORIZONTAL_SPEED 50.0f
#define TANK_TUBE_MAX_ROTATION_POSITIVE 10.0 //!< Max pos. Rotation des Rohrs
#define TANK_TUBE_MAX_ROTATION_NEGATIVE -190.0 //!< Max neg. Rotation des Rohrs


/*
 * Private Funktionsprototypen
 * 
 */

static int updateCallback(entity_t *self, inputEvent_t *inputEvents);
static int collisionCallback(entity_t *self, entityCollision_t *collision);
static int rotateToWorld(entity_t *tank);


/*
 * Implementation Öffentlicher Funktionen
 * 
 */

int Tank_Create(const char *player, float x, float y) {
    if (!player) {
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
    tank->physics.aabb.w = 40;
    tank->physics.aabb.h = 30;
    if (Physics_SetPosition(tank, x, y)) {
        goto errorStep2;
    }
    if (EntityHandler_AddEntity(tank)) {
        goto errorStep2;
    }
    // Einzelteile laden und einrichten
    sprite_t *rawSprite;
    entityPart_t *parts = calloc(3, sizeof(entityPart_t));
    if (!parts) {
        goto errorStep2;
    }
    tank->data = parts; // speichere Pointer auf Teile für späteren Zugriff
    // Raupen laden
    SDLW_GetResource("tracks", RESOURCETYPE_SPRITE, (void **)&rawSprite);
    if (!rawSprite) {
        goto errorStep3;
    }
    parts[TANK_PART_TRACKS].name = "Raupen";
    parts[TANK_PART_TRACKS].sprite = *rawSprite;
    if (EntityHandler_AddEntityPart(tank, &parts[TANK_PART_TRACKS])) {
        goto errorStep3;
    }
    // Fahrgestell laden
    SDLW_GetResource("chassis", RESOURCETYPE_SPRITE, (void **)&rawSprite);
    if (!rawSprite) {
        goto errorStep4;
    }
    parts[TANK_PART_CHASSIS].name = "Fahrgestell";
    parts[TANK_PART_CHASSIS].sprite = *rawSprite;
    if (EntityHandler_AddEntityPart(tank, &parts[TANK_PART_CHASSIS])) {
        goto errorStep4;
    }
    // Panzerrohr laden
    SDLW_GetResource("tube", RESOURCETYPE_SPRITE, (void **)&rawSprite);
    if (!rawSprite) {
        goto errorStep5;
    }
    parts[TANK_PART_TUBE].name = "Rohr";
    parts[TANK_PART_TUBE].sprite = *rawSprite;
    if (EntityHandler_AddEntityPart(tank, &parts[TANK_PART_TUBE])) {
        goto errorStep5;
    }
    // Teile einrichten
    return ERR_OK;

    // Mache im Fehlerfall alle Schritte einzeln rückgängig
errorStep5:
    EntityHandler_RemoveEntityPart(tank, &parts[TANK_PART_CHASSIS]);
errorStep4:
    EntityHandler_RemoveEntityPart(tank, &parts[TANK_PART_TRACKS]);
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

static int updateCallback(entity_t *self, inputEvent_t *inputEvents) {
    entityPart_t *tube = &((entityPart_t*)self->data)[TANK_PART_TUBE];
    const float *vx = &self->physics.velocity.x;
    // horizontale Geschwindigkeit gemäss WASD-Tasten verändern
    if (inputEvents->dummy == -1 && *vx > -TANK_MAX_HORIZONTAL_SPEED) {
        Physics_SetRelativeVelocity(self, -10.0f, NAN);
    } else if (inputEvents->dummy == 1 && *vx < TANK_MAX_HORIZONTAL_SPEED) {
        Physics_SetRelativeVelocity(self, +10.0f, NAN);
    }
    // Rohrstellung gemäss Pfeiltasten rotieren
    if (inputEvents->dummy == -2
     && tube->sprite.rotation > TANK_TUBE_MAX_ROTATION_NEGATIVE) {
        tube->sprite.rotation -= 0.5;
    } else if (inputEvents->dummy == 2
            && tube->sprite.rotation < TANK_TUBE_MAX_ROTATION_POSITIVE) {
        tube->sprite.rotation += 0.5;
    }
    // Horizontale Bewegung in jedem Zyklus um 5% dämpfen damit der Panzer nicht
    // endlos in eine Richtung fährt.
    Physics_SetVelocity(self, *vx * 0.95f, NAN);
    // rotiere den gesamten Panzer basierend auf der Position auf der Welt
    rotateToWorld(self);
    // Falls Leertaste -> Schuss feuern
    if (inputEvents->dummy == 4) {
        // Der Schuss soll am Ende des Schussrohrs erscheinen, daher muss dessen
        // aktuelle Position mit Vektorgeometrie berechnet werden.
        // Starte im Zentrum der Entität
        float x = self->physics.position.x;
        float y = self->physics.position.y;
        // Startpunkt zum Rohransatz schieben
        double angleRad = (self->physics.rotation + 90.0) * (M_PI / 180.0);
        x += tube->sprite.destination.y * cos(-angleRad);
        y += -(tube->sprite.destination.y * sin(-angleRad));
        // Startpunkt zum Rohrende schieben
        angleRad = (self->physics.rotation + tube->sprite.rotation) * (M_PI / 180.0);
        x += tube->sprite.destination.w * cos(-angleRad);
        y += -(tube->sprite.destination.w * sin(-angleRad));
        // Schuss erstellen
        double angle = self->physics.rotation + tube->sprite.rotation;
        Shell_Create(self->owner, x, y, 200.0f, angle);
    }
    return ERR_OK;
}

static int collisionCallback(entity_t *self, entityCollision_t *collision) {
    // Kollisionen mit Entiäten die dem selben Spieler gehören ignorieren.
    if (collision->flags & ENTITY_COLLISION_ENTITY) {
        if (!strcmp(self->owner, collision->partner->owner)) {
            collision->flags &= ~ENTITY_COLLISION_ENTITY;
        }
    }
    return ERR_OK;
}

static int rotateToWorld(entity_t *tank) {
    // Suche den Punkt auf der Welt der direkt unter der Mitte der Entität ist.
    SDL_Point topOfWorld;
    World_VerticalLineIntersection((SDL_Point){
        .x = tank->physics.position.x,
        .y = tank->physics.position.y},
        &topOfWorld);
    // Erhalte mittels Kollision in diesem Punkt eine Normale der Welt.
    SDL_Rect testBox = {.x = topOfWorld.x - 10, .y = topOfWorld.y - 10, .w = 20, .h = 20};
    entityCollision_t collision = {.partner = NULL};
    World_CheckCollision(testBox, &collision);
    if (collision.flags & ENTITY_COLLISION_WORLD) {
        double angle = atan2(-collision.normal.y, collision.normal.x);
        angle = angle / M_PI * 180.0;
        angle -= 90.0;
        //double angle = atan(collision.normal.y / collision.normal.x);
        Physics_SetRotation(tank, -angle);
    }
    SDLW_DrawFilledRect(testBox, (SDL_Color){255, 0, 0, 0});
    return ERR_OK;
}
