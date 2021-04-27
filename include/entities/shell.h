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
 * 
 * Erzeugt ein Panzerschuss und registriert diesen im EntityHandler.
 * 
 * @param shell Handle des Schusses, um ihn später zu löschen. Darf NULL sein.
 * @param player Eigentümer des Panzerschusss, wird für Kollisionen verwendet
 * @param x x-Koordinate der Startposition in der Welt
 * @param y y-Koordinate der Startposition in der Welt
 * @param velocity Startgeschwindigkeit des Schusses
 * @param angle Startwinkel des Schusses
 * 
 * @return ERR_OK, ERR_MEMORY oder ERR_FAIL
 */
int Shell_Create(entity_t **shell, const char *player, float x, float y, float velocity, double angle);

/**
 * @brief Zerstöre einen Panzerschuss.
 * 
 * Löscht die Ressourcen des Schusses und entfernt diesen aus dem EntityHandler.
 * Wird entweder durch zerstörende Kollision vom Schuss selber aufgerufen oder
 * aber von anderswo. Dann muss allerdings ein Pointer per Parameter \p shell im
 * Aufruf von \ref Shell_Create erhalten worden sein.
 * 
 * @param shell zu entfernender Schuss
 * 
 * @return ERR_OK oder Fehlercode
 */
int Shell_Destroy(entity_t *shell);
