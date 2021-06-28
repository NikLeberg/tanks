/**
 * @file controlsscene.c
 * @author Weber Jan (webej14@bfh.ch)
 * @brief Szene zur Erklärung der Spielsteuerung
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

static guiElement_t bttnInstructionW;        //!< Schriftzug "W : aim to the left"
static guiElement_t bttnInstructionA;        //!< Schriftzug "A : move left"
static guiElement_t bttnInstructionS;        //!< Schriftzug "S : aim to the right"
static guiElement_t bttnInstructionD;        //!< Schriftzug "D : move right"
static guiElement_t bttnInstructionSpaceBar; //!< Schriftzug "Spacebar : fire a missile"
static guiElement_t bttnBackToMenu;          //!< Button öffnet Szene MainMenu


/*
 * Private Funktionsprototypen
 * 
 */

/* ... */


/*
 * Implementation Öffentlicher Funktionen
 * 
 */

int sceneControls_Init(gui_t *sceneControls) {
    // Allen Buttons die Vorlage übergeben
    bttnInstructionW = Scene_ButtonPrototyp();
    bttnInstructionA = Scene_ButtonPrototyp();
    bttnInstructionS = Scene_ButtonPrototyp();
    bttnInstructionD = Scene_ButtonPrototyp();
    bttnInstructionSpaceBar = Scene_ButtonPrototyp();
    bttnBackToMenu = Scene_ButtonPrototyp();

    // Button InstructionW modifizieren
    bttnInstructionW.elementData.button.buttonSize = (SDL_Rect){312, 50, 400, 50};
    bttnInstructionW.elementData.button.buttonColor = COLORBACKGROUND;
    bttnInstructionW.elementData.button.textColor = COLORTEXT;
    bttnInstructionW.elementData.button.highlightColor = COLORBACKGROUND;

    // Button InstructionA modifizieren
    bttnInstructionA.elementData.button.buttonSize = (SDL_Rect){312, 100, 400, 50};
    bttnInstructionA.elementData.button.buttonColor = COLORBACKGROUND;
    bttnInstructionA.elementData.button.textColor = COLORTEXT;
    bttnInstructionA.elementData.button.highlightColor = COLORBACKGROUND;

    // Button InstructionS modifizieren
    bttnInstructionS.elementData.button.buttonSize = (SDL_Rect){312, 150, 400, 50};
    bttnInstructionS.elementData.button.buttonColor = COLORBACKGROUND;
    bttnInstructionS.elementData.button.textColor = COLORTEXT;
    bttnInstructionS.elementData.button.highlightColor = COLORBACKGROUND;

    // Button InstructionD modifizieren
    bttnInstructionD.elementData.button.buttonSize = (SDL_Rect){312, 200, 400, 50};
    bttnInstructionD.elementData.button.buttonColor = COLORBACKGROUND;
    bttnInstructionD.elementData.button.textColor = COLORTEXT;
    bttnInstructionD.elementData.button.highlightColor = COLORBACKGROUND;

    // Button InstructionSpaceBar modifizieren
    bttnInstructionSpaceBar.elementData.button.buttonSize = (SDL_Rect){312, 250, 400, 50};
    bttnInstructionSpaceBar.elementData.button.buttonColor = COLORBACKGROUND;
    bttnInstructionSpaceBar.elementData.button.textColor = COLORTEXT;
    bttnInstructionSpaceBar.elementData.button.highlightColor = COLORBACKGROUND;

    // Button BackToMenu modifizieren
    bttnBackToMenu.elementData.button.buttonSize = (SDL_Rect){412, 400, 200, 50};
    bttnBackToMenu.elementData.button.buttonColor = COLORBGOFFSET;
    bttnBackToMenu.elementData.button.textColor = COLORTEXT;
    bttnBackToMenu.elementData.button.id = BTTN_ID_BACKTOMAINMENU;
    bttnBackToMenu.elementData.button.onClickCallback = Bttn_CallBack_BackToMainMenu;

    // Initialisierung aller Elemente mit Fehlerkontrolle
    if (ERR_OK != GUI_Init(sceneControls) ||
        ERR_OK != Button_Init(&bttnInstructionW.elementData.button, "osans25", "W : aim to the left") ||
        ERR_OK != Button_Init(&bttnInstructionA.elementData.button, "osans25", "A : move left") ||
        ERR_OK != Button_Init(&bttnInstructionS.elementData.button, "osans25", "S : aim to the right") ||
        ERR_OK != Button_Init(&bttnInstructionD.elementData.button, "osans25", "D : move right") ||
        ERR_OK != Button_Init(&bttnInstructionSpaceBar.elementData.button, "osans25", "SPACEBAR : fire a missile") ||
        ERR_OK != Button_Init(&bttnBackToMenu.elementData.button, "osans25", "Back to Menu")) {
        return ERR_FAIL;
    }

    // GUI Elemente der Liste hinzufügen mit Fehlerkontrolle
    if (ERR_OK != GUI_AddElement(&bttnInstructionW, sceneControls) ||
        ERR_OK != GUI_AddElement(&bttnInstructionA, sceneControls) ||
        ERR_OK != GUI_AddElement(&bttnInstructionS, sceneControls) ||
        ERR_OK != GUI_AddElement(&bttnInstructionD, sceneControls) ||
        ERR_OK != GUI_AddElement(&bttnInstructionSpaceBar, sceneControls) ||
        ERR_OK != GUI_AddElement(&bttnBackToMenu, sceneControls)) {
        return ERR_FAIL;
    }

    return ERR_OK;
}


/*
 * Implementation Privater Funktionen
 * 
 */

/* ... */
