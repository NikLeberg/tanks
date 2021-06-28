/**
 * @file ingamescene.c
 * @author Weber Jan (webej14@bfh.ch)
 * @brief Szene des eigentlichen Spiels
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

static guiElement_t bttnPlayerNameA;    //!< Schrfitzug mit Namen des Spielers A
static guiElement_t bttnPlayerNameB;    //!< Schrfitzug mit Namen des Spielers B
static guiElement_t bttnHPBarBGA;       //!< Hintergund des Lebensbalken, zeigt maximale Lebenspunkte an
static guiElement_t bttnHPBarBGB;       //!< Hintergund des Lebensbalken, zeigt maximale Lebenspunkte an
static guiElement_t bttnHPBarCoverA;    //!< Abdeckung des Lebensbalken, zeigt die aktuellen Lebenspunkte des Spielers A an
static guiElement_t bttnHPBarCoverB;    //!< Abdeckung des Lebensbalken, zeigt die aktuellen Lebenspunkte des Spielers B an
static guiElement_t bttnBackToMainMenu; //!< öffnet die Szene MainMenu
static gui_t sceneInGameGUI;            //!< Liste mit sämtlichen GUI Elemente dieser Szene


/*
 * Private Funktionsprototypen
 * 
 */

/* ... */


/*
 * Implementation Öffentlicher Funktionen
 * 
 */

int sceneInGame_Init(sceneInGame_t *sceneInGame, player_t playerA, player_t playerB, inputEvent_t input) {

    // Allen Buttons die Vorlage übergeben
    bttnPlayerNameA = Scene_ButtonPrototyp();
    bttnPlayerNameB = Scene_ButtonPrototyp();
    bttnHPBarBGA = Scene_ButtonPrototyp();
    bttnHPBarBGB = Scene_ButtonPrototyp();
    bttnHPBarCoverA = Scene_ButtonPrototyp();
    bttnHPBarCoverB = Scene_ButtonPrototyp();
    bttnBackToMainMenu = Scene_ButtonPrototyp();

    // Button PlayerNameA modifizieren
    bttnPlayerNameA.elementData.button.buttonSize = (SDL_Rect){001, 001, 200, 30};
    bttnPlayerNameA.elementData.button.textColor = COLORTEXT;
    if (input.currentPlayer->name == playerA.name) {
        bttnPlayerNameA.elementData.button.buttonColor = COLORACTIVPLAYER;
        bttnPlayerNameA.elementData.button.highlightColor = COLORACTIVPLAYER;
    } else {
        bttnPlayerNameA.elementData.button.buttonColor = COLORINGAMEBTTN;
        bttnPlayerNameA.elementData.button.highlightColor = COLORINGAMEBTTN;
    }

    // Button PlayerNameB modifizieren
    bttnPlayerNameB.elementData.button.buttonSize = (SDL_Rect){824, 001, 200, 30};
    bttnPlayerNameB.elementData.button.textColor = COLORTEXT;
    if (input.currentPlayer->name == playerB.name) {
        bttnPlayerNameB.elementData.button.buttonColor = COLORACTIVPLAYER;
        bttnPlayerNameB.elementData.button.highlightColor = COLORACTIVPLAYER;
    } else {
        bttnPlayerNameB.elementData.button.buttonColor = COLORINGAMEBTTN;
        bttnPlayerNameB.elementData.button.highlightColor = COLORINGAMEBTTN;
    }

    // Button HPBarBackGroundA modifizieren
    bttnHPBarBGA.elementData.button.buttonSize = (SDL_Rect){001, 041, 200, 30};
    bttnHPBarBGA.elementData.button.buttonColor = COLORHPBARBG;
    bttnHPBarBGA.elementData.button.highlightColor = COLORHPBARBG;

    // Button HPBarBackgroundB modifizieren modifizieren
    bttnHPBarBGB.elementData.button.buttonSize = (SDL_Rect){824, 041, 200, 30};
    bttnHPBarBGB.elementData.button.buttonColor = COLORHPBARBG;
    bttnHPBarBGB.elementData.button.highlightColor = COLORHPBARBG;

    // Button HPBarCoverA modifizieren modifizieren
    bttnHPBarCoverA.elementData.button.buttonSize = (SDL_Rect){001, 041, (playerA.healthpoints * 2), 30};
    bttnHPBarCoverA.elementData.button.buttonColor = COLORHPBARCOVER;
    bttnHPBarCoverA.elementData.button.highlightColor = COLORHPBARCOVER;

    // Button HPBarCoverB modifizieren modifizieren
    bttnHPBarCoverB.elementData.button.buttonSize = (SDL_Rect){824, 041, (playerB.healthpoints * 2), 30};
    bttnHPBarCoverB.elementData.button.buttonColor = COLORHPBARCOVER;
    bttnHPBarCoverB.elementData.button.highlightColor = COLORHPBARCOVER;

    // Button BackToMainMenu modifizieren modifizieren
    bttnBackToMainMenu.elementData.button.buttonSize = (SDL_Rect){412, 001, 200, 30};
    bttnBackToMainMenu.elementData.button.buttonColor = COLORINGAMEBTTN;
    bttnBackToMainMenu.elementData.button.textColor = COLORTEXT;
    bttnBackToMainMenu.elementData.button.id = BTTN_ID_BACKTOMAINMENU_CLOSE_WORLD;
    bttnBackToMainMenu.elementData.button.onClickCallback = Bttn_CallBack_BackToMainMenu;

    char HPBarA[4]; //!< Lebenspunkte des Spielers A
    char HPBarB[4]; //!< Lebenspunkte des Spielers B

    // Vorbereitung der Ausgabe der aktuellen Lebenspunkte des Spielers A
    if (playerA.healthpoints > 0) {
        SDL_snprintf(HPBarA, 4, "%d", playerA.healthpoints);
    } else {
        SDL_snprintf(HPBarA, 4, "%d", 0);
    }
    // Vorbereitung der Ausgabe der aktuellen Lebenspunkte des Spielers B
    if (playerB.healthpoints > 0) {
        SDL_snprintf(HPBarB, 4, "%d", playerB.healthpoints);
    } else {
        SDL_snprintf(HPBarB, 4, "%d", 0);
    }

    // Initialisierung aller Elemente mit Fehlerkontrolle
    if (ERR_OK != GUI_Init(&sceneInGameGUI) ||
        ERR_OK != Button_Init(&bttnPlayerNameA.elementData.button, "osans25", " ") ||
        ERR_OK != Button_Init(&bttnPlayerNameB.elementData.button, "osans25", " ") ||
        ERR_OK != Button_Init(&bttnHPBarCoverA.elementData.button, "osans25", HPBarA) ||
        ERR_OK != Button_Init(&bttnHPBarCoverB.elementData.button, "osans25", HPBarB) ||
        ERR_OK != Button_Init(&bttnBackToMainMenu.elementData.button, "osans25", "Back to Menu")) {
        return ERR_FAIL;
    }

    // GUI Elemente der Liste hinzufügen mit Fehlerkontrolle
    if (ERR_OK != GUI_AddElement(&bttnPlayerNameA, &sceneInGameGUI) ||
        ERR_OK != GUI_AddElement(&bttnPlayerNameB, &sceneInGameGUI) ||
        ERR_OK != GUI_AddElement(&bttnHPBarCoverA, &sceneInGameGUI) ||
        ERR_OK != GUI_AddElement(&bttnHPBarCoverB, &sceneInGameGUI) ||
        ERR_OK != GUI_AddElement(&bttnHPBarBGA, &sceneInGameGUI) ||
        ERR_OK != GUI_AddElement(&bttnHPBarBGB, &sceneInGameGUI) ||
        ERR_OK != GUI_AddElement(&bttnBackToMainMenu, &sceneInGameGUI)) {
        return ERR_FAIL;
    }

    // Alle Elemente an die Struktur übergeben
    sceneInGame->bttnPlayerNameA = &bttnPlayerNameA;
    sceneInGame->bttnPlayerNameB = &bttnPlayerNameB;
    sceneInGame->bttnHPBarBGA = &bttnHPBarBGA;
    sceneInGame->bttnHPBarBGB = &bttnHPBarBGB;
    sceneInGame->bttnHPBarCoverA = &bttnHPBarCoverA;
    sceneInGame->bttnHPBarCoverB = &bttnHPBarCoverB;
    sceneInGame->bttnBackToMaiMen = &bttnBackToMainMenu;
    sceneInGame->sceneInGameGUI = sceneInGameGUI;

    return ERR_OK;
}

int sceneInGame_Update(sceneInGame_t *sceneInGame, player_t playerA, player_t playerB, inputEvent_t input) {

    // Zerstören der alten Button Textur
    SDL_DestroyTexture(bttnPlayerNameA.elementData.button.textTextur.texture);
    SDL_DestroyTexture(bttnPlayerNameB.elementData.button.textTextur.texture);
    SDL_DestroyTexture(bttnHPBarCoverA.elementData.button.textTextur.texture);
    SDL_DestroyTexture(bttnHPBarCoverB.elementData.button.textTextur.texture);

    // Allen Buttons die Vorlage übergeben
    bttnHPBarCoverA = Scene_ButtonPrototyp();
    bttnHPBarCoverB = Scene_ButtonPrototyp();

    // Button PlayerNameA modifizieren
    if (input.currentPlayer->name == playerA.name) {
        bttnPlayerNameA.elementData.button.buttonColor = COLORACTIVPLAYER;
        bttnPlayerNameA.elementData.button.highlightColor = COLORACTIVPLAYER;
    } else {
        bttnPlayerNameA.elementData.button.buttonColor = COLORINGAMEBTTN;
        bttnPlayerNameA.elementData.button.highlightColor = COLORINGAMEBTTN;
    }

    // Button PlayerNameB modifizieren
    if (input.currentPlayer->name == playerB.name) {
        bttnPlayerNameB.elementData.button.buttonColor = COLORACTIVPLAYER;
        bttnPlayerNameB.elementData.button.highlightColor = COLORACTIVPLAYER;
    } else {
        bttnPlayerNameB.elementData.button.buttonColor = COLORINGAMEBTTN;
        bttnPlayerNameB.elementData.button.highlightColor = COLORINGAMEBTTN;
    }

    // Button HPBarCoverA modifizieren modifizieren
    bttnHPBarCoverA.elementData.button.buttonSize = (SDL_Rect){001, 041, (playerA.healthpoints * 2), 30};
    bttnHPBarCoverA.elementData.button.buttonColor = COLORHPBARCOVER;
    bttnHPBarCoverA.elementData.button.highlightColor = COLORHPBARCOVER;

    // Button HPBarCoverB modifizieren modifizieren
    bttnHPBarCoverB.elementData.button.buttonSize = (SDL_Rect){824, 041, (playerB.healthpoints * 2), 30};
    bttnHPBarCoverB.elementData.button.buttonColor = COLORHPBARCOVER;
    bttnHPBarCoverB.elementData.button.highlightColor = COLORHPBARCOVER;

    char HPBarA[4]; //!< Lebenspunkte des Spielers A
    char HPBarB[4]; //!< Lebenspunkte des Spielers B

    // Vorbereitung der Ausgabe der aktuellen Lebenspunkte des Spielers A
    if (playerA.healthpoints > 0) {
        SDL_snprintf(HPBarA, 4, "%d", playerA.healthpoints);
    } else {
        SDL_snprintf(HPBarA, 4, "%d", 0);
    }
    // Vorbereitung der Ausgabe der aktuellen Lebenspunkte des Spielers B
    if (playerB.healthpoints > 0) {
        SDL_snprintf(HPBarB, 4, "%d", playerB.healthpoints);
    } else {
        SDL_snprintf(HPBarB, 4, "%d", 0);
    }

    // Initialisierung aller Elemente mit Fehlerkontrolle
    if (ERR_OK != Button_Init(&bttnPlayerNameA.elementData.button, "osans25", playerA.name) ||
        ERR_OK != Button_Init(&bttnPlayerNameB.elementData.button, "osans25", playerB.name) ||
        ERR_OK != Button_Init(&bttnHPBarCoverA.elementData.button, "osans25", HPBarA) ||
        ERR_OK != Button_Init(&bttnHPBarCoverB.elementData.button, "osans25", HPBarB)) {
        return ERR_FAIL;
    }

    // GUI Elemente der Liste hinzufügen mit Fehlerkontrolle
    if (ERR_OK != GUI_AddElement(&bttnPlayerNameA, &sceneInGameGUI) ||
        ERR_OK != GUI_AddElement(&bttnPlayerNameB, &sceneInGameGUI) ||
        ERR_OK != GUI_AddElement(&bttnHPBarCoverA, &sceneInGameGUI) ||
        ERR_OK != GUI_AddElement(&bttnHPBarCoverB, &sceneInGameGUI)) {
        return ERR_FAIL;
    }

    // Alle Elemente an die Struktur übergeben
    sceneInGame->sceneInGameGUI = sceneInGameGUI;

    return ERR_OK;
}


/*
 * Implementation Privater Funktionen
 * 
 */

/* ... */
