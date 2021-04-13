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

struct entity_s; // Vorwärtsdeklaration einer Entitätsstruktur

/**
 * @brief Status der Entität
 *
 * Werden per \ref EntityHandler_AddEntity() Entitäten hinzugefügt, so werden
 * diese erst im nächsten Update / Draw - Zyklus aktiv geschaltet.
 */
typedef enum {
    ENTITY_STATE_CREATED = 0, //!< Erstellt aber noch nicht aktiv
    ENTITY_STATE_ACTIVE       //!< Aktiv und wird gezeichnet
} entityState_t;

/**
 * @brief Physikalische Daten einer Entität
 *
 * @note Es herrscht eine die normale Erdeschleunigung nach unten.
 * @warning Nur das physics-Modul darf Werte dieser Struktur verändern. Für
 * Manipulationen stehen diverse Physics_Set*() Funktionen zur verfügung.
 */
typedef struct {
    /**
     * @brief Position der Entität
     *
     * Angaben sind absolut und zeigen auf das Zentrum der Entität
     * Die Einheit ist [pixel].
     */
    SDL_FPoint position;

    /**
     * @brief Geschwindigkeit der Entität
     * 
     * Angaben sind im Koordinatensystem der Welt resp. horizontal und vertikal.
     * Die Einheit ist [pixel / s].
     * @note Es herrscht eine Erdbeschleunigung nach unten, Y-Koordinate wird
     * mit jedem Update aktualisiert.
     */
    SDL_FPoint velocity;

    /**
     * @brief Rotation der Entität
     * 
     * Die Rotation erfolgt um das Zentrum der Entität.
     * Die Einheit ist [°]. Positiv = im Uhrzeigersinn
     */
    double rotation;

    /**
     * @brief Kollisionsbox der Entität
     * 
     * Axis-Aligned Bounding Box. Sie wird nicht rotiert und bleibt immer starr
     * in der Achsenausrichtung der Welt. Anhand dieser Box wird die Entität
     * physikalisch mit der Welt interagieren. Die x/y-Koordinaten werden
     * automatisch an \ref position angeglichen.
     * @note Angaben dürfen 0 sein, in diesem Falle wird eine Box berechnet in
     * der alle Einzelteile \ref entityPart_t platz haben.
     */
    SDL_Rect aabb;
} entityPhysics_t;

/**
 * @brief Flags möglicher Kollisionen einer Entität
 * 
 */
typedef enum {
    ENTITY_COLLISION_WORLD = 1,          //!< Kollision mit der Welt
    ENTITY_COLLISION_BORDER_LEFT = 2,    //!< Kollision mit linkem Spielrand
    ENTITY_COLLISION_BORDER_RIGHT = 4,   //!< Kollision mit rechtem Spielrand
    ENTITY_COLLISION_BORDER_TOP = 8,     //!< Kollision mit oberem Spielrand
    ENTITY_COLLISION_BORDER_BOTTOM = 16, //!< Kollision mit unterem Spielrand
    ENTITY_COLLISION_ENTITY = 32         //!< Kollision mit einer Entität
} entityCollisionFlags_t;

/**
 * @brief Informationen einer Kollision
 * 
 * Eine Struktur mit den folgenden Informationen wird einer Entität bei einer
 * Kollision übergeben.
 */
typedef struct {
    /**
     * @brief Kollisionsflags
     * 
     * Flag mit gesetzten Bits für einzelne Kollisionen gemäss
     * \ref entityCollisionFlags_t welche stattgefunden haben.
     */
    int flags;

    /**
     * @brief Vektor der Kollisionsnormale
     * 
     * Die Kollisionsnormale ist der kürzeste Vektor entlang der die Entität
     * verschoben werden müsste damit keine Kollision entsteht.
     */
    SDL_FPoint normale;

    /**
     * @brief Kollisionspartner
     * 
     * Die Entität die an der Kollision mitbeteiligt ist.
     * @note Nur gültig wenn in \ref entityCollision_t.flags
     * ENTITY_COLLISION_ENTITY aktiviert ist.
     */
    const struct entity_s *const partner;
} entityCollision_t;

/**
 * @brief Einzelteile einer Entität
 * 
 */
typedef struct {
    const char *name;    //!< Name des Einzelteils
    sprite_t sprite;     //!< das reale Sprite mit Textur und relativen Angaben
    sprite_t tempSprite; //!< temporäres Sprite mit berechneten Absolutwerten
} entityPart_t;

/**
 * @brief Callbacks für interaktive Entitäten
 *
 * Entitäten können durch Nutzung folgender Callbacks interaktiv gestaltet
 * werden. Die Reihenfolge der Funktionsaufrufe ist folgende:
 * - \ref entityCallbacks_t.onUpdate aller Entitäten
 * - Dann berechnet der EntityHandler die Positionen und Rotationen neu.
 * - Danach wird die Physik aller Entitäten verarbeitet. Beim Auftreten einer
 *   Kollision wird durch das physics-Modul der Callback
 *   \ref entityCallbacks_t.onCollision aufgerufen.
 * - \ref entityCallbacks_t.onDraw aller Entitäten
 */
typedef struct {
    /**
     * @brief Update
     *
     * Jeder Zyklus kann die Entität hiermit auf Eingabeevents reagieren.
     * @note Dieser Callback darf NULL sein.
     *
     * @param self Die Entität dessen Callback gerade aufgerufen wird
     * @param inputEvents Eingabeevents für die Entität
     * 
     * @return Fehlercode gemäss \ref error.h
     */
    int (*onUpdate)(struct entity_s *self, inputEvent_t *inputEvents);

    /**
     * @brief Kollision mit der Welt oder anderen Entitäten
     * 
     * Informiert die Entität über eine stattgefundene Kollision. Die Flags in
     * \ref entityCollision_t.flags geben an welche Kollision genau erfolgt ist.
     * Die Entität darf auf die Informationen physikalisch reagieren, muss dann
     * aber die Flags, auf die sie reagiert hat, 0 setzen. Nach dem Callback
     * reagiert das physik-Modul auf die noch gesetzten Flags.
     * @note Kollidiert eine Entität A mit einer Entität B, so werden beide
     * informiert, denn die Physik kann nicht ermitteln wer der Auslöser der
     * Kollision war. Der \p onCollision Callback wird für A mit \p self = A und
     * \ref entityCollision_t.partner = B aufgerufen und für B wird
     * \p onCollision mit \p self = B und \ref entityCollision_t.partner = A.
     * @note Ist dieser Callback NULL, so wird die Kollision gänzlich ignoriert.
     * Dies bedeutet aber, dass das physik-Modul auch nicht reagiert und die
     * Entität wird durch die Welt fallen.
     * 
     * @param self Die Entität dessen Callback gerade aufgerufen wird
     * @param collision Struktur mit Informationen über die Kollision
     * 
     * @return Fehlercode gemäss \ref error.h
     */
    int (*onCollision)(struct entity_s *self, entityCollision_t *collision);

    /**
     * @brief Zeichnen
     *
     * Jeder Zyklus wird die Entität hiermit aufgefordert sich selbst zu
     * zeichnen.
     * @note Ist dieser Callback NULL, so wird die standard Zeichnen Funktion
     * des EntityHandlers verwendet.

     * @param self Die Entität dessen Callback gerade aufgerufen wird
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
    /**
     * @brief Status der Entität
     * 
     * @warning Darf nur vom EntityHandler verändert werden.
     */
    entityState_t state;

    /**
     * @brief Physikdaten der Entität
     * 
     * @warning Ausser für die Initialisierung sind alle folgenden Änderungen
     * nur mittels Physics-Modul zulässig.
     */
    entityPhysics_t physics;

    /**
     * @brief Liste an Einzelteilen
     * 
     * Aus diesen Einzelteilen besteht die Entität. Damit etwas sichtbar wird,
     * muss mindestens ein Teil vorhanden sein.
     */
    list_t *parts;

    /**
     * @brief Funktionscallbacks für interaktive Entitäten
     * 
     */
    entityCallbacks_t callbacks;

    const char *owner; //!< Eigentümer der Entität, Spielername
    const char *name;  //!< Name der Entität
    void *data;        //!< Optionale Daten der Entität, zur freien Benutzung.
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
