/**
 * @file error.h
 * @author Leuenberger Niklaus (leuen4@bfh.ch)
 * @brief Globale Fehlercodes
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

/* ... */


/*
 * Typdeklarationen
 * 
 */

/**
 * @brief Fehlercodes
 * 
 */
typedef enum {
    ERR_OK = 0,         //!< Kein Fehler
    ERR_FAIL,           //!< Allgemeiner Fehler
    ERR_PARAMETER,      //!< Ungültiger Parameter übergeben
    ERR_NULLPARAMETER,  //!< Paramter als Null übergeben der aber nicht NULL sein darf
    ERR_MEMORY,         //!< Unzureichender Arbeitsspeicher
    ERR_SEQUENCE,       //!< Sequenzfehler, vorher muss noch etwas anderes getan werden
} error_t;


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
