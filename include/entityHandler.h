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
 * @brief Führe ein Update der Physik und aller Entitäten aus.
 * 
 * Aktualisiert zuerst die Entitäten und danach deren Physik.
 * 
 * @param[in] inputEvents Eingabeevents für die Entitäten (bsp.: nach links oder nach rechts)
 *
 * @return ERR_OK, ERR_PARAMETER oder ERR_FAIL
 */
int EntityHandler_Update(inputEvent_t *inputEvents);

/**
 * @brief Zeichne alle Entitäten.
 * 
 * Iteriert über alle vorhandenen Entitäten und weist diese an sich selbst zu zeichnen.
 * 
 * @return ERR_OK oder ERR_FAIL
 */
int EntityHandler_Draw();

/**
 * @brief Füge eine Entität hinzu.
 * 
 * Fügt die gegebene Entität der internen Liste aller Entitäten hinzu.
 * Die Speicherverwaltung ist dem Aufrufer überlassen, muss aber während der Lebensdauer
 * gültig bleiben (static).
 * @note Wird in einem laufendem Zyklus die Entität hinzugefügt, so ist diese erst
 * ab dem nächsten Aufruf von \ref EntityHandler_Update() aktiv und sichtbar.
 * 
 * @param entity neue Entität
 *
 * @return ERR_OK, ERR_PARAMETER oder ERR_FAIL
 */
int EntityHandler_AddEntity(entity_t *entity);

/**
 * @brief Entferne eine Entität.
 * 
 * Entfernt die gegebene Entität aus der internen Liste aller Entitäten.
 * Nach dem Aufruf darf der Speicher für die Entität freigegeben werden.
 * @warning Entfernen von Entitäten ist nur am Anfgang jedes Zyklus erlaubt.
 * Dies heiss nach \ref EntityHandler_Draw() aber vor \ref EntityHandler_Update().
 * 
 * @param entity zu entfernende Entität
 *
 * @return ERR_OK, ERR_PARAMETER oder ERR_FAIL
 */
int EntityHandler_RemoveEntity(entity_t *entity);

/**
 * @brief Entferne alle Entitäten.
 * 
 * Entferne alle Entitäten aus der internen Liste.
 * @warning Entfernen von Entitäten ist nur am Anfgang jedes Zyklus erlaubt.
 * Dies heisst nach \ref EntityHandler_Draw() aber vor \ref EntityHandler_Update().
 * 
 * @return int ERR_OK, ERR_PARAMETER oder ERR_FAIL
 */
int EntityHandler_RemoveAllEntities();

/**
 * @brief Füge ein Einzelteil einer Entität hinzu.
 * 
 * Fügt das gegebene Teil der Liste an Teilen der Entität hinzu.
 * Die Speicherverwaltung ist dem Aufrufer überlassen, muss aber während der Lebensdauer
 * gültig bleiben (static).
 * @note Gleiche Zyklusbedingungen wie \ref EntityHandler_AddEntity()
 * 
 * @param entity Entität der das Teil gehört
 * @param part neues Einzelteil
 *
 * @return ERR_OK, ERR_PARAMETER oder ERR_FAIL
 */
int EntityHandler_AddEntityPart(entity_t *entity, entityPart_t *part);

/**
 * @brief Entferne ein Einzelteil einer Entität.
 * 
 * Entfernt das gegebene Teil aus der Liste aller Teile in der Entität.
 * Nach dem Aufruf darf der Speicher für das Einzelteil freigegeben werden.
 * @note Gleiche Zyklusbedingungen wie \ref EntityHandler_RemoveEntity()
 * 
 * @param entity Entität der das Teil gehört
 * @param part zu entfernendes Einzelteil
 *
 * @return ERR_OK, ERR_PARAMETER oder ERR_FAIL
 */
int EntityHandler_RemoveEntityPart(entity_t *entity, entityPart_t *part);

/**
 * @brief Entferne alle Einzelteile einer Entität.
 * 
 * Entferne alle Einzelteile aus der internen Liste.
 * @warning Entfernen von Entitäten ist nur am Anfgang jedes Zyklus erlaubt.
 * @note Gleiche Zyklusbedingungen wie \ref EntityHandler_RemoveAllEntities()
 * 
 * @param entity Entität deren Einzelteile entfernt werden sollen
 * 
 * @return int ERR_OK, ERR_PARAMETER oder ERR_FAIL
 */
int EntityHandler_RemoveAllEntityParts(entity_t *entity);
