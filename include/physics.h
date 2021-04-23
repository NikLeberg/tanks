/**
 * @file physics.h
 * @author Leuenberger Niklaus (leuen4@bfh.ch)
 * @brief Verarbeitet die Physik der Entitäten
 * @version 0.1
 * @date 2021-03-19
 * 
 * @copyright Copyright (c) 2021 Leuenberger Niklaus
 * 
 * Dieses Modul stellt verschiedene Funktionen zum setzen von Physikparametern
 * der Entitäten bereit. Dies sind die Physics_Set* Funktionen. Weiter wird
 * durch dieses Modul die Physik der Entitäten in jedem Spielzyklus berechnet.
 * Es kontrolliert dann auch auf Kollisionen und behebt diese automatisch.
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
 * @brief Aktualisiere die Physik aller Entitäten.
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
 * @brief Setze die Position.
 * 
 * Setze die absolute Position der Entität in der Welt.
 * @note Parameter \p x oder \p y dürfen NAN sein. In diesem Fall wird nur der
 * andere Parameter übernommen. Ermöglicht das setzen von nur einer Komponente.
 * 
 * @param entity Entität deren Position gesetzt werden soll
 * @param x x-Koordinate
 * @param y y-Koordinate (nicht kartesisch!, 0 = oben)
 * 
 * @return ERR_OK oder ERR_PARAMETER
 */
int Physics_SetPosition(entity_t *entity, float x, float y);

/**
 * @brief Setze die relative Position.
 * 
 * Fast identisch zu \ref Physics_SetPosition aber anstelle dass die Position
 * direkt gesetzt wird, werden \p x oder \p y zur aktuellen Position
 * hinzuaddiert. Also eine relative Verschiebung.
 * @note Parameter \p x oder \p y dürfen NAN sein. In diesem Fall wird nur der
 * andere Parameter übernommen. Ermöglicht das setzen von nur einer Komponente.
 * 
 * @param entity Entität deren Position gesetzt werden soll
 * @param x relative x-Koordinate
 * @param y relative y-Koordinate (nicht kartesisch!, 0 = oben)
 * 
 * @return ERR_OK oder ERR_PARAMETER
 */
int Physics_SetRelativePosition(entity_t *entity, float x, float y);

/**
 * @brief Setze die Geschwindigkeit.
 * 
 * Setze die absolute Geschwindigkeit der Entität in der Welt.
 * @note Parameter \p x oder \p y dürfen NAN sein. In diesem Fall wird nur der
 * andere Parameter übernommen. Ermöglicht das setzen von nur einer Komponente.
 * 
 * @param entity Entität deren Geschwindigkeit gesetzt werden soll
 * @param x Horizonatale Geschwindigkeit
 * @param y Vertikale Geschwindigkeit
 * 
 * @return ERR_OK oder ERR_PARAMETER
 */
int Physics_SetVelocity(entity_t *entity, float x, float y);

/**
 * @brief Setze die relative Geschwindigkeit.
 * 
 * Fast identisch zu \ref Physics_SetVelocity aber anstelle dass die
 * Geschwindigkeit direkt gesetzt wird, werden \p x oder \p y zur aktuellen
 * Geschwindigkeit hinzuaddiert. Also eine relative Geschwindigkeitsänderung.
 * @note Parameter \p x oder \p y dürfen NAN sein. In diesem Fall wird nur der
 * andere Parameter übernommen. Ermöglicht das setzen von nur einer Komponente.
 * 
 * @param entity Entität deren Geschwindigkeit gesetzt werden soll
 * @param x relative Horizonatale Geschwindigkeit
 * @param y relative Vertikale Geschwindigkeit
 * 
 * @return ERR_OK oder ERR_PARAMETER
 */
int Physics_SetRelativeVelocity(entity_t *entity, float x, float y);

/**
 * @brief Setze die Geschwindigkeit in Polarform.
 * 
 * Setze die absolute Geschwindigkeit der Entität in der Welt.
 * 
 * @param entity Entität deren Geschwindigkeit gesetzt werden soll
 * @param velocity Betrag der Geschwindigkeit
 * @param angle Winkel zur Horizontalen, positiv = Uhrzeigersinn
 * 
 * @return ERR_OK oder ERR_PARAMETER
 */
int Physics_SetVelocityPolar(entity_t *entity, float velocity, double angle);

/**
 * @brief Setze die relative Geschwindigkeit in Polarform.
 * 
 * Fast identisch zu \ref Physics_SetVelocityPolar aber anstelle dass die
 * Geschwindigkeit direkt gesetzt wird, werden die berechneten Komponenten x und
 * y zur aktuellen Geschwindigkeit hinzuaddiert. Also eine relative
 * Geschwindigkeitsänderung.
 * @note Parameter \p x oder \p y dürfen NAN sein. In diesem Fall wird nur der
 * andere Parameter übernommen. Ermöglicht das setzen von nur einer Komponente.
 * 
 * @param entity Entität deren Geschwindigkeit gesetzt werden soll
 * @param velocity Betrag der relativen Geschwindigkeit
 * @param angle Winkel zur Horizontalen, positiv = Uhrzeigersinn
 * 
 * @return ERR_OK oder ERR_PARAMETER
 */
int Physics_SetRelativeVelocityPolar(entity_t *entity, float velocity, double angle);

/**
 * @brief Setze die Rotation
 * 
 * Setze die absolute Rotation der Entität in der Welt.
 * 
 * @param entity Entität deren Rotation gesetzt werden soll
 * @param rotation Winkel zur Horizontalen, positiv = Uhrzeigersinn
 * 
 * @return ERR_OK oder ERR_PARAMETER
 */
int Physics_SetRotation(entity_t *entity, double rotation);

/**
 * @brief Setze die relative Rotation
 * 
 * Fast identisch zu \ref Physics_SetRotation aber anstelle dass die Rotation
 * direkt gesetzt wird, wird \p rotation zur aktuellen Rotation hinzuaddiert.
 * Also eine relative Rotation.
 * 
 * @param entity Entität deren Rotation gesetzt werden soll
 * @param rotation relativer Winkel zur Horizontalen, positiv = Uhrzeigersinn
 * 
 * @return ERR_OK oder ERR_PARAMETER
 */
int Physics_SetRelativeRotation(entity_t *entity, double rotation);
