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

#define PHYSICS_GRAVITY 10.0f //!< Erdbeschleunigung [pixel / s2]


/*
 * Private Funktionsprototypen
 * 
 */

/* ... */


/*
 * Implementation Öffentlicher Funktionen
 * 
 */

int Physics_Update(list_t *entityList) {
    (void) entityList;
    return ERR_OK;
}


/*
 * Implementation Privater Funktionen
 * 
 */

/* ... */
