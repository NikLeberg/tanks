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

/**
 * @brief Alle Informationen und Datenspeicher für einen Schuss.
 * 
 */
typedef struct {
    entity_t shell;         //!< Der Schuss als Entität
    entityPart_t part;      //!< Schuss der tatsächlich angezeigt wird
    entityPart_t explosion; //!< Explosionsanimation
    sprite_t mask;          //!< Modifikationsmaske der Welt bei Kollision
    int isExploding;        //!< 0 = nicht am explodieren, 1 = explodiert gerade
} shellData_t;


/*
 * Variablendeklarationen
 * 
 */

/* ... */


/*
 * Private Funktionsprototypen
 * 
 */

/**
 * @brief Update-Callback für EntityHandler.
 * 
 * Wird vom EntityHandler in der Update-Phase aufgerufen. Rotiert den Schuss
 * gemäss Flugbahn und verwaltet die Explosions-animation.
 * 
 * @param self Pointer auf Schuss-Entität
 * @param inputEvents unbenutzt
 * 
 * @return immer ERR_OK
 */
static int updateCallback(entity_t *self, inputEvent_t *inputEvents);

/**
 * @brief Collision-Callback für EntityHandler.
 * 
 * Wird vom EntityHandler bei einer Kollision aufgerufen. Jede Kollision mit der
 * Welt oder mit Entitäten die nicht dem Schuss-Besitzer gehören, lösen eine
 * Explosion aus.
 * 
 * @param self Pointer auf Schuss-Entität
 * @param collision Informationen zur Kollision
 * 
 * @return immer ERR_OK
 */
static int collisionCallback(entity_t *self, entityCollision_t *collision);

/**
 * @brief Aktiviere die Explosion.
 * 
 * Aktiviert die Explosion indem es die Animation weiterschaltet. Und spielt den
 * Soundeffekt ab.
 * 
 * @param shell Pointer auf Schuss-Entität
 * 
 */
static void triggerExplosion(entity_t *shell);

/**
 * @brief Zerstöre die Welt an Schussposition.
 * 
 * Wurde die Explosion ausgelöst, wird am Ende der Animation die Welt zerstört.
 * 
 * @param shell Pointer auf Schuss-Entität
 * 
 */
static void destroyWorld(entity_t *shell);


/*
 * Implementation Öffentlicher Funktionen
 * 
 */

int Shell_Create(entity_t **shell, const char *player, float x, float y, float velocity, double angle) {
    if (!player) {
        return ERR_PARAMETER;
    }
    // Alloziere ein gesamter Schuss mit Speicher für Entität, dessen Teile, der
    // Weltmaske und Zustandsvariablen.
    shellData_t *shellData = calloc(1, sizeof(shellData_t));
    if (!shellData) {
        goto errorCalloc;
    }
    // speichere Pointer auf gesamten Schuss für späteren Zugriff
    shellData->shell.data = shellData;
    // Hauptentität einrichten
    shellData->shell.owner = player;
    shellData->shell.name = "Panzerschuss";
    shellData->shell.callbacks.onUpdate = updateCallback;
    shellData->shell.callbacks.onCollision = collisionCallback;
    shellData->shell.physics.aabb.w = 5;
    shellData->shell.physics.aabb.h = 5;
    if (Physics_SetPosition(&shellData->shell, x, y)
     || Physics_SetVelocityPolar(&shellData->shell, velocity, angle)) {
        goto errorEntity;
    }
    if (EntityHandler_AddEntity(&shellData->shell)) {
        goto errorEntity;
    }
    // Schusstextur resp. Sprite laden und hinzufügen
    sprite_t *rawSprite;
    SDLW_GetResource("shell", RESOURCETYPE_SPRITE, (void **)&rawSprite);
    if (!rawSprite) {
        goto errorLoadShell;
    }
    shellData->part.name = "Schuss";
    shellData->part.sprite = *rawSprite;
    if (EntityHandler_AddEntityPart(&shellData->shell, &shellData->part)) {
        goto errorLoadShell;
    }
    // Schussanimation resp. Sprite laden und hinzufügen
    SDLW_GetResource("shellEx", RESOURCETYPE_SPRITE, (void **)&rawSprite);
    if (!rawSprite) {
        goto errorLoadExplosion;
    }
    shellData->explosion.name = "Explosion";
    shellData->explosion.sprite = *rawSprite;
    shellData->explosion.sprite.destination.h /= 2;
    shellData->explosion.sprite.destination.w /= 2;
    if (EntityHandler_AddEntityPart(&shellData->shell, &shellData->explosion)) {
        goto errorLoadExplosion;
    }
    // Weltmaske laden
    SDLW_GetResource("shellMask", RESOURCETYPE_SPRITE, (void **)&rawSprite);
    if (!rawSprite) {
        goto errorLoadMask;
    }
    shellData->mask = *rawSprite;
    
    if (shell) {
        *shell = &shellData->shell;
    }
    return ERR_OK;

    // Mache im Fehlerfall alle Schritte einzeln rückgängig
errorLoadMask:
    EntityHandler_RemoveEntityPart(&shellData->shell, &shellData->explosion);
errorLoadExplosion:
    EntityHandler_RemoveEntityPart(&shellData->shell, &shellData->part);
errorLoadShell:
    EntityHandler_RemoveEntity(&shellData->shell);
errorEntity:
    free(shellData);
errorCalloc:
    if (shell) {
        *shell = NULL;
    }
    return ERR_FAIL;
}

int Shell_Destroy(entity_t *shell) {
    int ret = ERR_OK;
    ret |= EntityHandler_RemoveAllEntityParts(shell);
    ret |= EntityHandler_RemoveEntity(shell);
    free(shell->data);
    return ret;
}


/*
 * Implementation Privater Funktionen
 * 
 */

static int updateCallback(entity_t *self, inputEvent_t *inputEvents) {
    (void)inputEvents;
    shellData_t *shellData = (shellData_t *)self->data;
    // richte Schuss der Flugbahn aus
    double angle = atan2(-self->physics.velocity.y, self->physics.velocity.x);
    angle = angle / M_PI * 180.0;
    Physics_SetRotation(self, -angle);
    // Wenn der Schuss momentan explodiert, dann reagiere entsprechend.
    if (shellData->isExploding) {
        switch (shellData->explosion.sprite.multiSpriteIndex) {
            case (0): // Animation wurde zuende gespielt, Lösche die Entität
                Shell_Destroy(self);
                break;
            case (32): // Animation wurde zur Hälfte gespielt, zerstöre die Welt
                destroyWorld(self); // fallthrough --> damit gcc nicht meckert
                // break; <-- durchfallen, immer auch Animation weiterschalten
            default:
                Sprite_NextFrame(&shellData->explosion.sprite);
                break;
        }
    }
    return ERR_OK;
}

static int collisionCallback(entity_t *self, entityCollision_t *collision) {
    // Ignoriere Kollisionen mit der oberen Bildkante, der Schuss wird von
    // alleine wieder herunter kommen.
    collision->flags &= ~ENTITY_COLLISION_BORDER_TOP;
    // Löse eine Explosion aus, falls Kollision mit: Welt, linke-, rechte- oder
    // untere Bildkante.
    if (collision->flags
     & (ENTITY_COLLISION_WORLD | ENTITY_COLLISION_BORDER_BOTTOM |
       ENTITY_COLLISION_BORDER_LEFT | ENTITY_COLLISION_BORDER_RIGHT)) {
        // starte die Animation der Explosion
        triggerExplosion(self);
    }
    // Kollisionen mit Entiäten die dem selben Spieler gehören ignorieren.
    if (collision->flags & ENTITY_COLLISION_ENTITY) {
        if (!strcmp(self->owner, collision->partner->owner)) {
            collision->flags &= ~ENTITY_COLLISION_ENTITY;
        }
    }
    return ERR_OK;
}

static void triggerExplosion(entity_t *shell) {
    // Entferne Kollisionscallback damit keine weiteren Kollisionen
    // gemeldet werden. Die Entität löscht sich sowieso nach der Explosion.
    shell->callbacks.onCollision = NULL;
    // Entität statisch machen, somit bewegt sie sich nicht mehr.
    shell->physics.isStatic = 1;
    // Explosion aktivieren und Frame weiterschalten, damit im Update nicht
    // bereits die Entität gelöscht wird.
    shellData_t *shellData = (shellData_t *)shell->data;
    shellData->isExploding = 1;
    Sprite_NextFrame(&shellData->explosion.sprite);
    // Schuss aus Entität entfernen
    EntityHandler_RemoveEntityPart(shell, &shellData->part);
    // Soundeffekt abspielen
    SDLW_PlaySoundEffect("shellSound");
}

static void destroyWorld(entity_t *shell) {
    // Lade ein Kreis als Explosions-Maske
    sprite_t *rawSprite;
    SDLW_GetResource("shellMask", RESOURCETYPE_SPRITE, (void **)&rawSprite);
    sprite_t mask = *rawSprite;
    // Setze die Position gemäss Schussposition
    mask.position.x = shell->physics.position.x;
    mask.position.y = shell->physics.position.y;
    // Übergebe die Explosion der Welt, welche diese aus dem Vordergrund
    // auschneidet.
    World_Modify(mask);
}
