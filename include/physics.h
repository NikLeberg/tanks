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
 * @brief Aktualisiere die Physik aller Entitäten
 * 
 * Jede Entität hält in ihrer Struktur die eigenen Physikdaten. Diese werden von dieser Funktion
 * verarbeitet und ggf. aktualisiert. Weiter werden daraus die x/y Koordinaten verändert, sofern
 * dies erlaubt ist (keine Kollision mit anderen Entitäten oder der Welt).
 * Wird eine Kollision entdeckt, so wird der onCollosion-Callback beider Entitäten aufgerufen.
 * 
 * @param[in] entities Liste aller Entitäten
 * 
 * @return ERR_OK, ERR_PARAMETER oder ERR_FAIL
 */
int Physics_Update(list_t *entities);
