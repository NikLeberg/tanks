/**
* @file world.h
* @author Stoll Simon (stols4@bfh.ch)
* @brief Funktionen mit denen die Welt interagiert wird
* @version 0.0
* @date 2021-03-19
*
* @copyright Copyright (c) 2021 Stoll Simon
*
*/

#pragma once

/*
 * Includes
 *
 */
#include <SDL.h>

 /*
 * Typdeklarationen
 *
 */

 /**
  * @brief Struktur für die Kollision
  * Gibt an ob und wie eine Kollision vorhanden war.
  */
typedef struct CollisionInfo {
	int hasCollision;	//!< Ob das ABB bei CheckCollision() mit dem Kollisionsraster kollidiert hat.
}collisionInfo_t;

/*
* Öffentliche Funktionen
*
*/

/**
* @brief Initialisierung der Welt.
* Diese Funktion muss am Anfang aufgerufen werden.
*
* @return 0 oder Errorcode
*/
int World_Init();

/**
* @brief Lädt die Texturen und Umgebung anhand der gegebenen worldID.
* Die Welt wird anhand von Konfigurationsdateien Aufgebaut.
* Background und Foreground werden geladen. Das Kollisionsraster wird aufgebaut.
* Muss beim Starten der GameSzene aufgerufen werden.
* 
* @param[in] worldID Die ID der Welt die geladen werden soll
*
* @return 0 oder Errorcode
*/
int World_Load(char* worldID);

/**
* @brief Zeichnet den Weltenhintergrund 
* Hintergrund wird bei World_Load() definiert.
*
* @return 0 oder Errorcode
*/
int World_DrawBackground();

/**
* @brief Zeichnet den Weltenvordergrund
* Vordergrund wird bei World_Load() definiert.
*
* @return 0 oder Errorcode
*/
int World_DrawForeground();

/**
* @brief Überprüft ob und wie eine Kollision vorhanden war anhand einer AABB 
* AABB (AxisAlignedBoundBox) ist von einem Entity definiert und wird überprüft ob es mit derm Kollisionsraster überlappt.
* Die \p collision sagt aus, wie die Kollision mit der Welt aussieht.
* 
* @param[in] aabb Die AABB-Kollisionsbox
* @param[out] collision Die Kollisionsdaten
*
* @return 0 oder Errorcode
*/
int CheckCollision(SDL_Rect aabb, collisionInfo_t* collision);
