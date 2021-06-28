/**
 * @file nameplayerscene.c
 * @author Weber Jan (webej14@bfh.ch)
 * @brief Szene für die Bennenung der zwei Spieler
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

static guiElement_t bttnPlayerA;   //!< Schriftzug "Player A"
static guiElement_t bttnPlayerB;   //!< Namen Text Input Spieler A
static guiElement_t tiNamePlayerA; //!< Schriftzug "Player B"
static guiElement_t tiNamePlayerB; //!< Namen Text Input Spieler B
static guiElement_t bttnNext;      //!< schaltet zur nächsten Szene
static gui_t sceneNamePlayerGUI;   //!< Liste mit sämtlichen GUI Elemente dieser Szene

/*
 * Private Funktionsprototypen
 * 
 */

/* ... */


/*
 * Implementation Öffentlicher Funktionen
 * 
 */

int sceneNamePlayer_Init(sceneNamePlayer_t *sceneNamePlayer) {
    // Allen Buttons und TextInputs die entsprechende Vorlage übergeben
    bttnPlayerA = Scene_ButtonPrototyp();
    bttnPlayerB = Scene_ButtonPrototyp();
    tiNamePlayerA = Scene_TextInputPrototyp();
    tiNamePlayerB = Scene_TextInputPrototyp();
    bttnNext = Scene_ButtonPrototyp();

    // Button PlayerA modifizieren
    bttnPlayerA.elementData.button.buttonSize = (SDL_Rect){156, 100, 200, 100};
    bttnPlayerA.elementData.button.buttonColor = COLORBACKGROUND;
    bttnPlayerA.elementData.button.textColor = COLORTEXT;
    bttnPlayerA.elementData.button.highlightColor = COLORBACKGROUND;

    // Button PlayerB modifizieren
    bttnPlayerB.elementData.button.buttonSize = (SDL_Rect){668, 100, 200, 100};
    bttnPlayerB.elementData.button.buttonColor = COLORBACKGROUND;
    bttnPlayerB.elementData.button.textColor = COLORTEXT;
    bttnPlayerB.elementData.button.highlightColor = COLORBACKGROUND;

    // TextInput NamePlayerA modifizieren
    tiNamePlayerA.elementData.textInput.textRectSize = (SDL_Rect){156, 200, 200, 50};
    tiNamePlayerA.elementData.textInput.textBgc = COLORBGOFFSET;
    tiNamePlayerA.elementData.textInput.textColor = COLORTEXT;

    // TextInput NamePlayerB modifizieren
    tiNamePlayerB.elementData.textInput.textRectSize = (SDL_Rect){668, 200, 200, 50};
    tiNamePlayerB.elementData.textInput.textBgc = COLORBGOFFSET;
    tiNamePlayerB.elementData.textInput.textColor = COLORTEXT;

    // Button Next modifizieren
    bttnNext.elementData.button.buttonSize = (SDL_Rect){462, 400, 100, 50};
    bttnNext.elementData.button.buttonColor = COLORBGOFFSET;
    bttnNext.elementData.button.textColor = COLORTEXT;
    bttnNext.elementData.button.onClickCallback = Bttn_CallBack_Next;

    // Initialisierung aller Elemente mit Fehlerkontrolle
    if (ERR_OK != GUI_Init(&sceneNamePlayerGUI) ||
        ERR_OK != Button_Init(&bttnPlayerA.elementData.button, "osans25", "Player A") ||
        ERR_OK != Text_Init(&tiNamePlayerA.elementData.textInput) ||
        ERR_OK != Button_Init(&bttnPlayerB.elementData.button, "osans25", "Player B") ||
        ERR_OK != Text_Init(&tiNamePlayerB.elementData.textInput) ||
        ERR_OK != Button_Init(&bttnNext.elementData.button, "osans25", "Next")) {
        return ERR_FAIL;
    }

    // GUI Elemente der Liste hinzufügen mit Fehlerkontrolle
    if (ERR_OK != GUI_AddElement(&bttnPlayerA, &sceneNamePlayerGUI) ||
        ERR_OK != GUI_AddElement(&tiNamePlayerA, &sceneNamePlayerGUI) ||
        ERR_OK != GUI_AddElement(&bttnPlayerB, &sceneNamePlayerGUI) ||
        ERR_OK != GUI_AddElement(&tiNamePlayerB, &sceneNamePlayerGUI) ||
        ERR_OK != GUI_AddElement(&bttnNext, &sceneNamePlayerGUI)) {
        return ERR_FAIL;
    }

    // Alle Elemente an die Struktur übergeben
    sceneNamePlayer->bttnPlayerA = &bttnPlayerA;
    sceneNamePlayer->tiNamePlayerA = &tiNamePlayerA;
    sceneNamePlayer->bttnPlayerB = &bttnPlayerB;
    sceneNamePlayer->tiNamePlayerB = &tiNamePlayerB;
    sceneNamePlayer->bttnNext = &bttnNext;
    sceneNamePlayer->sceneNamPlaGUI = sceneNamePlayerGUI;

    return ERR_OK;
}


/*
 * Implementation Privater Funktionen
 * 
 */

/* ... */
