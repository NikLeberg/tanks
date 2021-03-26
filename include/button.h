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


/*
 * Typdeklarationen
 *
 */

/**
 * @brief Tastenstruktur
 *
 */

typedef struct {
    int id;                       //!< Tasten Identifikation
    sprite_t textur;              //!< representation einer Fläche (Textur)
    void (*onClickCallback)(int); //!< Funktions aufruf bei tastenklick
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
 * @brief Button_Update
 * @param inputEvents Übergabe von Mausereignisse
 * @param button Pointer auf zu aktualisierenden Taster
 * @return 0 oder error code
 */

int Button_Update(void *inputEvents, button_t *button);

/**
 * @brief Button_Draw
 * @param button Zu zeichnende Taste
 * @return 0 oder error code
 */

int Button_Draw(button_t *button);
