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

#define DELTA_TIME (1.0f / 60.0f) //!< Updateintervall [s]
#define GRAVITY 40.0f             //!< Erdbeschleunigung [pixel / s2]
#define NEAR_ZERO 0.1f            //!< Werte die kleiner sind zählen als 0

/**
 * @brief Rückstossfaktor einer Kollision in X-Achse mit der Welt
 * 
 * Skaliert die Kollisionsnormale in X-Richtung, empirisch ermittelt.
 * Generell sollte der Rückstoss in X-Richtung klein genug sein, dass eine
 * seitliche Touchierung die Entität nicht zu stark zurückbeschleunigt wird.
 * Aber gross genug, dass schnell bewegende Entitäten nicht seitwärts in die
 * Welt bewegen können.
 */
#define WORLD_SCALE_X_FACTOR 100.0f

/**
 * @brief Rückstossfaktor einer Kollision in Y-Achse mit der Welt
 * 
 * Skaliert die Kollisionsnormale in Y-Richtung, empirisch ermittelt.
 * Generell sollte der Rückstoss in Y-Richtung gross genug sein um das Versinken
 * der Entität in den Boden zu verhindern. Aber nicht zu gross, damit keine zu
 * grossen Steigungen entlang bewegt werden kann.
 */
#define WORLD_SCALE_Y_FACTOR 20.0f

/**
 * @brief Rückstossfaktor einer Kollision zweier Entitäten
 * 
 * Skaliert die Kollisionsnormale in beide Achsen, empirisch ermittelt.
 * Soll gross genug sein so dass Entitäten nicht ineinander oder durcheinander
 * bewegen können.
 */
#define ENTITY_SCALE_FACTOR 20.0f


/*
 * Private Funktionsprototypen
 * 
 */

/**
 * @brief Berechne Physikschritt für eine Entität.
 *
 * Wird durch List_ForeachArg() für jede Entität der Liste aufgerufen und
 * berechnet einen Physikschritt.
 * 
 * @param data opaker Pointer auf eine Entität
 * @param userData opaker Pointer auf Entitätsliste
 *
 * @return ERR_OK oder ERR_FAIL
 */
static int updateEntity(void *data, void *userData);

/**
 * @brief Bereinige Physikdaten.
 * 
 * Da sehr kleine Beschleunigungen mühsam berechnet werden müssen, aber keinen
 * nennenswerten einfluss auf die Position haben, werden diese auf 0 gesetzt.
 * 
 * @param[in,out] physics Physikdaten welche berenigt werden sollen
 */
static void clearNearToZero(entityPhysics_t *physics);

/**
 * @brief Überprüfe auf Kollision einer Entität mit allen anderen und der Welt.
 * 
 * Überprüft zuerst auf eine Kollision mit der Welt und danach iteriert es über
 * die Liste an Entitäten und prüft abermals auf Kollisionen. Die Kollision mit
 * der Welt wird hier dem Callback mitgeteilt.
 * 
 * @note Komplexität ist O(n^2)
 * 
 * @param entity zu prüfende Entität
 * @param entityList Liste aller Entitäten
 * 
 * @return ERR_OK oder ERR_FAIL
 */
static int checkForAllCollisions(entity_t *entity, list_t *entityList);

/**
 * @brief Überprüfe auf Kollision zwischen zwei Entitäten.
 *
 * Wird durch List_ForeachArg() aufgerufen und prüft mittels SDL auf Kollision.
 * Falls eine existiert so wird die Kollisionsnormale gesetzt und der Callback
 * der Entität aufgerufen.
 * 
 * @param data opaker Pointer auf eine Entität für die momentan geprüft wird
 * @param userData opaker Pointer auf Entität gegen die geprüft wird
 *
 * @return ERR_OK oder ERR_FAIL
 */
static int checkForEntityCollision(void *data, void *userData);

/**
 * @brief Reagiere mit Standardaktion auf Kollision.
 * 
 * Reagiere sehr rudimentär auf Kollisionen auf welche der Callback 
 * \ref entityCallbacks_t.onCollision der Entität noch nicht reagiert hat (er
 * hat die Flags nicht zurückgesetzt). Für Kollisionen mit Normale wird entlang
 * der Normale die Geschwindigkeit erhöht. Die restlichen Kollisionen setzen die
 * Geschwindigkeit zurück auf 0.
 * 
 * @param entity Entität die an der Kollision beteiligt ist
 * @param collision Infos der Kollision
 * 
 * @return ERR_OK
 */
static int handleCollision(entity_t *entity, entityCollision_t *collision);

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
    double angleRad = angle * (M_PI / 180.0);
    return Physics_SetVelocity(entity, velocity * cos(-angleRad),
                               -(velocity * sin(-angleRad)));
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
    int ret = checkForAllCollisions(entity, entityList);
    return ret;
}

static void clearNearToZero(entityPhysics_t *physics) {
    if (fabs(physics->velocity.x) <= NEAR_ZERO || isnan(physics->velocity.x)) {
        physics->velocity.x = 0.0f;
    }
    if (fabs(physics->velocity.y) <= NEAR_ZERO || isnan(physics->velocity.y)) {
        physics->velocity.y = 0.0f;
    }
    if (fabs(physics->position.x) <= NEAR_ZERO || isnan(physics->position.x)) {
        physics->position.x = 0.0f;
    }
    if (fabs(physics->position.y) <= NEAR_ZERO || isnan(physics->position.y)) {
        physics->position.y = 0.0f;
    }
    if (fabs(physics->rotation) <= NEAR_ZERO || isnan(physics->rotation)) {
        physics->rotation = 0.0f;
    }
}

static int checkForAllCollisions(entity_t *entity, list_t *entityList) {
    int ret = ERR_OK;
    entityCollision_t worldCollision = {.partner = NULL};
    ret = World_CheckCollision(entity->physics.aabb, &worldCollision);
    if (ret) {
        // Fehler bei der Kollisionerkennung
        return ret;
    } else if (worldCollision.flags) {
        // Kollision mit der Welt
        // Skaliere Kollisionsnormale gemäss Rückstossfaktoren
        worldCollision.normal.x *= WORLD_SCALE_X_FACTOR;
        worldCollision.normal.y *= WORLD_SCALE_Y_FACTOR;
        if (entity->callbacks.onCollision) {
            // Callback der Entität aufrufen
            entity->callbacks.onCollision(entity, &worldCollision);
            handleCollision(entity, &worldCollision);
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
    entityCollision_t entityCollision = {
        .flags = ENTITY_COLLISION_ENTITY,
        .partner = targetEntity
    };
    // Kollisionsnormale ermitteln: suche die Kürzere Seite der Überlappung und
    // ermittle die Vorzeichen des Vektors. Als Vektorlänge wird die Breite bez.
    // Höhe der Überlappung genommen und mit einem Faktor gewichtet.
    if (intersection.w > intersection.h) {
        // Breiter als Hoch = Vektor auf y-Achse
        entityCollision.normal.y = intersection.h * ENTITY_SCALE_FACTOR;
        if (sourceEntity->physics.aabb.y < intersection.y) {
            entityCollision.normal.y *= -1.0f;
        }
    } else {
        // Höher als Breit = Vektor auf x-Achse
        entityCollision.normal.x = intersection.w * ENTITY_SCALE_FACTOR;
        if (sourceEntity->physics.aabb.x < intersection.x) {
            entityCollision.normal.x *= -1.0f;
        }
    }
    // Callback der Entität aufrufen, Kollision mit anderer Entität
    if (sourceEntity->callbacks.onCollision) {
        sourceEntity->callbacks.onCollision(sourceEntity, &entityCollision);
        handleCollision(sourceEntity, &entityCollision);
    }
    return ERR_OK;
}


static int handleCollision(entity_t *entity, entityCollision_t *collision) {
    if (!entity || !collision) {
        return ERR_PARAMETER;
    }
    // Kollision mit dem linken oder rechten Bildrand
    if (collision->flags & ENTITY_COLLISION_BORDER_LEFT
     || collision->flags & ENTITY_COLLISION_BORDER_RIGHT) {
        // horizontale Bewegung rückgängig machen
        entity->physics.position.x -= entity->physics.velocity.x * DELTA_TIME;
        // und Geschwindigkeit 0 setzen
        entity->physics.velocity.x = 0.0f;
    }
    // Kollision mit dem oberen oder unteren Bildrand
    if (collision->flags & ENTITY_COLLISION_BORDER_TOP
     || collision->flags & ENTITY_COLLISION_BORDER_BOTTOM) {
        // vertikale Bewegung rückgängig machen
        entity->physics.position.y -= entity->physics.velocity.y * DELTA_TIME;
        // und Geschwindigkeit 0 setzen
        entity->physics.velocity.y = 0.0f;
    }
    // Kollision mit einer anderen Entität oder mit der Welt
    if (collision->flags & ENTITY_COLLISION_ENTITY
     || collision->flags & ENTITY_COLLISION_WORLD) {
        // Korrigiere die Geschwindigkeit der Entität anhand der gegebenen
        // Kollisionsnormalen.
        // Beachte die horizontale Komponente nur wenn die Entität sich auch
        // horizontal bewegt. Verhindert verrutschen auf schräger Welt.
        if (collision->normal.x != 0.0f && entity->physics.velocity.x != 0.0f) {
            // Verhindere Überkorrektur, die horizontale Korrektur darf maximal
            // ein Stopp erzwingen aber kein Rückstoss resp Vorzeichenwechsel.
            // Verhindert, dass bei einer Hangaufwärtsbewegung die Entität den
            // Hang hinunter geworfen wird.
            float correction = collision->normal.x * DELTA_TIME;
            if (entity->physics.velocity.x < -correction) {
                entity->physics.velocity.x = 0.0f;
            } else {
                entity->physics.velocity.x += correction;
            }
        }
        // vertikale Komponente
        if (collision->normal.y != 0.0f) {
            // Kompensiere zusätzlich noch die Gravitation. Hilft den Entitäten
            // nicht zu sehr zu zittern.
            float correction = (collision->normal.y - GRAVITY) * DELTA_TIME;
            entity->physics.velocity.y = correction;
        }
    }
    return ERR_OK;
}
