/**
 * @file text.h
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
#include "input.h"
#include "SDL_rect.h"


/*
 * Typdeklarationen
 *
 */

/**
 * @brief Struktur mit allen möglichen Parametern und Eigenschaften,
 * die ein Textfeld im Spiel haben kann.
 *
 */

typedef struct {
    SDL_Color textBgc;        //!< Eingabefeld Hintergrundfarbe
    sprite_t textTextur;      //!< Repräsentation der Texttextur
    SDL_Color highlightColor; //!< Auswahlgrenzenfarbe
    SDL_Color textColor;      //!< Text Farbe
    SDL_Rect textRectSize;    //!< Eingabefeld Grösse
    int borderWidth;          //!< Feld Auswahlgrenzenbreite
    int state;                //!< Feld Zustand
    char text[32];            //!< Text String für max. 31 Zeichen
    char *font;               //!< Schriftart
    int index;                //!< Index für Texteingabe
} text_t;


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
 * @brief Text_Init
 *
 * Initialisiert die Parameterliste einer Textstruktur.
 *
 * @param[in] textInput Liste der Textfeldeigenschaften.
 * @return 0 oder error code
 */

int Text_Init(text_t *textInput);

/**
 * @brief Text_Update
 *
 * Überprüft und aktualisiert anhand der inputEvent, den Status eines Textfeld.
 * Bei einem aktiven Textfeld wird der Text anhand der inputEvent, aktualisiert.
 *
 * @param[in] inputEvents Übergabe von Benutzereingabeereignisse
 * @param[in] textInput Liste der Textfeldeigenschaften.
 * @return 0 oder error code
 */

int Text_Update(inputEvent_t *inputEvents, text_t *textInput);

/**
 * @brief Text_Draw
 *
 * Übergibt den zurzeit aktuellsten textInput dem Zeichner.
 * Bei einem Textfeld mit einem aktiven Status wird zusätzlich ein Auswahlrahmen
 * in passender grösse des Textfeldes generiert.
 *
 * @param[in] textInput Liste der Textfeldeigenschaften.
 * @return 0 oder error code
 */

int Text_Draw(text_t *textInput);
