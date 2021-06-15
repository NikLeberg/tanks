/**
 * @file scene.h
 * @author Weber Jan (webej14@bfh.ch)
 * @brief Scene auswahl und Koordination benötigter Module
 * @version 0.1
 * @date 2021-03-19
 * 
 * @copyright Copyright (c) 2021 Weber Jan
 * 
 */

#pragma once


/*
 * Includes
 * 
 */

#include "sdlWrapper.h"


/*
 * Typdeklarationen
 * 
 */

/**
 * @brief inputEvent Struktur
 *
 */
typedef struct inputEvent_s {
    SDL_Point mousePosition; //!< Mausposition
    int mouseButtons;        //!< gedrückte Maustasten
    char *lastKey;           //!< zuletzt gedrückte Taste
    char lastChar;           //!< letztes Zeichen

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

/**
 * @brief Initialisierung aller Scenen
 * 
 * Initialisiert alle Scenen, erstellt Elemente wie Buttons etc.
 * Damit alle Scenen zum Aufrufen bereit stehen.
 * 
 * @return 0 oder Fehlercode
 */
int Scene_Init();

/**
 * @brief Aktuelle Scene aktualisieren
 * 
 * Aktualisiert sämtliche Elemente der aktuellen Scene. Dafür werden
 * die Folgemodule: GUI und EntityHandler angesprochen.
 * 
 * @return 0 oder Fehlercode
 */
int Scene_Update();

/**
 * @brief Scene auf dem Bildschirm zeichnen
 * 
 * Zeichnet alle Elemente der aktuellen Scene auf dem Bildschirm.
 * Dafür werden die Module: World, EntityHandler und GUI angesprochen,
 * welche dann ihre Elemente dem SDL Wrapper übergeben und danach auf
 * dem Bildschirm angezeigt werden.
 * 
 * @return 0 oder Fehlercode
 */
int Scene_Draw();

/**
 * @brief Lade nächste Scene
 * 
 * Lädt die nächste Scene, bereitet alle Elemente und Parameter vor.
 * Die nächste Scene wird jedoch erst im nächsten Cycle aktiv, bis dahin
 * bleibt die aktuelle Scene unverändert.
 * 
 * @param SceneID
 * @return 0 oder Fehlercode
 */
int Scene_LoadNext(int SceneID);
