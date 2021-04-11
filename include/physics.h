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

/**
 * @brief Setze die Position
 * 
 * @param entity Entität deren Position gesetzt werden soll
 * @param x x-Koordinate
 * @param y y-Koordinate (nicht kartesisch!, 0 = oben)
 * 
 * @return ERR_OK oder ERR_PARAMETER
 */
int Physics_SetPosition(entity_t *entity, float x, float y);

/**
 * @brief Setze die Geschwindigkeit
 * 
 * @param entity Entität deren Geschwindigkeit gesetzt werden soll
 * @param x Horizonatale Geschwindigkeit
 * @param y Vertikale Geschwindigkeit
 * 
 * @return 
 */
int Physics_SetVelocity(entity_t *entity, float x, float y);
int Physics_SetVelocityPolar(entity_t *entity, float velocity, double angle);
int Physics_SetRotation(entity_t *entity, double rotation);
