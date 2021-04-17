/**
 * @file entityHandler.h
 * @author Leuenberger Niklaus (leuen4@bfh.ch)
 * @brief Verwaltet alle Entitäten
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

#include "scene.h"
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
 * @brief Führe ein Update der Physik und aller Entitäten aus
 * 
 * Aktualisiert zuerst die Physik der Entitäten und danach die Entitäten selbst.
 * 
 * @param[in] inputEvents Eingabeevents für die Entitäten (bsp.: nach links oder nach rechts)
 * @return ERR_OK, ERR_PARAMETER oder ERR_FAIL
 */
int EntityHandler_Update(inputEvent_t *inputEvents);

/**
 * @brief Zeichne alle Entitäten
 * 
 * Iteriert über alle vorhandenen Entitäten und weist diese an sich selbst zu zeichnen.
 * 
 * @return ERR_OK oder ERR_FAIL
 */
int EntityHandler_Draw();

/**
 * @brief Füge eine Entität hinzu
 * 
 * Fügt die gegebene Entität der internen Liste aller Entitäten hinzu.
 * Die Speicherverwaltung ist dem Aufrufer überlassen, muss aber während der Lebensdauer
 * gültig bleiben (static).
 * @note Wird in einem laufendem Zyklus die Entität hinzugefügt, so ist diese erst
 * ab dem nächsten Aufruf von EntityHandler_Update() aktiv und sichtbar.
 * 
 * @param entity neue Entität
 * @return ERR_OK, ERR_PARAMETER oder ERR_FAIL
 */
int EntityHandler_AddEntity(entity_t *entity);

/**
 * @brief Entferne eine Entität
 * 
 * Entfernt die gegebene Entität aus der internen Liste aller Entitäten.
 * Nach dem Aufruf darf der Speicher für die Entität freigegeben werden.
 * @warning Entfernen von Entitäten ist nur am Anfgang jedes Zyklus erlaubt.
 * Dies heiss nach EntityHandler_Draw() aber vor EntityHandler_Update().
 * 
 * @param entity zu entfernende Entität
 * @return ERR_OK, ERR_PARAMETER oder ERR_FAIL
 */
int EntityHandler_RemoveEntity(entity_t *entity);

/**
 * @brief Entferne alle Entitäten
 * 
 * Entferne alle Entitäten aus der internen Liste.
 * @warning Entfernen von Entitäten ist nur am Anfgang jedes Zyklus erlaubt.
 * Dies heiss nach EntityHandler_Draw() aber vor EntityHandler_Update().
 * 
 * @return int ERR_OK, ERR_PARAMETER, ERR_FAIL oder ERR_SEQUENCE
 */
int EntityHandler_RemoveAll();
