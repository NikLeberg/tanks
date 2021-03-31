/**
 * @file entity.h
 * @author Leuenberger Niklaus (leuen4@bfh.ch)
 * @brief Datenstruktur einer Entität
 * @version 0.1
 * @date 2021-03-19
 * 
 * @copyright Copyright (c) 2021 Leuenberger Niklaus
 *
 * Eine Entität \ref entity_t ist ein Spielobjekt mit Physik. Diese Entität kann
 * sich als ganzes bewegen und obliegt der Spielphysik mittels einer einzelnen
 * Kollisionsbox \ref entityPhysics_t.aabb.
 *      z.B. ein gesamter Panzer.
 * Jede Entität wiederum ist aus mindestens einem Einzelteil \ref entityPart_t
 * aufgebaut. Die maximale Anzahl an Teilen ist unbeschränkt. Diese bestehen
 * Hauptsächlich aus einer Textur und Angaben wie sie relativ zur gesamten
 * Entität platziert werden sollen. Vorteil dadurch ist, dass sich diese Teile
 * einzeln Animieren lassen. Oder aber, dass ein Panzer verschiedene Waffentypen
 * haben kann.
 *      z.B. Panzerrohr, Fahrkabine, Chassis und Raupen
 * Erweiterte Dokumentation ist in \ref Entitaet zu finden.
 * 
 */

#pragma once


/*
 * Includes
 * 
 */

#include "sdlWrapper.h"
#include "scene.h"
#include "list.h"


/*
 * Typdeklarationen
 * 
 */

/**
 * @brief Status der Entität
 * Werden per \ref EntityHandler_AddEntity() Entitäten hinzugefügt, so werden diese erst im nächsten
 * Update / Draw - Zyklus aktiv geschaltet.
 */
typedef enum {
    ENTITY_STATE_CREATED = 0, //!< Erstellt aber noch nicht aktiv und wird daher noch nicht gezeichnet.
    ENTITY_STATE_ACTIVE       //!< Aktiv und wird gezeichnet.
} entityState_t;

/**
 * @brief 2D-Vektor
 * 
 * @warning Keine kartesischen Koordinaten, Ursprung ist oben links!
 * 
 */
typedef struct {
    float x; //!< X Komponente
    float y; //!< Y Komponente
} entityVector_t;

/**
 * @brief Physikalische Daten einer Entität
 *
 * @note Es herrscht eine konstante Beschleunigung nach unten (Erdbeschleunigung).
 * @warning Nur das physics-Modul darf Werte dieser Struktur verändern.
 * Für Manipulationen stehen diverse Physics_Set*() Funktionen zur verfügung.
 * 
 */
typedef struct {
    entityVector_t position;     //!< Position des Zentrums der Entität [m]
    entityVector_t velocity;     //!< Geschwindigkeit der Positionsänderung [m/s]
    float rotation;              //!< Rotation der Entität um das Zentrum, positiv = Gegen-Uhrzeigersinn [rad]
    SDL_Rect aabb;               //!< Kollisionsbox / Axis-Aligned Bounding Box
} entityPhysics_t;

/**
 * @brief Einzelteile einer Entität
 * 
 */
typedef struct {
    sprite_t sprite;                 //!< alle Felder ausser textur werden automatisch von \ref EntityHandler_Update() aktualisiert
    entityVector_t relativePosition; //!< Position relativ zum Mittelpunkt der Entität
    float relativeRotation;          //!< Rotation um den Mittelpunkt relativ zur Rotation der Entität, positiv = Gegen-Uhrzeigersinn [rad]
    const char *name;                //!< Name des Einzelteils
} entityPart_t;

struct entity_s;

/**
 * @brief Callbacks für interaktive Entitäten
 *
 * Entitäten können durch Nutzung folgender Callbacks interaktiv gestaltet werden.
 * Die Reihenfolge der Funktionsaufrufe ist folgende:
 * - \ref entityCallbacks_t.onUpdate aller Entitäten
 * - Dann berechnet der EntityHandler die Positionen und Rotationen neu.
 * - Danach wird die Physik aller Entitäten verarbeitet. Beim Auftreten einer Kollision
 *   wird durch das physics-Modul der Callback \ref entityCallbacks_t.onCollision aufgerufen.
 * - \ref entityCallbacks_t.onDraw aller Entitäten
 */
typedef struct {
    /**
     * @brief Update
     *
     * Jeder Zyklus kann die Entität hiermit auf Eingabeevents reagieren.
     * @note Dieser Callback darf NULL sein.
     *
     * @param self Pointer auf die Entität dessen Callback gerade aufgerufen wird
     * @param inputEvents Eingabeevents für die Entität (bsp.: nach links oder nach rechts)
     * 
     * @return Fehlercode gemäss \ref error.h
     */
    int (*onUpdate)(struct entity_s *self, inputEvent_t *inputEvents);

    /**
     * @brief Kollision mit der Welt oder anderen Entitäten
     * 
     * @note Ist dieser Callback NULL, so wird die Kollision ignoriert.

     * @param self Pointer auf die Entität dessen Callback gerade aufgerufen wird
     * 
     * @return Fehlercode gemäss \ref error.h
     */
    int (*onCollision)(struct entity_s *self);

    /**
     * @brief Zeichnen
     *
     * Jeder Zyklus wird die Entität hiermit aufgefordert sich selbst zu zeichnen.
     * @note Ist dieser Callback NULL, so wird die standart Zeichnen Funktion des
     * EntityHandlers verwendet.

     * @param self Pointer auf die Entität dessen Callback gerade aufgerufen wird
     * 
     * @return Fehlercode gemäss \ref error.h
     */
    int (*onDraw)(struct entity_s *self);
} entityCallbacks_t;

/**
 * @brief Datenstruktur einer Entität
 * 
 */
typedef struct entity_s {
    entityState_t state;         //!< Status der Entität, darf nur von EntityHandler verändert werden.
    entityPhysics_t physics;     //!< Physikdaten der Entität, wird vom physics-Modul verwaltet.
    list_t *parts;               //!< Liste an Einzelteilen aus deren die Entität besteht.
    entityCallbacks_t callbacks; //!< Funktionscallbacks für interaktive Entitäten

    const char *owner;           //!< Eigentümer der Entität, Spielername
    const char *name;            //!< Name der Entität
    void *data;                  //!< Pointer auf optionale Daten der Entität, zur freien Benutzung.
} entity_t;


/*
 * Variablendeklarationen
 * 
 */

/* ... */


/*
 * Öffentliche Funktionen
 * 
 */

/* ... */
