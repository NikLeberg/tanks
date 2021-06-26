/**
 * @file input.h
 * @author Weber Jan (webej14@bfh.ch)
 * @brief Datenstruktur eines Input-Events
 * @version 0.1
 * @date 2021-06-16
 * 
 * @copyright Copyright (c) 2021 Weber Jan
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
 * @brief Spielzug zustand des Spielers.
 * 
 * Zu beginn kann der Spieler den Panzer bewegen, und das Schussrohr rotieren.
 * Sobald er mit der Leertaste einen Schuss abgefeuert hat, kann er sicht nicht
 * mehr bewegen. Wenn der Schuss dann irgendwo explodiert ist, ist der
 * Spielzugug beendet.
 */
typedef enum {
    PLAYER_STEP_MOVE = 0, //!< Spieler kann sich bewegen
    PLAYER_STEP_FIRE,     //!< Spieler hat abgefeuert, warte auf Explosion
    PLAYER_STEP_DONE      //!< Spielerzug ist beendet
} playerStep_t;

/**
 * @brief Spielerinformationen
 *
 */
typedef struct {
    char *name; 		//!< Spielernamen
	int healthpoints;	//!< Lebenspunkte des Spielers
	playerStep_t step;	//!< Aktueller Spielzug
                      	// ...
} player_t;

/**
 * @brief inputEvent Struktur
 *
 */
typedef struct {
    player_t *currentPlayer;	//!< Aktueller Spieler
    SDL_Point mousePosition;	//!< Mausposition
    int mouseButtons;      		//!< gedrückte Maustasten
    SDL_KeyCode lastKey;  		//!< zuletzt gedrückte Taste
	char currentChar;			//!< aktuell eingegebener Char

    /**
	 * @brief Richtung gegeben über WASD-Tasten
	 * 
	 * x: -1 = links, 0 = nichts, +1 = rechts
	 * y: -1 = hoch, 0 = nichts, +1 = runter
	 */
    SDL_Point axisWASD;

    /**
	 * @brief Richtung gegeben über Pfeiltasten
	 * 
	 * x: -1 = links, 0 = nichts, +1 = rechts
	 * y: -1 = hoch, 0 = nichts, +1 = runter
	 */
    SDL_Point axisArrow;
} inputEvent_t;


/*
 * Variablendeklarationen
 * 
 */

/* ... */


/*
 * Öffentliche Funktionen
 * 
 */

/* ... */
