/**
 * @file mainmenuscene.c
 * @author Weber Jan (webej14@bfh.ch)
 * @brief Szene des Hauptmenüs
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

static guiElement_t bttnNewGame;    //!< startet ein neues Spiel
static guiElement_t bttnControls;   //!< öffnet die Ansicht der im Spiel Steuerung
static guiElement_t bttnChangeName; //!< öffnet die Szene NamePlayer erneut
static guiElement_t bttnQuit;       //!< Schliesst das Programm
static gui_t sceneMainMenuGUI;      //!< Liste mit sämtlichen GUI Elemente dieser Szene


/*
 * Private Funktionsprototypen
 * 
 */

/* ... */


/*
 * Implementation Öffentlicher Funktionen
 * 
 */

int sceneMainMenu_Init(sceneMainMenu_t *sceneMainMenu) {
    // Allen Buttons die Vorlage übergeben
    bttnNewGame = Scene_ButtonPrototyp();
    bttnControls = Scene_ButtonPrototyp();
    bttnChangeName = Scene_ButtonPrototyp();
    bttnQuit = Scene_ButtonPrototyp();

    // Button NewGame modifizieren
    bttnNewGame.elementData.button.buttonSize = (SDL_Rect){262, 125, 200, 100};
    bttnNewGame.elementData.button.buttonColor = COLORBGOFFSET;
    bttnNewGame.elementData.button.textColor = COLORTEXT;
    bttnNewGame.elementData.button.onClickCallback = Bttn_CallBack_NewGame;

    // Button Controls modifizieren
    bttnControls.elementData.button.buttonSize = (SDL_Rect){562, 125, 200, 100};
    bttnControls.elementData.button.buttonColor = COLORBGOFFSET;
    bttnControls.elementData.button.textColor = COLORTEXT;
    bttnControls.elementData.button.onClickCallback = Bttn_CallBack_Controls;

    // Button ChangeName modifizieren
    bttnChangeName.elementData.button.buttonSize = (SDL_Rect){262, 325, 200, 100};
    bttnChangeName.elementData.button.buttonColor = COLORBGOFFSET;
    bttnChangeName.elementData.button.textColor = COLORTEXT;
    bttnChangeName.elementData.button.onClickCallback = Bttn_CallBack_ChangeName;

    // Button QuitGame modifizieren
    bttnQuit.elementData.button.buttonSize = (SDL_Rect){562, 325, 200, 100};
    bttnQuit.elementData.button.buttonColor = COLORBGOFFSET;
    bttnQuit.elementData.button.textColor = COLORTEXT;
    bttnQuit.elementData.button.onClickCallback = Bttn_CallBack_Quit;

    // Initialisierung aller Elemente mit Fehlerkontrolle
    if (ERR_OK != GUI_Init(&sceneMainMenuGUI) ||
        ERR_OK != Button_Init(&bttnNewGame.elementData.button, "osans25", "New Game") ||
        ERR_OK != Button_Init(&bttnControls.elementData.button, "osans25", "Controls") ||
        ERR_OK != Button_Init(&bttnChangeName.elementData.button, "osans25", "Change Names") ||
        ERR_OK != Button_Init(&bttnQuit.elementData.button, "osans25", "Quit Game")) {
        return ERR_FAIL;
    }

    // GUI Elemente der Liste hinzufügen mit Fehlerkontrolle
    if (ERR_OK != GUI_AddElement(&bttnNewGame, &sceneMainMenuGUI) ||
        ERR_OK != GUI_AddElement(&bttnControls, &sceneMainMenuGUI) ||
        ERR_OK != GUI_AddElement(&bttnChangeName, &sceneMainMenuGUI) ||
        ERR_OK != GUI_AddElement(&bttnQuit, &sceneMainMenuGUI)) {
        return ERR_FAIL;
    }

    // Alle Elemente an die Struktur übergeben
    sceneMainMenu->bttnChangeName = &bttnChangeName;
    sceneMainMenu->bttnControls = &bttnControls;
    sceneMainMenu->bttnNewGame = &bttnNewGame;
    sceneMainMenu->bttnQuit = &bttnQuit;
    sceneMainMenu->sceneMainMenuGUI = sceneMainMenuGUI;

    return ERR_OK;
}


/*
 * Implementation Privater Funktionen
 * 
 */

/* ... */
