/**
 * @file list.h
 * @author Stoll Simon (stols4@bfh.ch)
 * @brief Einfach verlinkte Liste
 * @version 0.1
 * @date 2021-03-19
 *
 * @copyright Copyright (c) 2021 Stoll Simon
 *
 */

#pragma once


/*
 * Typdeklarationen
 *
 */

/**
 * @brief Rückruffunktion mit Listendaten als Argument
 * 
 */
typedef int (*fnPntrDataCallback)(void *data);

/**
 * @brief Rückruffunktion mit Listendaten als Argument und zusätzliche Daten die vom Benutzer angegeben wurden
 * 
 */
typedef int (*fnPntrDataCallbackArg)(void *data, void *userData);

/**
 * @brief Ein Element der Liste \ref list_t
 *
 */
typedef struct listElement_s {
    void *data;                        //!< Der Datenpointer, NULL = keine Daten
    struct listElement_s *nextElement; //!< Das nächste Element in der Liste
} listElement_t;

/**
 * @brief Einfach verlinkte dynamische liste
 *
 */
typedef struct list_t {
    listElement_t *listHead;         //!< Das erste Element in der Liste
    int elementCount;                //!< Anzahl Elemente in der Liste
    fnPntrDataCallback dataAutoFree; //!< Wird benutzt um Datenstrukturen beim Entfernen automatisch zu deallozieren.
} list_t;

/*
 * Öffentliche Funktionen
 *
 */

/**
 * @brief Erstellt und initialisiert eine list_t.
 * Wird gebraucht um ein neuen \ref list_t* zu erstellen.
 *
 * @param[out] list Der Ort wo die Liste erstellt werden soll
 *
 * @return 0 oder Errorcode
 */
int List_Create(list_t **list);

/**
 * @brief Initialisiert eine list_t
 * Wird gebraucht um einen nicht Pointer \ref list_t zu initialisieren.
 *
 * @param[in] list Die Liste die Initialisiert werden soll
 *
 * @return 0 oder Errorcode
 */
int List_Init(list_t *list);

/**
 * @brief Befreit die benutzten Ressourcen der Liste.
 * Befreit auch die benutzten Ressourcen der Elemente.
 * Für jedes Element wird \ref list_t.dataAutoFree aufgerufen, falls vorhanden.
 *
 * @param[in] list Der Pointer zur Liste die zerstört werden soll
 *
 * @return 0 oder Errorcode
 */
int List_Destroy(list_t **list);

/**
 * @brief Fügt ein Element der Liste hinzu.
 * Das hinzugefügte Element steht am Anfang der Liste.
 * Daten können doppelt vorkommen.
 *
 * @param[in] list Die Liste die bearbeitet wird
 * @param[in] data Die Daten die hinzugefügt werden sollen
 *
 * @return 0 oder Errorcode
 */
int List_Add(list_t *list, void *data);

/**
 * @brief Entfernt das angegebene Element.
 * Falls das Element doppelt in der Liste vorhanden ist, wird nur das Erste entfernt.
 * Für das Element wird \ref list_t.dataAutoFree aufgerufen, falls vorhanden.
 *
 * @param[in] list Die Liste die bearbeitet wird
 * @param[in] data Die Daten die entfernt werden sollen
 *
 * @return 0 oder Errorcode
 */
int List_Remove(list_t *list, void *data);

/**
 * @brief Entfernt alle Elemente aus der Liste.
 * Für jedes Element wird \ref list_t.dataAutoFree aufgerufen, falls vorhanden.
 *
 * @param[in] list Die Liste die bearbeitet wird
 *
 * @return 0 oder Errorcode
 */
int List_Clear(list_t *list);

/**
 * @brief Führt \p callback für alle Elemente aus.
 * Bei einem Error wird die Schleife abgebrochen.
 *
 * @param[in] list Die Liste die bearbeitet wird
 * @param[in] callback Funktion die jedes Element durchführen wird
 *
 * @return 0 oder Errorcode
 */
int List_Foreach(list_t *list, fnPntrDataCallback callback);

/**
 * @brief Führt \p callback für alle Elemente aus.
 * Bei einem Error wird die Schleife abgebrochen.
 *
 * @param[in] list Die Liste die bearbeitet wird
 * @param[in] callback Funktion die jedes Element durchführen wird
 * @param[in] userData Daten die jeder Aufruf zusätzlich bekommt
 *
 * @return 0 oder Errorcode
 */
int List_ForeachArg(list_t *list, fnPntrDataCallbackArg callback, void *userData);

/**
 * @brief Sucht das erste Element das bei \p searchCmpFn 1 zurückgibt.
 * Das gefundene Element wird in \p **data gespeichert.
 * 
 * @note \p searchCmpFn sollte nur 0 oder 1 ausgeben.
 *
 * @param[in] list Die Liste die bearbeitet wird
 * @param[in] searchCmpFn Suchfunktion die für jedes Element aufgerufen wird
 * @param[out] data Der Ort wo das gesuchte Element ausgegeben wird. NULL = Nichts gefunden
 *
 * @return 0 oder Errorcode
 */
int List_Search(list_t *list, fnPntrDataCallback searchCmpFn, void **data);

/**
 * @brief Sucht das erste Element das bei \p searchCmpFn 1 zurückgibt.
 * Das gefundene Element wird in \p **data gespeichert.
 * 
 * @note \p searchCmpFn sollte nur 0 oder 1 ausgeben.
 *
 * @param[in] list Die Liste die bearbeitet wird
 * @param[in] searchCmpFn Suchfunktion die für jedes Element aufgerufen wird
 * @param[in] userData Daten die jeder Aufruf zusätzlich bekommt
 * @param[out] data Der Ort wo das gesuchte Element ausgegeben wird. NULL = Nichts gefunden
 *
 * @return 0 oder Errorcode
 */
int List_SearchArg(list_t *list, fnPntrDataCallbackArg searchCmpFn, void *userData, void **data);
