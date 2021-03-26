# Coderichtlinien

## Modularisierung
- gemäss Vorlage `adder`
- 1x modul.c in `/src`
- 1x modul.h in `/include`
- 1x test_modul.c in `/test`

## Sprache
- alles in Englisch, ausser Kommentare

## Kommentare
- Doxygen Kommentare gemäss Modulvorlage
    - `/** Kommentar */` - über einer Funktion oder Struktur
        - mit `@brief`, `@param[in,out]` und `@return`
    - `//!< Kommentar` - für Elemente einer Struktur
    - oder
    - `/**< Kommentar */`
- Nicht das Was, sondern das Warum Kommentieren.

## Formatierung
- Soft-Tabs
- sauber eingerückt
- Globale Funktionen
    - CamelCase `Object()`
    - +Namespace `Modul_Object()`
    - +Verb `Modul_GetObject()`
- Globale Variablen
    - pascalCase `object`
    - +Namespace `modul_object`
- Lokale Funktionen und Variablen
    - identisch wie globale Funktionen
    - ohne Namespace
