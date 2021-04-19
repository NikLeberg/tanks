/**
 * @file tank.h
 * @author Leuenberger Niklaus (leuen4@bfh.ch)
 * @brief Panzerentität
 * @version 0.1
 * @date 2021-04-18
 * 
 * @copyright Copyright (c) 2021 Leuenberger Niklaus
 * 
 */

#pragma once


/*
 * Includes
 * 
 */

#include "entity.h"
#include "sdlWrapper.h"


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
 * Öffentliche Funktionen
 * 
 */

/**
 * @brief Erstelle einen Panzer.
 * Erzeugt ein Panzer und registriert diesen mit all seinen Einzelteilen
 * im EntityHandler.
 * 
 * @param owner Eigentümer des Panzers, wird für Kollisionen verwendet
 * @param startPosition Startposition in der Welt
 * 
 * @return ERR_OK, ERR_MEMORY oder ERR_FAIL
 */
int Tank_Create(const char *player, SDL_Point startPosition);

int Tank_Destroy(entity_t *tank);
