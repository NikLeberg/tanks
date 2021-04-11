/**
 * @file physics.h
 * @author Leuenberger Niklaus (leuen4@bfh.ch)
 * @brief Verarbeitet die Physik der Entitäten
 * @version 0.1
 * @date 2021-03-19
 * 
 * @copyright Copyright (c) 2021 Leuenberger Niklaus
 * 
 */

#pragma once


/*
 * Includes
 * 
 */

#include "list.h"
#include "entity.h"


/*
 * Typdeklarationen
 * 
 */

/**
 * @brief Flags möglicher Kollision
 * 
 */
typedef enum {
    PHYSICS_COLLISION_WORLD = 0,     //!< Kollision mit der Welt
    PHYSICS_COLLISION_BORDER_LEFT,   //!< Kollision mit linkem Spielrand
    PHYSICS_COLLISION_BORDER_RIGHT,  //!< Kollision mit rechtem Spielrand
    PHYSICS_COLLISION_BORDER_TOP,    //!< Kollision mit oberem Spielrand
    PHYSICS_COLLISION_BORDER_BOTTOM, //!< Kollision mit unterem Spielrand
    PHYSICS_COLLISION_ENTITY         //!< Kollision mit einer Entität
} physicsCollisionFlags_t;

/**
 * @brief Informationen einer Kollision
 * 
 */
typedef struct {
    /**
     * @brief Kollisionsflags
     * 
     * Flag mit gesetzten Bits für einzelne Kollisionen gemäss
     * \ref physicsCollisionFlags_t welche stattgefunden haben.
     */
    int flags;

    /**
     * @brief Vektor der Kollisionsnormale
     * 
     * Die Kollisionsnormale ist der kürzeste Vektor entlang der die Entität
     * verschoben werden müsste damit keine Kollision entsteht.
     */
    SDL_FPoint normale;
} physicsCollision_t;

/*
 * Variablendeklarationen
 * 
 */

/* ... */


/*
 * Öffentliche Funktionen
 * 
 */

/**
 * @brief Aktualisiere die Physik aller Entitäten
 * 
 * Jede Entität hält in ihrer Struktur die eigenen Physikdaten. Diese werden von
 * dieser Funktion verarbeitet und ggf. aktualisiert. Weiter werden daraus die
 * x/y Koordinaten verändert, sofern dies erlaubt ist (keine Kollision mit
 * anderen Entitäten oder der Welt). Wird eine Kollision entdeckt, so wird der
 * onCollision-Callback beider Entitäten aufgerufen.
 * 
 * @param[in] entityList Liste aller Entitäten
 * 
 * @return ERR_OK, ERR_PARAMETER oder ERR_FAIL
 */
int Physics_Update(list_t *entityList);
