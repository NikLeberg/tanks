/**
 * @file scenebttncallbackfunc.c
 * @author Weber Jan (webej14@bfh.ch)
 * @brief Callbacks für verschiedenen Szene-Buttons
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
#include "entityHandler.h"


/*
 * Typdeklarationen
 * 
 */

/* ... */


/*
 * Variablendeklarationen
 * 
 */

/* ... */


/*
 * Private Funktionsprototypen
 * 
 */

/* ... */


/*
 * Implementation Öffentlicher Funktionen
 * 
 */

void Bttn_CallBack_Quit() {
    gameloop = 0;
}

void Bttn_CallBack_Next() {
    // Die switch Struktur wählt jeweils die nächste Szene aus
    switch (currentSceneID) {
    case SCENE_STARTUP:
        currentSceneID = SCENE_NAMEPLAYER;
        break;
    case SCENE_NAMEPLAYER:
        currentSceneID = SCENE_MAINMENU;
        break;
    default:
        currentSceneID = SCENE_MAINMENU;
        break;
    }
}

void Bttn_CallBack_NewGame() {
    currentSceneID = SCENE_CHOOSEWORLD;
}

void Bttn_CallBack_Controls() {
    currentSceneID = SCENE_CONTROLS;
}

void Bttn_CallBack_ChangeName() {
    currentSceneID = SCENE_NAMEPLAYER;
}

void Bttn_CallBack_BackToMainMenu(int id) {
    currentSceneID = SCENE_MAINMENU;
    if (id == BTTN_ID_BACKTOMAINMENU_CLOSE_WORLD) {
        if (ERR_OK != EntityHandler_RemoveAllEntities()) {
            currentSceneID = SCENE_ERR_FAIL;
        }
        World_Quit();
    }
}

void Bttn_CallBack_LoadWorld(int id) {
    currentSceneID = SCENE_PREPAREGAME;
    if (ERR_OK != World_Init()) {
        currentSceneID = SCENE_ERR_FAIL;
    }
    if (id == BTTN_ID_WORLD_EVERGREEN) {
        if (ERR_OK != World_Load("evergreen")) {
            currentSceneID = SCENE_ERR_FAIL;
        }
    } else if (id == BTTN_ID_WORLD_AQUADUCT) {
        if (ERR_OK != World_Load("aquaduct")) {
            currentSceneID = SCENE_ERR_FAIL;
        }
    }
}


/*
 * Implementation Privater Funktionen
 * 
 */

/* ... */
