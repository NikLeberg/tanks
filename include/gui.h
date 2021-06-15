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
#include "guiElements/button.h"
#include "guiElements/text.h"


/**
 * @brief Enum von möglichen GUI Elementen Bausteine
 *
 */

typedef enum {
    TYPE_BUTTON, //!< enum für Taste
    TYPE_TEXT    //!< enum für Text
} guiElemntType_t;

/**
 * @brief Union von möglichen GUI Elementen Bausteine
 *
 */

typedef union {
    button_t button;  //!< Speicher für Taster
    text_t textInput; //!< Speicher für Text
} elementUnion_t;

/**
 * @brief guiElement mit definiertem Typ
 *
 */

typedef struct {
    guiElemntType_t type;       //!< Element Typen Bezeichnung (0 = Button, 1 = Text, ...)
    elementUnion_t elementData; //!< Zugriff auf die Union von Elementen
} guiElement_t;

/**
 * @brief Auflistung von GUI Elementen
 *
 */

typedef struct {
    list_t element; //!< Die Liste der Elemente
} gui_t;


/*
 * Öffentliche Funktionen
 *
 */

/**
 * @brief GUI_Init
 *
 * Initialisiert die GUI Liste.
 *
 * @param[in] gui Neues GUI
 * @return 0 oder error code
 */

int GUI_Init(gui_t *gui);

/**
 * @brief GUI_Update
 *
 * Jegliche Ereignisse/Interaktionen mit dem UI in der scene werden dem GUI_Update übergeben,
 * welche die erhaltenen Informationen mit der dazugehörigen Aktion vergleicht und ausführt.
 * die verarbeiteten Informationen werden darauf über das Button Modul and den SDL_Wrapper übergeben.
 *
 * @param[in] inputEvents Aktualisierung des GUI, je nach Ereignisse
 * @param[in] gui gewählte GUI Elemente
 * @return 0 oder error code
 */

int GUI_Update(inputEvent_t *inputEvents, gui_t *gui);

/**
 * @brief GUI_Draw
 *
 * Durch die vom GUI enthaltenen Ereignisse/Interaktionen, durchsucht die Funktion GUI_Draw die Liste
 * von Gui Elementen, nach dem Ereignis entsprechende Element das Grafisch aktualisiert werden muss.
 * über das Button Modul an den SDL_Wrapper übergeben.
 *
 * @param[in] gui Auswahl der zu zeichnenden GUI Elemente
 * @return 0 oder error code
 */

int GUI_Draw(gui_t *gui);

/**
 * @brief GUI_AddElement
 *
 * Wird beim aktualisieren der Scene oder dem UI weitere Elemente hinzugefügt, z.B. einen weiteren Spieler,
 * fügt die Funktion GUI_AddElement die noch fehlenden Elemente der Liste der GUI Elemente hinzu.
 *
 * @param[in] element Neues hinzuzufügendes GUI Element
 * @param[in] gui Zugehörige GUI Liste
 * @return 0 oder error code
 */

int GUI_AddElement(guiElement_t *element, gui_t *gui);

/**
 * @brief GUI_RemoveElement
 *
 * Identisch zur Funktion GUI_AddElement, macht die Funktion GUI_RemoveElement genau das Gegenteil.
 * Nicht benötigte Elemente werden aus der Liste der GUI Elemente entfernt.
 *
 * @param[in] element Zu entfernendes GUI Element
 * @param[in] gui Zugehörige GUI Liste
 * @return 0 oder error code
 */

int GUI_RemoveElement(guiElement_t *element, gui_t *gui);
