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
 * @brief Erstelle einen Panzerschuss.
 * Erzeugt ein Panzerschuss und registriert diesen im EntityHandler.
 * 
 * @param player Eigentümer des Panzerschusss, wird für Kollisionen verwendet
 * @param x x-Koordinate der Startposition in der Welt
 * @param y y-Koordinate der Startposition in der Welt
 * @param velocity Startgeschwindigkeit des Schusses
 * @param angle Startwinkel der Schusses
 * 
 * @return ERR_OK, ERR_MEMORY oder ERR_FAIL
 */
int Shell_Create(const char *player, float x, float y, float velocity, double angle);

int Shell_Destroy(entity_t *shell);
