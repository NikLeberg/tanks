/**
 * @file button.h
 * @author Hinnaoui Angelo (hinna2@bfh.ch)
 * @brief Buttons als GUI Element
 * @version 0.01
 * @date 2021-03-24
 *
 * @copyright Copyright (c) 2021 Hinnaoui Angelo
 *
 */

#pragma once


/*
 * Includes
 *
 */

#include "sdlWrapper.h"
#include "scene.h"


/*
 * Typdeklarationen
 *
 */

/**
 * @brief Tastenstruktur mit allen möglichen Parametern und Eigenschaften,
 * die ein Taster im Spiel haben kann.
 *
 */

typedef struct {
    int id;                       //!< Tasten Identifikation
    sprite_t textTextur;          //!< Repräsentation der Texttextur
    void (*onClickCallback)(int); //!< Funktions Aufruf bei Tastenklick
    SDL_Color buttonColor;        //!< Tasten Farbe
    SDL_Color highlightColor;     //!< Auswahlgrenzenfarbe
    SDL_Color textColor;          //!< Text Farbe
    SDL_Rect buttonSize;          //!< Tasten Grösse
    int borderWidth;              //!< Tasten Auswahlgrenzenbreite
    int state;                    //!< Tasten Zustand
} button_t;


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
 * @brief Button_Init
 *
 * Sammelt Informationen zur Tasten Initialisierung.
 * Sind alle Parameter vollstaendig, wird dass Packet an die Funktion Sprite_CreateText(),
 * zur Erstellung der Textur, uebergeben.
 * Sind die Parameter unvollstaendig, wird ein error code ausgegeben.
 *
 * @param[in] button Parameter und Eigenschaften
 * @param[in] font Schriftart der Tastenbeschriftung
 * @param[in] text für die Tastenbeschriftung
 * @return 0 oder error code
 */

int Button_Init(button_t *button, char *font, char *text);

/**
 * @brief Button_Update
 *
 * Aktualisiert, bei Interaktion, das Tastenfeld mit akustischer und visuelle Rueckmeldung.
 * Bei einem Tastendruck, wird ein Callback ausgeführt.
 * Sind die Parameter unvollstaendig, wird ein error code ausgegeben.
 *
 * @param[in] inputEvents Übergabe von Mausereignisse
 * @param[in] button Pointer auf zu aktualisierenden Taster
 * @return 0 oder error code
 */

int Button_Update(inputEvent_t *inputEvents, button_t *button);

/**
 * @brief Button_Draw
 *
 * Nach jeder Tasten Aktualisierung, wird das aktuelle Aussehen des Taster an den Zeichner uebergeben.
 *
 * @param[in] button Zu zeichnende Taste
 * @return 0 oder error code
 */

int Button_Draw(button_t *button);
