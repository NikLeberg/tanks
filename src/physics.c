/**
 * @file physics.c
 * @author Leuenberger Niklaus (leuen4@bfh.ch)
 * @brief Verarbeitet die Physik der Entitäten
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

#include <stdbool.h>
#include <math.h>

#include "error.h"
#include "list.h"
#include "physics.h"


/*
 * Typdeklarationen
 * 
 */

/* ... */


/*
 * Variablendeklarationen
 * 
 */

#define DELTA_TIME (1.0f / 60.0f) //!< Updateintervall
#define GRAVITY 10.0f             //!< Erdbeschleunigung [pixel / s2]
#define NEAR_ZERO 0.01f           //!< Werte die kleiner sind zählen als 0
#define BOUNCE_BACK_FACTOR 0.5f   //!< Wie stark der Rückstoss einer Kollisionsüberlappung gewichtet ist


/*
 * Private Funktionsprototypen
 * 
 */

static int updateEntity(void *data, void *userData);
static void clearNearToZero(entityPhysics_t *physics);
static int checkForAllCollisions(entity_t *entity, list_t *entityList);
static int checkForEntityCollision(void *data, void *userData);

/*
 * Implementation Öffentlicher Funktionen
 * 
 */

int Physics_Update(list_t *entityList) {
    int ret = ERR_OK;
    // Alle Entitäten aktualisieren
    ret = List_ForeachArg(entityList, updateEntity, entityList);
    return ret;
}

int Physics_SetPosition(entity_t *entity, float x, float y) {
    if (!entity) {
        return ERR_PARAMETER;
    }
    entity->physics.position.x = x;
    entity->physics.position.y = y;
    return ERR_OK;
}

int Physics_SetVelocity(entity_t *entity, float x, float y) {
    if (!entity) {
        return ERR_PARAMETER;
    }
    entity->physics.velocity.x = x;
    entity->physics.velocity.y = y;
    return ERR_OK;
}

int Physics_SetVelocityPolar(entity_t *entity, float velocity, double angle) {
    return Physics_SetVelocity(entity, velocity * sin(-angle),
                               -(velocity * cos(-angle)));
}

int Physics_SetRotation(entity_t *entity, double rotation) {
    if (!entity) {
        return ERR_PARAMETER;
    }
    entity->physics.rotation = rotation;
    return ERR_OK;
}


/*
 * Implementation Privater Funktionen
 * 
 */

static int updateEntity(void *data, void *userData) {
    entity_t *entity = (entity_t *)data;
    list_t *entityList = (list_t *)userData;
    // Werte die nahezu 0 sind auf 0 setzen
    clearNearToZero(&entity->physics);
    // Erdbeschleunigung anwenden
    entity->physics.velocity.y += GRAVITY * DELTA_TIME;
    // Geschwindigkeit anwenden
    entity->physics.position.x += entity->physics.velocity.x * DELTA_TIME;
    entity->physics.position.y += entity->physics.velocity.y * DELTA_TIME;
    // Position auf AABB übertragen
    entity->physics.aabb.x = entity->physics.position.x;
    entity->physics.aabb.y = entity->physics.position.y;
    // Auf Kollision mit anderen Entitäten prüfen
    return checkForAllCollisions(entity, entityList);
}

static void clearNearToZero(entityPhysics_t *physics) {
    if (abs(physics->velocity.x) <= NEAR_ZERO) {
        physics->velocity.x = 0.0f;
    }
    if (abs(physics->velocity.y) <= NEAR_ZERO) {
        physics->velocity.y = 0.0f;
    }
    if (abs(physics->position.x) <= NEAR_ZERO) {
        physics->velocity.x = 0.0f;
    }
    if (abs(physics->position.y) <= NEAR_ZERO) {
        physics->velocity.y = 0.0f;
    }
    if (abs(physics->rotation) <= NEAR_ZERO) {
        physics->rotation = 0.0f;
    }
}

static int checkForAllCollisions(entity_t *entity, list_t *entityList) {
    int ret = ERR_OK;
    entityCollision_t worldCollision = {0};
    // ret = World_CheckCollision(entity->physics.aabb, &worldCollision); world-Modul noch nicht fertig
    if (ret) {
        return ret;
    } else if (worldCollision.flags) {
        // Callback der Entität aufrufen, Kollision mit der Welt
        if (entity->callbacks.onCollision) {
            entity->callbacks.onCollision(entity, &worldCollision);
        }
    }
    ret = List_ForeachArg(entityList, checkForEntityCollision, entity);
    return ret;
}

static int checkForEntityCollision(void *data, void *userData) {
    entity_t *targetEntity = (entity_t *)data;
    entity_t *sourceEntity = (entity_t *)userData;
    // keine Kollision mit sich selbst
    if (targetEntity == sourceEntity) {
        return ERR_OK;
    }
    // Kollision der AABBs prüfen
    bool collision;
    SDL_Rect intersection;
    collision = SDL_IntersectRect(&targetEntity->physics.aabb,
                                  &sourceEntity->physics.aabb,
                                  &intersection);
    if (!collision) {
        return ERR_OK;
    }
    // Kollisionsnormale ermitteln: suche die Kürzere Seite der Überlappung und
    // ermittle die Vorzeichen des Vektors. Als Vektorlänge wird die Breite bez.
    // Höhe der Überlappung genommen und mit einem Faktor gewichtet.
    entityCollision_t entityCollision = {.flags = 0x1 < ENTITY_COLLISION_ENTITY};
    if (intersection.w > intersection.h) {
        // Breiter als Hoch = Vektor auf y-Achse
        entityCollision.normale.y = intersection.h * BOUNCE_BACK_FACTOR;
        if (sourceEntity->physics.aabb.y < intersection.y) {
            entityCollision.normale.y *= -1.0f;
        }
    } else {
        // Höher als Breit = Vektor auf x-Achse
        entityCollision.normale.x = intersection.w * BOUNCE_BACK_FACTOR;
        if (sourceEntity->physics.aabb.x < intersection.x) {
            entityCollision.normale.x *= -1.0f;
        }
    }
    // Callback der Entität aufrufen, Kollision mit anderer Entität
    if (sourceEntity->callbacks.onCollision) {
        sourceEntity->callbacks.onCollision(sourceEntity, &entityCollision);
    }
    return ERR_OK;
}
