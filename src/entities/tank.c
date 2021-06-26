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
 * @brief Alle Informationen und Datenspeicher für einen Panzer.
 * 
 */
typedef struct {
    entity_t tank;        //!< Der Panzer als Entität
    entityPart_t tracks;  //!< Raupen
    entityPart_t chassis; //!< Fahrgestell
    entityPart_t tube;    //!< Schussrohr
    entityPart_t fire;    //!< Schussanimation
    entityPart_t arrow;   //!< Pfeil der den aktiven Spieler signalisiert
} tankData_t;


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
#define TANK_TUBE_MAX_ROTATION_POSITIVE 10.0   //!< Max pos. Rotation des Rohrs
#define TANK_TUBE_MAX_ROTATION_NEGATIVE -190.0 //!< Max neg. Rotation des Rohrs

/**
 * @brief Gesundheitsabzug per Treffer
 * 
 * Wie viele Gesundheitspunkte von \ref player_t.healthpoints abgezogen werden
 * per Treffer eines Geschosses.
 */
#define TANK_NEG_HEALTH_PER_HIT 30


/*
 * Private Funktionsprototypen
 * 
 */

/**
 * @brief Update-Callback für EntityHandler.
 * 
 * Wird vom EntityHandler in der Update-Phase aufgerufen. Verarbeitet Benutzer-
 * eingaben und z.B. bewegt dadurch den Panzer oder stellt das Rohr.
 * 
 * @param self Pointer auf Panzer-Entität
 * @param inputEvents Eingabeevents vom Spieler
 * 
 * @return immer ERR_OK
 */
static int updateCallback(entity_t *self, inputEvent_t *inputEvents);

/**
 * @brief Collision-Callback für EntityHandler.
 * 
 * Wird vom EntityHandler bei einer Kollision aufgerufen. Ignoriert alle
 * Kollisionen mit Entitäten des eigenen Besitzers. Also z.B. ein Schuss.
 * 
 * @param self Pointer auf Panzer-Entität
 * @param collision Informationen zur Kollision
 * 
 * @return immer ERR_OK
 */
static int collisionCallback(entity_t *self, entityCollision_t *collision);

/**
 * @brief Rotiere Panzer gemäss Weltuntergrund.
 * 
 * Sucht den nähesten vertikalen Punkt auf der Weltoberfläche und ermittelt
 * durch eine Kollisionsabfrage den Winkel der Kollisiosnormale.
 * 
 * @param tank Pointer auf Panzer der Rotiert werden soll
 * 
 */
static void rotateToWorld(entity_t *tank);

/**
 * @brief Schiesse einen Schuss ab.
 * 
 * Erstellt eine Schuss-Entität im aktuellen Winkel des Rohrs. Diese ist
 * unabhängig vom Panzer.
 * 
 * @param tank Pointer auf Panzer der den Schuss auslöst
 * 
 */
static void fire(entity_t *tank);

/**
 * @brief Bewege Panzer in der Horizontalen.
 * 
 * Der Panzer kann per WASD-Tasten bewegt werden:
 *      A -> Links  -> Bewegung in x--
 *      D -> Rechts -> Bewegung in x++
 * 
 * @param self Panzer der bewegt werden soll
 * @param axisWASD Richtung aus dem \ref inputEvent_t
 * 
 */
static void moveHorizontal(entity_t *self, SDL_Point axisWASD);

/**
 * @brief Rotiere Schussrohr.
 * 
 * Das Schussrohr kann per WASD-Tasten rotiert werden:
 *      W -> Hoch   -> Rotation im Gegenuhrzeigersinn
 *      S -> Runter -> Bewegung im Uhrzeigersinn
 * 
 * @param tankData Erweiterte Daten zum Panzer
 * @param axisWASD Richtung aus dem \ref inputEvent_t
 * 
 */
static void rotateTube(tankData_t *tankData, SDL_Point axisWASD);


/*
 * Implementation Öffentlicher Funktionen
 * 
 */

int Tank_Create(entity_t **tank, player_t *player, float x, float y) {
    if (!player) {
        return ERR_PARAMETER;
    }
    // Alloziere ein gesamter Panzer mit Speicher für Entität und dessen Teile.
    tankData_t *tankData = calloc(1, sizeof(tankData_t));
    if (!tankData) {
        goto errorCalloc;
    }
    // speichere Pointer auf gesamten Panzer für späteren Zugriff
    tankData->tank.data = tankData;
    // Hauptentität einrichten
    tankData->tank.owner = player;
    tankData->tank.name = "Panzer";
    tankData->tank.callbacks.onUpdate = updateCallback;
    tankData->tank.callbacks.onCollision = collisionCallback;
    tankData->tank.physics.aabb.w = 40;
    tankData->tank.physics.aabb.h = 30;
    if (Physics_SetPosition(&tankData->tank, x, y)) {
        goto errorEntity;
    }
    if (EntityHandler_AddEntity(&tankData->tank)) {
        goto errorEntity;
    }
    // Einzelteile laden und einrichten
    sprite_t *rawSprite;
    // Raupen laden
    SDLW_GetResource("tracks", RESOURCETYPE_SPRITE, (void **)&rawSprite);
    if (!rawSprite) {
        goto errorLoadTracks;
    }
    tankData->tracks.name = "Raupen";
    tankData->tracks.sprite = *rawSprite;
    if (EntityHandler_AddEntityPart(&tankData->tank, &tankData->tracks)) {
        goto errorLoadTracks;
    }
    // Fahrgestell laden
    SDLW_GetResource("chassis", RESOURCETYPE_SPRITE, (void **)&rawSprite);
    if (!rawSprite) {
        goto errorLoadChassis;
    }
    tankData->chassis.name = "Fahrgestell";
    tankData->chassis.sprite = *rawSprite;
    if (EntityHandler_AddEntityPart(&tankData->tank, &tankData->chassis)) {
        goto errorLoadChassis;
    }
    // Panzerrohr laden
    SDLW_GetResource("tube", RESOURCETYPE_SPRITE, (void **)&rawSprite);
    if (!rawSprite) {
        goto errorLoadTube;
    }
    tankData->tube.name = "Rohr";
    tankData->tube.sprite = *rawSprite;
    if (EntityHandler_AddEntityPart(&tankData->tank, &tankData->tube)) {
        goto errorLoadTube;
    }
    // Feuer Animation laden
    SDLW_GetResource("tankFire", RESOURCETYPE_SPRITE, (void **)&rawSprite);
    if (!rawSprite) {
        goto errorLoadFire;
    }
    tankData->fire.name = "Rohr";
    tankData->fire.sprite = *rawSprite;
    tankData->fire.sprite.destination.h /= 2;
    tankData->fire.sprite.destination.w /= 2;
    // An Positionierung des Rohr anpassen, soll am Ende des Rohrs entstehen.
    tankData->fire.sprite.pivot.x = -(32 + tankData->tube.sprite.destination.w);
    tankData->fire.sprite.pivot.y = tankData->tracks.sprite.pivot.y;
    tankData->fire.sprite.destination.x = 32 + tankData->tube.sprite.destination.w;
    tankData->fire.sprite.destination.y = -12;
    if (EntityHandler_AddEntityPart(&tankData->tank, &tankData->fire)) {
        goto errorLoadFire;
    }
    // Pfeil Animation laden
    SDLW_GetResource("arrow", RESOURCETYPE_SPRITE, (void **)&rawSprite);
    if (!rawSprite) {
        goto errorLoadArrow;
    }
    tankData->arrow.name = "Pfeil";
    tankData->arrow.sprite = *rawSprite;
    tankData->arrow.sprite.destination.y = -50;
    // Pfeil noch nicht der Entität hinzufügen, wird im UpdateCallback gemacht.
    // So kann die Sichtbarkeit gemäss aktivem Spieler umgeschaltet werden.
    if (tank) {
        *tank = &tankData->tank;
    }
    return ERR_OK;

    // Mache im Fehlerfall alle Schritte einzeln rückgängig
errorLoadArrow:
    EntityHandler_RemoveEntityPart(&tankData->tank, &tankData->fire);
errorLoadFire:
    EntityHandler_RemoveEntityPart(&tankData->tank, &tankData->tube);
errorLoadTube:
    EntityHandler_RemoveEntityPart(&tankData->tank, &tankData->chassis);
errorLoadChassis:
    EntityHandler_RemoveEntityPart(&tankData->tank, &tankData->tracks);
errorLoadTracks:
    EntityHandler_RemoveEntity(&tankData->tank);
errorEntity:
    free(tankData);
errorCalloc:
    if (tank) {
        *tank = NULL;
    }
    return ERR_FAIL;
}

int Tank_Destroy(entity_t *tank) {
    int ret = ERR_OK;
    ret |= EntityHandler_RemoveAllEntityParts(tank);
    ret |= EntityHandler_RemoveEntity(tank);
    free(tank->data);
    return ret;
}


/*
 * Implementation Privater Funktionen
 * 
 */

static int updateCallback(entity_t *self, inputEvent_t *inputEvents) {
    tankData_t *tankData = (tankData_t *)self->data;
    // Wenn der Panzer dem aktuellen Spieler gehört und im momentanen Spielzug
    // "Move" ist, so reagiere auf Bewegungs-Events.
    if (self->owner == inputEvents->currentPlayer
     && inputEvents->currentPlayer->step == PLAYER_STEP_MOVE) {
        // horizontale Geschwindigkeit gemäss WASD-Tasten verändern
        moveHorizontal(self, inputEvents->axisWASD);
        // Rohrstellung gemäss WASD-Tasten rotieren
        rotateTube(tankData, inputEvents->axisWASD);
        // Falls Leertaste -> Schuss feuern
        if (inputEvents->lastKey == ' ') {
            // übergang in den nächsten Spielzug
            inputEvents->currentPlayer->step = PLAYER_STEP_FIRE;
            // erstelle Schussentität und spiele Animation ab
            fire(self);
        }
    }
    // Horizontale Bewegung in jedem Zyklus um 5% dämpfen damit der Panzer nicht
    // endlos in eine Richtung fährt.
    Physics_SetVelocity(self, self->physics.velocity.x * 0.95f, NAN);
    // rotiere den gesamten Panzer basierend auf der Position auf der Welt
    rotateToWorld(self);
    // Falls Schussanimation am laufen -> weiterschalten
    if (tankData->fire.sprite.multiSpriteIndex != 0) {
        Sprite_NextFrame(&tankData->fire.sprite);
    }
    // Wenn dieser Panzer dem aktuellen Spieler gehört, so zeige einen Pfeil
    // als Indikator über dem Panzer an.
    if (self->owner == inputEvents->currentPlayer) {
        if (4 == tankData->tank.parts->elementCount
         && PLAYER_STEP_MOVE == inputEvents->currentPlayer->step) {
            // Wenn der Panzer nur aus vier Teilen besteht so wurde der Pfeil
            // noch nicht hinzugefügt. Füge den Pfeil hinzu.
            EntityHandler_AddEntityPart(self, &tankData->arrow);
        } else if (PLAYER_STEP_FIRE == inputEvents->currentPlayer->step) {
            // Schuss wurde abgefeuert -> entferne den Pfeil
            EntityHandler_RemoveEntityPart(self, &tankData->arrow);
        } else {
            // Pfeil ist sichtbar -> Animation weiterschalten
            Sprite_NextFrame(&tankData->arrow.sprite);
            // Entferne die relative Rotation des Panzers auf dem Pfeil
            tankData->arrow.sprite.rotation = -self->physics.rotation;
        }
    }
    return ERR_OK;
}

static int collisionCallback(entity_t *self, entityCollision_t *collision) {
    // Reagiere Auf Kollisionen mit Entitäten
    if (collision->flags & ENTITY_COLLISION_ENTITY) {
        if (!strcmp(self->owner->name, collision->partner->owner->name)) {
            // gehört dem eigenen Spieler, ignorieren
            collision->flags &= ~ENTITY_COLLISION_ENTITY;
        } else if (!strcmp(collision->partner->name, "Panzerschuss")) {
            // Kollision mit einem Schuss der nicht von uns ist.
            collision->flags &= ~ENTITY_COLLISION_ENTITY;
            // Ziehe Lebenspunkte ab
            self->owner->healthpoints -= TANK_NEG_HEALTH_PER_HIT;
        }
    }
    return ERR_OK;
}

static void rotateToWorld(entity_t *tank) {
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
        Physics_SetRotation(tank, -angle);
    }
    return;
}

static void fire(entity_t *tank) {
    tankData_t *tankData = (tankData_t *)tank->data;
    entityPart_t *tube = &tankData->tube;
    // Der Schuss soll am Ende des Schussrohrs erscheinen, daher muss dessen
    // aktuelle Position mit Vektorgeometrie berechnet werden.
    // Starte im Zentrum der Entität
    float x = tank->physics.position.x;
    float y = tank->physics.position.y;
    // Startpunkt zum Rohransatz schieben
    double angleRad = (tank->physics.rotation + 90.0) * (M_PI / 180.0);
    x += tube->sprite.destination.y * cos(-angleRad);
    y += -(tube->sprite.destination.y * sin(-angleRad));
    // Startpunkt zum Rohrende schieben
    angleRad = (tank->physics.rotation + tube->sprite.rotation) * (M_PI / 180.0);
    x += tube->sprite.destination.w * cos(-angleRad);
    y += -(tube->sprite.destination.w * sin(-angleRad));
    // Schuss erstellen
    double angle = tank->physics.rotation + tube->sprite.rotation;
    Shell_Create(NULL, tank->owner, x, y, 100.0f, angle);
    // Feuer Animation aktivieren im aktuellen Winkel des Rohrs
    tankData->fire.sprite.rotation = tankData->tube.sprite.rotation;
    Sprite_SetFrame(&tankData->fire.sprite, 1);
    // Soundeffekt abspielen
    SDLW_PlaySoundEffect("tankSound");
}

static void moveHorizontal(entity_t *self, SDL_Point axisWASD) {
    const float *vx = &self->physics.velocity.x;
    // horizontale Geschwindigkeit gemäss WASD-Tasten verändern
    if (axisWASD.x == -1 && *vx > -TANK_MAX_HORIZONTAL_SPEED) {
        Physics_SetRelativeVelocity(self, -10.0f, NAN);
    } else if (axisWASD.x == 1 && *vx < TANK_MAX_HORIZONTAL_SPEED) {
        Physics_SetRelativeVelocity(self, +10.0f, NAN);
    }
}

static void rotateTube(tankData_t *tankData, SDL_Point axisWASD) {
    entityPart_t *tube = &tankData->tube;
    // Rohrstellung gemäss WASD-Tasten rotieren
    if (axisWASD.y == -1 &&
        tube->sprite.rotation > TANK_TUBE_MAX_ROTATION_NEGATIVE) {
        tube->sprite.rotation -= 0.5;
    } else if (axisWASD.y == 2 &&
               tube->sprite.rotation < TANK_TUBE_MAX_ROTATION_POSITIVE) {
        tube->sprite.rotation += 0.5;
    }
}
