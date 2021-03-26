/**
 * @file gui.h
 * @author Hinnaoui Angelo (hinna2@bfh.ch)
 * @brief GUI Header
 * @version 0.01
 * @date 2021-03-19
 *
 * @copyright Copyright (c) 2021 Hinnaoui Angelo
 *
 */

#pragma once


/*
 * Includes
 *
 */
#include "SDL_ttf.h"
#include "sdlWrapper.h"
#include "list.h"
#include "button.h"

/**
 * @brief Union von möglichen GUI Elementen Bausteine
 *
 */

typedef union {
    button_t button;    //!< Speicher für Taster
    // Textfeld
    // ...
} elementUnion_t;

/**
 * @brief guiElement mit definiertem Typ
 *
 */

typedef struct {
    int type;                      //!< Element Typen Bezeichnung (zb: 0 = Button, 1 = Text, ...)
    elementUnion_t elementData;    //!< Zugriff auf die Union von Elementen
} guiElement_t;

/**
 * @brief Auflistung von GUI Elementen
 *
 */

typedef struct {
    list_t elemente; //!< Die Liste der Elemente
} gui_t;

/**
 * @brief GUI_Init
 * @param gui Neues GUI
 * @return 0 oder error code
 */

int GUI_Init(gui_t *gui);

/**
 * @brief GUI_Update
 *
 * Jegliche Ereignisse/Interaktionen mit dem UI in der scene werdem dem GUI_Update übergeben,
 * welche die erhaltenen Informationen mit der dazugehörigen Aktion vergleicht und ausführt.
 * die verarbeiteten Informationen werde darauf übr das Button Modul and den SDL_Wrapper übergeben.
 *
 * @param inputEvents Aktualisiertung des GUI, je nach Ereignisse
 * @param gui gewählte GUI Elemente
 * @return 0 oder error code
 */

int GUI_Update(void *inputEvents, gui_t *gui);

/**
 * @brief GUI_Draw
 *
 * Durch die vom GUI enthaltenen Ereignisse/Interaktionen, durchsucht die Funktion GUI_Draw die Liste
 * von Gui Elementen, nach dem Ereigniss entsprechende Element das Grafisch aktualiesiert werden muss.
 * über das Button Modul an den SDL_Wrapper übergeben.
 *
 * @param gui auswahl der zu zeichnenden GUI Elemente
 * @return 0 oder error code
 */

int GUI_Draw(gui_t *gui);

/**
 * @brief GUI_AddElement
 *
 * Wird beim aktualisieren der Sceene oder  dem UI weitere Elemente hinzugefügt, zb. einen weitern Spieler,
 * fügt die Funktion GUI_AddElement die noch fehlenden Elemente der Liste der GUI Elemente hinzu.
 *
 * @param element Neues hinzuzufügendes GUI Element
 * @param gui Zugehörige GUI Liste
 * @return 0 oder error code
 */

int GUI_AddElement(guiElement_t *element, gui_t *gui);

/**
 * @brief GUI_RemoveElement
 *
 * Identisch zur Funktion GUI_AddElement, macht die Funktion GUI_RemoveElement genau das Gegenteil.
 * Nicht benötigte Elemente werden aus der Liste der GUI Elemente entfernt.
 *
 * @param element Zu entfernendes GUI Element
 * @param gui Zugehörige GUI Liste
 * @return 0 oder error code
 */

int GUI_RemoveElement(guiElement_t *element, gui_t *gui);
