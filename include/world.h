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
#include "sprite.h"
#include "entity.h"


/*
 * Typdeklarationen
 *
 */

struct sdlwResource_s;    // Vorwärtsdeklaration einer SDLW-Ressource
struct entityCollision_s; // Vorwärtsdeklaration eine Kollisions-Struktur

/**
 * @brief Struktur zum Laden einer Konfiguration
 * 
 */
typedef struct worldConfig_s {
    SDL_Texture *foreground; //!< Vordergrundstextur
    SDL_Texture *background; //!< Hintergrundstextur
    char bgMusic[32];        //!< Hintergrundsmusik
} worldConfig_t;


/*
 * Öffentliche Funktionen
 *
 */

/**
 * @brief Erstellt eine Weltenkonfiguration.
 * 
 * Lädt anhand der Konfigurationszeile eine Welt.
 *
 * @param[in] config Zeile aus Konfigurationsdatei mit Argumenten
 * @param[out] resource Wohin die Ressource geladen werden soll
 *
 * @return 0 oder Errorcode
 */
int World_LoadConfig(char *config, struct sdlwResource_s *resource);

/**
 * @brief Initialisierung aller Welten.
 * 
 * Diese Funktion muss am Anfang aufgerufen werden.
 *
 * @return 0 oder Errorcode
 */
int World_Init();

/**
 * @brief Schliesst alle Welten.
 * 
 * Diese Funktion muss am Ende aufgerufen werden.
 * Befreit alle allozierten Ressourcen.
 */
void World_Quit();

/**
 * @brief Lädt die Texturen und Umgebung anhand der gegebenen worldID.
 * 
 * Die Welt wird anhand von Konfigurationsdateien aufgebaut.
 * Hintergrund und Vordergrund werden geladen. Das Kollisionsraster wird aufgebaut.
 * Muss beim Starten der GameSzene aufgerufen werden.
 * 
 * @param[in] worldID Die ID der Welt die geladen werden soll
 *
 * @return 0 oder Errorcode
 */
int World_Load(char *worldID);

/**
 * @brief Zeichnet den Weltenhintergrund.
 * 
 * Hintergrund wird bei \ref World_Load() definiert.
 *
 * @return 0 oder Errorcode
 */
int World_DrawBackground();

/**
 * @brief Zeichnet den Weltenvordergrund.
 * 
 * Vordergrund wird bei \ref World_Load() definiert.
 *
 * @return 0 oder Errorcode
 */
int World_DrawForeground();

/**
 * @brief Überprüft ob und wie eine Kollision vorhanden war anhand einer AABB.
 * 
 * AABB (AxisAlignedBoundBox) ist von einem Entity definiert und wird überprüft
 * ob es mit dem Kollisionsraster überlappt.
 * Die \p collision sagt aus, wie die Kollision mit der Welt aussieht.
 * 
 * @param[in] aabb Die AABB-Kollisionsbox
 * @param[out] collision Die Kollisionsdaten
 *
 * @return 0 oder Errorcode
 */
int World_CheckCollision(SDL_Rect aabb, struct entityCollision_s *collision);

/**
 * @brief Modifiziert die Welt.
 * 
 * Sprite Textur muss entweder vom Typ Mask oder Stamp sein!
 * 
 * @param[in] sprite Das Aussehen, Position und Rotation der Modifikation
 * 
 * @return 0 oder Errorcode
 */
int World_Modify(sprite_t sprite);

/**
 * @brief Überprüft ob eine Vertikale Linie vom Startpunkt nach unten die Welt schneidet.
 * 
 * @param[in] searchStart Der Punkt von dem die Linie ausgeht
 * @param[out] hitPoint Der Pixel, wo die erste "Kollision" stattgefunden hat
 * 
 * @return 0 oder Errorcode
 */
int World_VerticalLineIntersection(SDL_Point searchStart, SDL_Point *hitPoint);

/**
 * @brief Gibt Punkte mit regelmässigen X-Abständen aus, bei denen das AABB frei ist.
 * 
 * @param[in] aabb Die Kollisionsbox des zu setzenden Elemenentes
 * @param[in] count Wie viele Elemente gesetzt werden müssen
 * @param[out] locations Array mit [count] Punkten, muss vom Nutzer übergeben werden
 * 
 * @return 0 oder Errorcode
 */
int World_CreateStartLocations(SDL_Rect aabb, int count, SDL_Point *locations);
