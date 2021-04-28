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
 * @brief Führt ein Update der Physik und aller Entitäten aus.
 * 
 * Aktualisiert zuerst die Entitäten und danach deren Physik.
 * 
 * @param[in] inputEvents Eingabeevents für die Entitäten
 *
 * @return ERR_OK, ERR_PARAMETER oder ERR_FAIL
 */
int EntityHandler_Update(inputEvent_t *inputEvents);

/**
 * @brief Zeichne alle Entitäten.
 * 
 * Iteriert über alle vorhandenen Entitäten und weist diese an sich selbst zu
 * zeichnen.
 * 
 * @return ERR_OK oder ERR_FAIL
 */
int EntityHandler_Draw();

/**
 * @brief Fügt eine Entität hinzu.
 * 
 * Fügt die gegebene Entität der internen Liste aller Entitäten hinzu. Die
 * Speicherverwaltung ist dem Aufrufer überlassen, muss aber während der
 * Lebensdauer gültig bleiben (static).
 * @note Wird in einem laufendem Zyklus eine Entität hinzugefügt, so ist diese
 * erst nach Aufruf von \ref EntityHandler_Update() aktiv und sichtbar.
 * 
 * @param entity neue Entität
 *
 * @return ERR_OK, ERR_PARAMETER oder ERR_FAIL
 */
int EntityHandler_AddEntity(entity_t *entity);

/**
 * @brief Entfernt eine Entität.
 * 
 * Entfernt die gegebene Entität aus der internen Liste aller Entitäten.
 * Nach dem Aufruf darf der Speicher für die Entität freigegeben werden.
 * @warning Entfernen von Entitäten inherhalb von
 * \ref entityCallbacks_t.onCollision für Kollisionen zwischen zwei Entitäten
 * ist untersagt. Denn die andere Entität erhält im \ref entityCollision_t einen
 * Pointer zum Kollisionspartner. Ist dieser bereits gelöscht so könnte das
 * System abstürzen.
 * 
 * @param entity zu entfernende Entität
 *
 * @return ERR_OK, ERR_PARAMETER oder ERR_FAIL
 */
int EntityHandler_RemoveEntity(entity_t *entity);

/**
 * @brief Entfernt alle Entitäten.
 * 
 * Entferne alle Entitäten aus der internen Liste.
 * @warning Entfernen von Entitäten inherhalb von
 * \ref entityCallbacks_t.onCollision für Kollisionen zwischen zwei Entitäten
 * ist untersagt. Denn die andere Entität erhält im \ref entityCollision_t einen
 * Pointer zum Kollisionspartner. Ist dieser bereits gelöscht so könnte das
 * System abstürzen.
 * 
 * @return int ERR_OK, ERR_PARAMETER oder ERR_FAIL
 */
int EntityHandler_RemoveAllEntities();

/**
 * @brief Fügt ein Einzelteil einer Entität hinzu.
 * 
 * Fügt das gegebene Teil der Liste an Teilen der Entität hinzu. Die
 * Speicherverwaltung ist dem Aufrufer überlassen, muss aber während der
 * Lebensdauer gültig bleiben (static).
 * @note Gleiche Zyklusbedingungen wie \ref EntityHandler_AddEntity()
 * 
 * @param entity Entität der das Teil gehört
 * @param part neues Einzelteil
 *
 * @return ERR_OK, ERR_PARAMETER oder ERR_FAIL
 */
int EntityHandler_AddEntityPart(entity_t *entity, entityPart_t *part);

/**
 * @brief Entfernt ein Einzelteil einer Entität.
 * 
 * Entfernt das gegebene Teil aus der Liste aller Teile in der Entität.
 * Nach dem Aufruf darf der Speicher für das Einzelteil freigegeben werden.
 * @warning Gleiche Löschbedingungen wie \ref EntityHandler_RemoveEntity()
 * 
 * @param entity Entität der das Teil gehört
 * @param part zu entfernendes Einzelteil
 *
 * @return ERR_OK, ERR_PARAMETER oder ERR_FAIL
 */
int EntityHandler_RemoveEntityPart(entity_t *entity, entityPart_t *part);

/**
 * @brief Entfernt alle Einzelteile einer Entität.
 * 
 * Entfernt alle Einzelteile aus der internen Liste.
 * @warning Entfernen von Entitäten ist nur am Anfgang jedes Zyklus erlaubt.
 * @warning Gleiche Löschbedingungen wie \ref EntityHandler_RemoveAllEntities()
 * 
 * @param entity Entität deren Einzelteile entfernt werden sollen
 * 
 * @return int ERR_OK, ERR_PARAMETER oder ERR_FAIL
 */
int EntityHandler_RemoveAllEntityParts(entity_t *entity);
