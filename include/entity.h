/**
 * @file entity.h
 * @author Leuenberger Niklaus (leuen4@bfh.ch)
 * @brief Datenstruktur einer Entität
 * @version 0.1
 * @date 2021-03-19
 * 
 * @copyright Copyright (c) 2021 Leuenberger Niklaus
 * 
 */

#pragma once


/*
 * Includes
 * 
 */

#include "sdlWrapper.h"


/*
 * Typdeklarationen
 * 
 */

/**
 * @brief 2D-Vektor
 * 
 * @note Keine kartesischen Koordinaten!
 * @note Ursprung ist oben links
 * 
 */
typedef struct {
    double x; //!< X Komponente
    double y; //!< Y Komponente
} entityVector_t;

/**
 * @brief Physikalische Daten einer Entität
 * 
 * @warning Nur das physics-Modul darf Werte dieser Struktur verändern.
 * Für Manipulationen stehen diverse Physics_Set*() Funktionen zur verfügung.
 * 
 */
typedef struct {
    entityVector_t position;     //!< Position der linken unteren Ecke der Entität
    entityVector_t velocity;     //!< Geschwindigkeit der Positionsänderung
    entityVector_t acceleration; //!< Beschleunigung
    SDL_Rect aabb;               //!< Kollisionsbox / Axis-Aligned Bounding Box
} entityPhysics_t;

/**
 * @brief Datenstruktur einer Entität
 * 
 */
typedef struct {
    entityPhysics_t physics; //!< Physikdaten der Entität, wird vom physics-Modul verwaltet.
    void *data;              //!< Pointer auf optionale Daten der Entität, zur freien Benutzung.
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
