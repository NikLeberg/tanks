/**
 * @file scene_err_fail.c
 * @author Weber Jan (webej14@bfh.ch)
 * @brief Szene die bei Fehler erscheint
 * @version 0.1
 * @date 2021-04-16
 * 
 * @copyright Copyright (c) 2021 Weber Jan
 * 
 */


/*
 * Includes
 * 
 */

#include "scene.h"


/*
 * Typdeklarationen
 * 
 */

/* ... */


/*
 * Variablendeklarationen
 * 
 */

static guiElement_t bttnSomethingWentWrong; //!< Schriftzug "Oops something went wrong.."
static guiElement_t bttnQuitGame;           //!< Schriftzug "Quit Game"

/*
 * Private Funktionsprototypen
 * 
 */

/* ... */


/*
 * Implementation Öffentlicher Funktionen
 * 
 */

int sceneErrFail_Init(gui_t *sceneErrFail) {
    // Allen Buttons die Vorlage übergeben
    bttnSomethingWentWrong = Scene_ButtonPrototyp();
    bttnQuitGame = Scene_ButtonPrototyp();

    // Button SomethingWentWrong modifizieren
    bttnSomethingWentWrong.elementData.button.buttonSize = (SDL_Rect){312, 150, 400, 100};
    bttnSomethingWentWrong.elementData.button.buttonColor = COLORBACKGROUND;
    bttnSomethingWentWrong.elementData.button.highlightColor = COLORBACKGROUND;
    bttnSomethingWentWrong.elementData.button.textColor = COLORTEXT;

    // Button GuitGame modifizieren
    bttnQuitGame.elementData.button.buttonSize = (SDL_Rect){412, 250, 200, 50};
    bttnQuitGame.elementData.button.buttonColor = COLORBGOFFSET;
    bttnQuitGame.elementData.button.textColor = COLORTEXT;
    bttnQuitGame.elementData.button.onClickCallback = Bttn_CallBack_Quit;

    // Initialisierung aller Elemente mit Fehlerkontrolle
    if (ERR_OK != GUI_Init(sceneErrFail) ||
        ERR_OK != Button_Init(&bttnSomethingWentWrong.elementData.button, "osans25", "Oops something went wrong..") ||
        ERR_OK != Button_Init(&bttnQuitGame.elementData.button, "osans25", "Quit Game")) {
        return ERR_FAIL;
    }

    // GUI Elemente der Liste hinzufügen mit Fehlerkontrolle
    if (ERR_OK != GUI_AddElement(&bttnSomethingWentWrong, sceneErrFail) ||
        ERR_OK != GUI_AddElement(&bttnQuitGame, sceneErrFail)) {
        return ERR_FAIL;
    }

    return ERR_OK;
}


/*
 * Implementation Privater Funktionen
 * 
 */

/* ... */
