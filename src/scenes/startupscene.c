/**
 * @file startupscene.c
 * @author Weber Jan (webej14@bfh.ch)
 * @brief Szene die beim Start für einige Sekunden gezeigt wird
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

static guiElement_t bttnGameName;      //!< Schrfitzug "TANKS"
static guiElement_t bttnCreatorsNames; //!< Schrfitzug "leuen4, stols4, hinna2, webej14"


/*
 * Private Funktionsprototypen
 * 
 */

/* ... */


/*
 * Implementation Öffentlicher Funktionen
 * 
 */

int sceneStartup_Init(gui_t *sceneStartUp) {
    // Allen Buttons die Vorlage übergeben
    bttnGameName = Scene_ButtonPrototyp();
    bttnCreatorsNames = Scene_ButtonPrototyp();

    // Button GameName modifizieren
    bttnGameName.elementData.button.buttonSize = (SDL_Rect){412, 100, 200, 100};
    bttnGameName.elementData.button.buttonColor = COLORBACKGROUND;
    bttnGameName.elementData.button.textColor = COLORTEXT;
    bttnGameName.elementData.button.highlightColor = COLORBACKGROUND;

    // Button CreatorsNames modifizieren
    bttnCreatorsNames.elementData.button.buttonSize = (SDL_Rect){312, 300, 400, 100};
    bttnCreatorsNames.elementData.button.buttonColor = COLORBACKGROUND;
    bttnCreatorsNames.elementData.button.textColor = COLORTEXT;
    bttnCreatorsNames.elementData.button.highlightColor = COLORBACKGROUND;

    // Initialisierung aller Elemente mit Fehlerkontrolle
    if (ERR_OK != GUI_Init(sceneStartUp) ||
        ERR_OK != Button_Init(&bttnGameName.elementData.button, "osans25", "TANKS") ||
        ERR_OK != Button_Init(&bttnCreatorsNames.elementData.button, "osans25", "leuen4, stols4, hinna2, webej14")) {
        return ERR_FAIL;
    }

    // GUI Elemente der Liste hinzufügen mit Fehlerkontrolle
    if (ERR_OK != GUI_AddElement(&bttnGameName, sceneStartUp) ||
        ERR_OK != GUI_AddElement(&bttnCreatorsNames, sceneStartUp)) {
        return ERR_FAIL;
    }

    return ERR_OK;
}


/*
 * Implementation Privater Funktionen
 * 
 */

/* ... */
