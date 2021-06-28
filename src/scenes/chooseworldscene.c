/**
 * @file chooseworldscene.c
 * @author Weber Jan (webej14@bfh.ch)
 * @brief Szene zur Auswahl der Spielwelt
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

static guiElement_t bttnChooseWorld; //!< Schriftzug "Choose a World"
static guiElement_t bttnEvergreen;   //!< Button lädt Welt Evergreen
static guiElement_t bttnAquaduct;    //!< Button lädt Welt Aquaduct
static gui_t sceneChooseWorldGUI;    //!< Liste mit sämtlichen GUI Elemente dieser Szene


/*
 * Private Funktionsprototypen
 * 
 */

/* ... */


/*
 * Implementation Öffentlicher Funktionen
 * 
 */
int sceneChooseWorld_Init(sceneChooseWorld_t *sceneChooseWorld) {
    // Allen Buttons die Vorlage übergeben
    bttnChooseWorld = Scene_ButtonPrototyp();
    bttnEvergreen = Scene_ButtonPrototyp();
    bttnAquaduct = Scene_ButtonPrototyp();

    // Button ChooseWorld modifizieren
    bttnChooseWorld.elementData.button.buttonSize = (SDL_Rect){412, 144, 200, 100};
    bttnChooseWorld.elementData.button.buttonColor = COLORBACKGROUND;
    bttnChooseWorld.elementData.button.textColor = COLORTEXT;
    bttnChooseWorld.elementData.button.highlightColor = COLORBACKGROUND;

    // Button Evergreen modifizieren
    bttnEvergreen.elementData.button.buttonSize = (SDL_Rect){262, 244, 200, 100};
    bttnEvergreen.elementData.button.buttonColor = COLORBGOFFSET;
    bttnEvergreen.elementData.button.textColor = COLORTEXT;
    bttnEvergreen.elementData.button.id = BTTN_ID_WORLD_EVERGREEN;
    bttnEvergreen.elementData.button.onClickCallback = Bttn_CallBack_LoadWorld;

    // Button Aquaduct modifizieren
    bttnAquaduct.elementData.button.buttonSize = (SDL_Rect){562, 244, 200, 100};
    bttnAquaduct.elementData.button.buttonColor = COLORBGOFFSET;
    bttnAquaduct.elementData.button.textColor = COLORTEXT;
    bttnAquaduct.elementData.button.id = BTTN_ID_WORLD_AQUADUCT;
    bttnAquaduct.elementData.button.onClickCallback = Bttn_CallBack_LoadWorld;

    // Initialisierung aller Elemente mit Fehlerkontrolle
    if (ERR_OK != GUI_Init(&sceneChooseWorldGUI) ||
        ERR_OK != Button_Init(&bttnChooseWorld.elementData.button, "osans25", "Choose a World") ||
        ERR_OK != Button_Init(&bttnEvergreen.elementData.button, "osans25", "Evergreen") ||
        ERR_OK != Button_Init(&bttnAquaduct.elementData.button, "osans25", "Aquaduct")) {
        return ERR_FAIL;
    }

    // GUI Elemente der Liste hinzufügen mit Fehlerkontrolle
    if (ERR_OK != GUI_AddElement(&bttnChooseWorld, &sceneChooseWorldGUI) ||
        ERR_OK != GUI_AddElement(&bttnEvergreen, &sceneChooseWorldGUI) ||
        ERR_OK != GUI_AddElement(&bttnAquaduct, &sceneChooseWorldGUI)) {
        return ERR_FAIL;
    }

    // Alle Elemente an die Struktur übergeben
    sceneChooseWorld->bttnChooseWorld = &bttnChooseWorld;
    sceneChooseWorld->bttnEvergreen = &bttnEvergreen;
    sceneChooseWorld->bttnAquaduct = &bttnAquaduct;
    sceneChooseWorld->sceneChooseWorldGUI = sceneChooseWorldGUI;

    return ERR_OK;
}


/*
 * Implementation Privater Funktionen
 * 
 */

/* ... */
