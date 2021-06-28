/**
 * @file main.c
 * @author Weber Jan (webej14@bfh.ch)
 * @brief Main
 * @version 0.1
 * @date 2021-03-19
 * 
 * @copyright Copyright (c) 2021 Weber Jan
 * 
 */


/*
 * Includes
 * 
 */

#include <stdio.h>
#include <stdbool.h>

#include "sdlWrapper.h"
#include "error.h"
#include "scene.h"
#include "entity.h"
#include "entityHandler.h"
#include "entities/tank.h"

/*
 * Typdeklarationen
 * 
 */


/*
 * Variablendeklarationen
 * 
 */


/**
 * @brief Main
 * 
 * @param argc Anzahl Befehlszeilenargumente
 * @param argv Array der Argumente
 * @return int immer 0
 */
int main(int argc, char *argv[]) {
    (void)argc;
    (void)argv;
    static gui_t sceneStartUp;             //!< GUI Elemente Liste
    static sceneNamePlayer_t sceneNamPla;  //!< Struktur mit allen Elementen der Szene
    static gui_t sceneNamePlayer;          //!< GUI Elemente Liste
    static sceneMainMenu_t sceneMaiMen;    //!< Struktur mit allen Elementen der Szene
    static gui_t sceneMainMenu;            //!< GUI Elemente Liste
    static gui_t sceneControls;            //!< GUI Elemente Liste
    static sceneInGame_t sceneInGam;       //!< Struktur mit allen Elementen der Szene
    static gui_t sceneInGame;              //!< GUI Elemente Liste
    static sceneChooseWorld_t sceneChoWor; //!< Struktur mit allen Elementen der Szene
    static gui_t sceneChooseWorld;         //!< GUI Elemente Liste
    static gui_t sceneVictory;             //!< GUI Elemente Liste
    static gui_t sceneErrFail;             //!< GUI Elemente Liste
    static gui_t *sceneCurrent;            //!< Zeiger auf aktuelle GUI Elemente Liste
    static player_t playerA;               //!< Spieler A Struktur
    static player_t playerB;               //!< Spieler B Struktur
    static entity_t *tankPlayerA;          //!< Zeiger auf Panzer von Spieler A
    static entity_t *tankPlayerB;          //!< Zeiger auf Panzer von Spieler B
    static int startUpLoop = 0;            //!< Delay beim StartUp


    // SDL Init mit Fenster Grösse 1024x576
    if (ERR_OK != SDLW_Init(1024, 576)) {
        currentSceneID = SCENE_ERR_FAIL;
    }

    // SDL lädt Config File
    if (ERR_OK != SDLW_LoadResources("assets/config.cfg")) {
        currentSceneID = SCENE_ERR_FAIL;
    }

    // startet SDL Text Input Aufnahme
    SDL_StartTextInput();


    // Benutzereingaben variablen
    SDL_Event event;
    inputEvent_t inputEvent = {0};
    inputEvent.currentPlayer = &playerA;

    // Szene Initialisierung
    if (ERR_OK != Scene_Init(playerA, playerB, inputEvent, &sceneStartUp,
                             &sceneNamPla, &sceneMaiMen, &sceneControls,
                             &sceneInGam, &sceneChoWor, &sceneVictory,
                             &sceneErrFail)) {
        currentSceneID = SCENE_ERR_FAIL;
    }
    // StartUp Szene zeichnen, damit der Bildschrim nicht leer beleibt;
    if (ERR_OK != Scene_Draw(&sceneStartUp)) {
        currentSceneID = SCENE_ERR_FAIL;
    }
    // GUI Listen and die entsprechenden Variablen, von der Struktur übergeben
    sceneNamePlayer = sceneNamPla.sceneNamPlaGUI;
    sceneMainMenu = sceneMaiMen.sceneMainMenuGUI;
    sceneInGame = sceneInGam.sceneInGameGUI;
    sceneChooseWorld = sceneChoWor.sceneChooseWorldGUI;

    // Start Bedigung der Szene definieren
    sceneCurrent = &sceneStartUp;

    // Globale Variablen setzen
    currentSceneID = SCENE_STARTUP;
    gameloop = 1;

    // Game Loop, in dieser while-Schleife läuft das Programm
    while (gameloop == 1) {
        // Die switch Abfrage reagiert auf die aktive SceneID
        switch (currentSceneID) {
        case SCENE_STARTUP:
            sceneCurrent = &sceneStartUp;
            // StartUp Delay, damit man den Schriftzug sieht
            startUpLoop++;
            if (startUpLoop >= 200) {
                currentSceneID = SCENE_NAMEPLAYER;
            }
            break;
        case SCENE_NAMEPLAYER:
            sceneCurrent = &sceneNamePlayer;
            break;
        case SCENE_MAINMENU:
            sceneCurrent = &sceneMainMenu;
            break;
        case SCENE_CONTROLS:
            sceneCurrent = &sceneControls;
            break;
        case SCENE_PREPAREGAME:
            // Spielernamen übergeben
            playerA.name = sceneNamPla.tiNamePlayerA->elementData.textInput.text;
            playerB.name = sceneNamPla.tiNamePlayerB->elementData.textInput.text;

            // Spielernamen kontrollieren, wird angepasst falls
            // unberührt oder doppelten Namen
            if (strcmp(playerA.name, "Entry") == 0) {
                strcpy(playerA.name, "Player A");
            }
            if (strcmp(playerB.name, "Entry") == 0) {
                strcpy(playerB.name, "Player B");
            }
            if (strcmp(playerA.name, playerB.name) == 0) {
                strcat(playerB.name, "2");
            }

            // Spieler vorbereiten
            playerA.healthpoints = 100;
            playerB.healthpoints = 100;
            playerA.step = PLAYER_STEP_START;
            playerB.step = PLAYER_STEP_START;

            // InGame Szene mit den Spielernamen aktualisieren
            if (ERR_OK != sceneInGame_Update(&sceneInGam, playerA, playerB, inputEvent)) {
                currentSceneID = SCENE_ERR_FAIL;
            }
            sceneInGame = sceneInGam.sceneInGameGUI;

            // Fünf Startpositionen erstellen aber nur die äusseren zwei
            // verwenden. So sind die Panzer weit voneinander entfernt.
            SDL_Rect aabb = {.h = 30, .w = 40};
            SDL_Point locations[3];
            World_CreateStartLocations(aabb, 3, locations);

            if (ERR_OK != Tank_Create(&tankPlayerA, &playerA, locations[0].x, locations[0].y) ||
                ERR_OK != Tank_Create(&tankPlayerB, &playerB, locations[2].x, locations[2].y)) {
                currentSceneID = SCENE_ERR_FAIL;
            }

            currentSceneID = SCENE_INGAME;
            break;
        case SCENE_CHOOSEWORLD:
            sceneCurrent = &sceneChooseWorld;
            break;
        case SCENE_INGAME:
            // Aktiven Spieler umschalten
            if (PLAYER_STEP_DONE == inputEvent.currentPlayer->step) {
                inputEvent.currentPlayer = (inputEvent.currentPlayer == &playerA) ? &playerB : &playerA;
                inputEvent.currentPlayer->step = PLAYER_STEP_START;
                if (ERR_OK != sceneInGame_Update(&sceneInGam, playerA, playerB, inputEvent)) {
                    currentSceneID = SCENE_ERR_FAIL;
                }
                sceneInGame = sceneInGam.sceneInGameGUI;
            }
            // Lebenspunkte Abfrage, falls ein Spieler Lebenspunkte
            // gleich 0 oder unter 0 hat, wird das Spiel beendet
            // und der Sieger wird bekannt gegeben.
            if (playerA.healthpoints <= 0) {
                currentSceneID = SCENE_VICTORY;
                if (ERR_OK != sceneVictory_Update(&sceneVictory, &playerB) ||
                    ERR_OK != EntityHandler_RemoveAllEntities()) {
                    currentSceneID = SCENE_ERR_FAIL;
                }
                World_Quit();
            } else if (playerB.healthpoints <= 0) {
                currentSceneID = SCENE_VICTORY;
                if (ERR_OK != sceneVictory_Update(&sceneVictory, &playerA) ||
                    ERR_OK != EntityHandler_RemoveAllEntities()) {
                    currentSceneID = SCENE_ERR_FAIL;
                }
                World_Quit();
            }
            sceneCurrent = &sceneInGame;
            break;
        case SCENE_VICTORY:
            sceneCurrent = &sceneVictory;
            break;
        case SCENE_ERR_FAIL:
            sceneCurrent = &sceneErrFail;
            break;
        default:
            sceneCurrent = &sceneMainMenu;
            break;
        }
        // Aktuelle Szene aktualisieren, mit allen enthaltenen Elementen
        if (ERR_OK != Scene_Update(&event, &inputEvent, sceneCurrent)) {
            currentSceneID = SCENE_ERR_FAIL;
        }
    }

    // Entfernen aller benutzen Resourcen
    EntityHandler_RemoveAllEntities();
    World_Quit();
    SDLW_Quit();
    return 0;
}