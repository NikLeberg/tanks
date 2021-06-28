/**
 * @file victoryscene.c
 * @author Weber Jan (webej14@bfh.ch)
 * @brief Szene die den Spieler angibt der gewonnen hat
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

static guiElement_t bttnPlayerXWon; //!< Schrfitzug "Player X has won!"
static guiElement_t bttnBackToMenu; //!< Button öffnet Szene MainMenu


/*
 * Private Funktionsprototypen
 * 
 */

/* ... */


/*
 * Implementation Öffentlicher Funktionen
 * 
 */

int sceneVictory_Init(gui_t *sceneVictory) {
    // Allen Buttons die Vorlage übergeben
    bttnPlayerXWon = Scene_ButtonPrototyp();
    bttnBackToMenu = Scene_ButtonPrototyp();

    // Button PlayerAWon modifizieren
    bttnPlayerXWon.elementData.button.buttonSize = (SDL_Rect){312, 150, 400, 100};
    bttnPlayerXWon.elementData.button.buttonColor = COLORBACKGROUND;
    bttnPlayerXWon.elementData.button.highlightColor = COLORBACKGROUND;
    bttnPlayerXWon.elementData.button.textColor = COLORTEXT;

    // Button BackToMenu modifizieren
    bttnBackToMenu.elementData.button.buttonSize = (SDL_Rect){412, 250, 200, 50};
    bttnBackToMenu.elementData.button.buttonColor = COLORBGOFFSET;
    bttnBackToMenu.elementData.button.textColor = COLORTEXT;
    bttnBackToMenu.elementData.button.id = BTTN_ID_BACKTOMAINMENU_CLOSE_WORLD;
    bttnBackToMenu.elementData.button.onClickCallback = Bttn_CallBack_BackToMainMenu;

    // Initialisierung aller Elemente mit Fehlerkontrolle
    if (ERR_OK != GUI_Init(sceneVictory) ||
        ERR_OK != Button_Init(&bttnPlayerXWon.elementData.button, "osans25", "Winner placeholder") ||
        ERR_OK != Button_Init(&bttnBackToMenu.elementData.button, "osans25", "Back to Menu")) {
        return ERR_FAIL;
    }

    // GUI Elemente der Liste hinzufügen mit Fehlerkontrolle
    if (ERR_OK != GUI_AddElement(&bttnPlayerXWon, sceneVictory) ||
        ERR_OK != GUI_AddElement(&bttnBackToMenu, sceneVictory)) {
        return ERR_FAIL;
    }

    return ERR_OK;
}

int sceneVictory_Update(gui_t *sceneVictory, player_t *winner) {
    // Zerstören der alten Button Textur
    SDL_DestroyTexture(bttnPlayerXWon.elementData.button.textTextur.texture);

    // Schrfitzug "Player A has won!" vorbereiten mit entsprechendem Namen
    char textOutput[64];
    strcpy(textOutput, winner->name);
    strcat(textOutput, " has won!");

    // Initialisierung aller Elemente mit Fehlerkontrolle
    if (ERR_OK != Button_Init(&bttnPlayerXWon.elementData.button, "osans25", textOutput)) {
        return ERR_FAIL;
    }

    // GUI Elemente der Liste hinzufügen mit Fehlerkontrolle
    if (ERR_OK != GUI_AddElement(&bttnPlayerXWon, sceneVictory)) {
        return ERR_FAIL;
    }

    return ERR_OK;
}


/*
 * Implementation Privater Funktionen
 * 
 */

/* ... */
