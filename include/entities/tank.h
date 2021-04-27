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
 * @param tank Handle des Panzers, um ihn später zu löschen. Darf NULL sein.
 * @param player Eigentümer des Panzers, wird für Kollisionen verwendet
 * @param x x-Koordinate der Startposition in der Welt
 * @param y y-Koordinate der Startposition in der Welt
 * 
 * @return ERR_OK, ERR_MEMORY oder ERR_FAIL
 */
int Tank_Create(entity_t **tank, const char *player, float x, float y);

/**
 * @brief Zerstöre einen Panzer.
 * 
 * Löscht die Ressourcen des Panzers und entfernt diesen aus dem EntityHandler.
 * Wird entweder durch zerstörende Kollision vom Panzer selber aufgerufen oder
 * aber von anderswo. Dann muss allerdings ein Pointer per Parameter \p tank im
 * Aufruf von \ref Tank_Create erhalten worden sein.
 * 
 * @param tank zu entfernender Panzer
 * 
 * @return ERR_OK oder Fehlercode
 */
int Tank_Destroy(entity_t *tank);
