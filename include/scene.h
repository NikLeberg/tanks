/**
 * @file scene.h
 * @author Weber Jan (webej14@bfh.ch)
 * @brief Scene auswahl und Koordination benötigter Module
 * @version 0.1
 * @date 2021-03-19
 * 
 * @copyright Copyright (c) 2021 Weber Jan
 * 
 */

#pragma once


/*
 * Includes
 * 
 */

#include "input.h"
#include "gui.h"
#include "error.h"
#include "sdlWrapper.h"


/*
 * Typdeklarationen
 * 
 */

/**
 * @brief ID Namen aller Szenen des Programmes
 * 
 * Mit Hilfe dieser ID-Namen, kann man zu jeder Zeit auslesen welche Szene
 * aktiv ist und die nächste Szene vorbereiten.
 */
typedef enum {
	SCENE_STARTUP, SCENE_NAMEPLAYER, SCENE_MAINMENU,
	SCENE_CONTROLS, SCENE_CHOOSEWORLD, SCENE_PREPAREGAME,
	SCENE_INGAME, SCENE_VICTORY, SCENE_ERR_FAIL
} SceneID;

/**
 * @brief ID für Button mit ähnlicher CallBackFunc
 * 
 */
typedef enum {
	BTTN_ID_BACKTOMAINMENU, BTTN_ID_BACKTOMAINMENU_CLOSE_WORLD,
	BTTN_ID_WORLD_EVERGREEN, BTTN_ID_WORLD_AQUADUCT
} ButtonID;

/**
 * @brief Verlinkung aller GUI Elemente der Szene NamePlayer
 * 
 * Zum auslesen oder bearbeiten einzelner GUI Elemente dieser Szene
 * erhalten sie Zeiger darauf. Zusätzlich wird die komplette Liste
 * aller Elemente übergeben, welche dann gezeichnet werden können.
 */
typedef struct {
	guiElement_t *bttnPlayerA;		//!< Schriftzug "Player A"
	guiElement_t *tiNamePlayerA;	//!< Namen Text Input Spieler A
	guiElement_t *bttnPlayerB;		//!< Schriftzug "Player B"
	guiElement_t *tiNamePlayerB;	//!< Namen Text Input Spieler B
	guiElement_t *bttnNext;			//!< schaltet zur nächsten Szene
	gui_t sceneNamPlaGUI;			//!< Liste mit sämtlichen GUI Elemente dieser Szene
} sceneNamePlayer_t;

/**
 * @brief Verlinkung aller GUI Elemente der Szene MainMenu
 * 
 * Zum auslesen oder bearbeiten einzelner GUI Elemente dieser Szene
 * erhalten sie Zeiger darauf. Zusätzlich wird die komplette Liste
 * aller Elemente übergeben, welche dann gezeichnet werden können.
 */
typedef struct {
	guiElement_t *bttnNewGame;		//!< startet ein neues Spiel
	guiElement_t *bttnControls;		//!< öffnet die Ansicht der im Spiel Steuerung
	guiElement_t *bttnChangeName;	//!< öffnet die Szene NamePlayer erneut
	guiElement_t *bttnQuit;			//!< Schliesst das Programm
	gui_t sceneMainMenuGUI;			//!< Liste mit sämtlichen GUI Elemente dieser Szene
} sceneMainMenu_t;

/**
 * @brief Verlinkung aller GUI Elemente der Szene InGame
 * 
 * Zum auslesen oder bearbeiten einzelner GUI Elemente dieser Szene
 * erhalten sie Zeiger darauf. Zusätzlich wird die komplette Liste
 * aller Elemente übergeben, welche dann gezeichnet werden können.
 */
typedef struct {
	guiElement_t *bttnPlayerNameA;	//!< Schriftzug mit dem Namen des Spielers A
	guiElement_t *bttnPlayerNameB;	//!< Schriftzug mit dem Namen des Spielers B
	guiElement_t *bttnBackToMaiMen;	//!< öffnet die Szene MainMenu
	guiElement_t *bttnHPBarBGA;		//!< Hintergund des Lebensbalken, zeigt maximale Lebenspunkte an
	guiElement_t *bttnHPBarBGB;		//!< Hintergund des Lebensbalken, zeigt maximale Lebenspunkte an
	guiElement_t *bttnHPBarCoverA;	//!< Abdeckung des Lebensbalken, zeigt die aktuellen Lebenspunkte des Spielers A an
	guiElement_t *bttnHPBarCoverB;	//!< Abdeckung des Lebensbalken, zeigt die aktuellen Lebenspunkte des Spielers B an
	gui_t sceneInGameGUI;			//!< Liste mit sämtlichen GUI Elemente dieser Szene
} sceneInGame_t;

/**
 * @brief Verlinkung aller GUI Elemente der Szene ChooseWorld
 * 
 * Zum auslesen oder bearbeiten einzelner GUI Elemente dieser Szene
 * erhalten sie Zeiger darauf. Zusätzlich wird die komplette Liste
 * aller Elemente übergeben, welche dann gezeichnet werden können.
 */
typedef struct {
	guiElement_t *bttnChooseWorld;	//!< Schriftzug "Choose a World"
	guiElement_t *bttnEvergreen;	//!< öffnet die Welt Evergreen
	guiElement_t *bttnAquaduct;		//!< öffnet die welt Aquaduct
	gui_t sceneChooseWorldGUI;		//!< Liste mit sämtlichen GUI Elemente dieser Szene
} sceneChooseWorld_t;


/*
 * Variablendeklarationen
 * 
 */

#define COLORBACKGROUND (SDL_Color){34,139,34,255}		//!< Hintergrund Farbe
#define COLORBGOFFSET (SDL_Color){24,100,24,255}		//!< Farbe die sich vom Hintergund absetzt
#define COLORTEXT (SDL_Color){255,255,255,255}			//!< Text Farbe
#define COLORINGAMEBTTN (SDL_Color){050,200,050,255}	//!< Farbe für Buttons im Spiel
#define COLORACTIVPLAYER (SDL_Color){255, 165, 0, 255}	//!< Farbe für den Button des aktiven Spielers
#define COLORHPBARBG (SDL_Color){202, 38, 38, 255}		//!< Hintergund Farbe des Lebensbalken
#define COLORHPBARCOVER (SDL_Color){34, 139, 34, 255}	//!< Abdeckfarbe des Lebensbalken

extern int gameloop;			//!< Globale Variable, 1=Programm läuft, 0=Programm wird beendet 
extern SceneID currentSceneID;	//!< Globale Variable, enthält die aktuell aktive Szene ID

/*
 * Öffentliche Funktionen
 * 
 */

/**
 * @brief Initialisierung aller Scenen
 * 
 * @param[in] playerA Struktur
 * @param[in] playerB Struktur
 * @param[in] input Benutzereingaben
 * @param[out] sceneStartUp Speicher Ort der GUI Elemente Liste
 * @param[out] sceneNamePlayer Speicher Ort der GUI Elemente Liste
 * @param[out] sceneMainMenu Speicher Ort der GUI Elemente Liste
 * @param[out] sceneControls Speicher Ort der GUI Elemente Liste
 * @param[out] sceneInGame Speicher Ort der GUI Elemente Liste
 * @param[out] sceneChooseWorld Speicher Ort der GUI Elemente Liste
 * @param[out] sceneVictory Speicher Ort der GUI Elemente Liste
 * @param[out] sceneErrFail Speicher Ort der GUI Elemente Liste
 * @return int 0 oder Fehlercode 
 */
int Scene_Init(player_t playerA, player_t playerB, inputEvent_t input,
				gui_t *sceneStartUp, sceneNamePlayer_t *sceneNamePlayer,
				sceneMainMenu_t *sceneMainMenu, gui_t *sceneControls,
				sceneInGame_t *sceneInGame, sceneChooseWorld_t *sceneChooseWorld,
				gui_t *sceneVictory, gui_t *sceneErrFail);

/**
 * @brief Szene aktualisieren mit Benutzer Eingaben
 * 
 * Aktualisiert sämtliche Elemente der aktuellen Szene. Dafür werden
 * entsprechende Folgemodule angesprochen.
 * 
 * @param[in] event Benutzereingaben 
 * @param[out] inputEvent Speicherort der konvertierten Benutzereingaben
 * @param[in] scene Aktive Szene
 * @return int 0 oder Fehlercode
 */
int Scene_Update(SDL_Event *event, inputEvent_t *inputEvent, gui_t *scene);

/**
 * @brief Im Spiel Szene zeichnen
 * 
 * Zeichnet alle Elemente der aktuellen Scene auf dem Bildschirm.
 * Dafür werden die Module: World, EntityHandler und GUI angesprochen,
 * welche dann ihre Elemente dem SDL Wrapper übergeben und danach auf
 * dem Bildschirm angezeigt werden.
 * 
 * @param[in] scene zu zeichnende Szene
 * @return int 0 oder Fehlercode
 */
int Scene_DrawGame(gui_t *scene);

/**
 * @brief Scene auf dem Bildschirm zeichnen
 * 
 * Zeichnet alle Elemente der aktuellen Scene auf dem Bildschirm.
 * Dafür wird das Folgemodul GUI angesprochen, welche dann ihre
 * Elemente dem SDL Wrapper übergeben und danach auf dem Bildschirm
 * angezeigt werden.
 * 
 * @param scene zu zeichnende Szene
 * @return int 0 oder Fehlercode
 */
int Scene_Draw(gui_t *scene);

/**
 * @brief Gibt eine Button Vorlage zurück
 * 
 * @return guiElement_t buttonprototyp
 */
guiElement_t Scene_ButtonPrototyp();

/**
 * @brief Gibt TextInput Vorlage zurück
 * 
 * @return guiElement_t textinputprototyp
 */
guiElement_t Scene_TextInputPrototyp();

/**
 * @brief Initialisierung der Szene StartUP
 * 
 * @param[out] sceneStartUp Speicherort der GUI Elemente Liste 
 * @return int 0 oder Fehlercode
 */
int sceneStartup_Init(gui_t *sceneStartUp);

/**
 * @brief Initialisierung der Szene NamePlayer
 * 
 * @param[out] sceneNamePlayer Struktur mit Zeiger auf alle Elemente und die gesamte GUI Elemente Liste 
 * @return int 0 oder Fehlercode
 */
int sceneNamePlayer_Init(sceneNamePlayer_t *sceneNamePlayer);

/**
 * @brief Initialisierung der Szene MainMenu
 * 
 * @param[out] sceneMainMenu Struktur mit Zeiger auf alle Elemente und die gesamte GUI Elemente Liste
 * @return int 0 oder Fehlercode
 */
int sceneMainMenu_Init(sceneMainMenu_t *sceneMainMenu);

/**
 * @brief Initialisierung der Szene Controls
 * 
 * @param[out] sceneControls Speicherort der GUI Elemente Liste
 * @return int 0 oder Fehlercode
 */
int sceneControls_Init(gui_t *sceneControls);

/**
 * @brief Initialisierung der Szene InGame
 * 
 * @param[out] sceneInGame Struktur mit Zeiger auf alle Elemente und die gesamte GUI Elemente Liste
 * @param[in] playerA Struktur 
 * @param[in] playerB Struktur
 * @param[in] input konvertierte Benutzereingabe
 * @return int 0 oder Fehlercode
 */
int sceneInGame_Init(sceneInGame_t *sceneInGame, player_t playerA, player_t playerB, inputEvent_t input);

/**
 * @brief Update der Szene InGame
 * 
 * @param[out] sceneInGame Struktur mit Zeiger auf alle Elemente und die gesamte GUI Elemente Liste
 * @param[in] playerA Struktur 
 * @param[in] playerB Struktur
 * @param[in] input konvertierte Benutzereingabe
 * @return int 0 oder Fehlercode
 */
int sceneInGame_Update(sceneInGame_t *sceneInGame, player_t playerA, player_t playerB, inputEvent_t input);

/**
 * @brief Initialisierung der Szene ChooseWorld
 * 
 * @param[out] sceneChooseWorld Struktur mit Zeiger auf alle Elemente und die gesamte GUI Elemente Liste
 * @return int 0 oder Fehlercode
 */
int sceneChooseWorld_Init(sceneChooseWorld_t *sceneChooseWorld);

/**
 * @brief Initialisierung der Szene Victory
 * 
 * @param[out] sceneVictory Speicherort der GUI Elemente Liste 
 * @return int 0 oder Fehlercode
 */
int sceneVictory_Init(gui_t *sceneVictory);

/**
 * @brief Update der Szene Victory
 * 
 * @param[out] sceneVictory Speicherort der GUI Elemente Liste 
 * @param[in] winner Siegreiches Spieler Struct 
 * @return int 0 oder Fehlercode
 */
int sceneVictory_Update(gui_t *sceneVictory, player_t *winner);

/**
 * @brief Initialisierung der Szene ErrFail
 * 
 * @param[out] sceneErrFail Speicherort der GUI Elemente Liste
 * @return int 0 oder Fehlercode 
 */
int sceneErrFail_Init(gui_t *sceneErrFail);

/**
 * @brief Button Call Back Funktion Quit
 * 
 * Schliesst das Programm.
 */
void Bttn_CallBack_Quit();

/**
 * @brief Button Call Back Funktion Next
 * 
 * Öffnet die nächste Szene.
 */
void Bttn_CallBack_Next();

/**
 * @brief Button Call Back Funktion New Game
 * 
 * Startet ein neues Spiel.
 */
void Bttn_CallBack_NewGame();

/**
 * @brief Button Call Back Funktion Controls
 * 
 * Öffnet die Szene mit der im Spiel Steuerung.
 */
void Bttn_CallBack_Controls();

/**
 * @brief Button Call Back Funktion Change Name
 * 
 * Öffnet die Szene mit der Spieler Namen Eingabe.
 */
void Bttn_CallBack_ChangeName();

/**
 * @brief Button Call Back Funktion BackToMainMenu
 * 
 * @param id Button ID
 * 
 * Falls nötig schliesst die Welt und löscht alle
 * Entities, öffnet anschliessend die Szene MainMenu.
 */
void Bttn_CallBack_BackToMainMenu(int id);

/**
 * @brief Button Call Back Funktion LoadWorld
 * 
 * @param id Button ID
 * 
 * Lädt abhängig der Bttn ID die entsprechende welt.
 */
void Bttn_CallBack_LoadWorld(int id);
