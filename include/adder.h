/**
 * @file adder.h
 * @author Leuenberger Niklaus (leuen4@bfh.ch)
 * @brief Addition von Ganzzahlen (Vorlage)
 * @version 0.2
 * @date 2021-03-05
 * 
 * @copyright Copyright (c) 2021 Leuenberger Niklaus
 * 
 * Dieses Modul dient als Vorlage für andere Module.
 * Im eventuellen Release ist dies zu entfernen.
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
 * @brief Beispielstruktur
 * 
 */
typedef struct {
    int a; //!< Erste Zahl
    int b; //!< Zweite Zahl
    int result; //!< Ergebnis der Berechnung mittels add2()
} addition_t;


/*
 * Variablendeklarationen
 * 
 */

/* ... */


/*
 * Öffentliche Funktionen
 * 
 */

/**
 * @brief Addiere zwei Ganzzahlen
 * 
 * Übergebene Ganzzahlen a und b werden addiert.
 * Der gesamte Wertebereich von int ist zulässig.
 * 
 * @param a Zahl a
 * @param b Zahl b
 * 
 * @return int a + b
 */
int add1(int a, int b);

/**
 * @brief Addiere zwei Ganzzahlen aus Struktur
 * 
 * Als Alternative zu add1() werden hier die beiden Zahlen \p a und \p b
 * in der \ref addition_t Struktur addiert.
 * Das Resultat der Addition wird einerseits per return zurückgegeben
 * und auch in \ref addition_t.result gespeichert.
 * 
 * @param[in,out] addition Pointer auf Struktur der zu addierenden Zahlen
 * 
 * @return int a + b
 */
int add2(addition_t *addition);
