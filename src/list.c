/**
 * @file list.c
 * @author Stoll Simon (stols4@bfh.ch)
 * @brief Single linked list with automatic free function
 * @version 0.0
 * @date 2021-03-19
 *
 * @copyright Copyright (c) 2021 Stoll Simon
 *
 */

/*
 * Includes
 *
 */

#include "list.h"

/*
 * Implementation öfentlicher Funktionen
 *
 */

int List_Create(list_t **list) {
    (void)list;
    return 0;
}

int List_Init(list_t *list) {
    (void)list;
    return 0;
}

int List_Destroy(list_t *list) {
    (void)list;
    return 0;
}

int List_Add(list_t *list, void *data) {
    (void)list;
    (void)data;
    return 0;
}

int List_Remove(list_t *list, void *data) {
    (void)list;
    (void)data;
    return 0;
}

int List_Clear(list_t *list) {
    (void)list;
    return 0;
}

int List_Foreach(list_t *list, fnPntrDataCallback callback) {
    (void)list;
    (void)callback;
    return 0;
}

int List_Search(list_t* list, fnPntrDataCallback searchFn, void** data) {
    (void)list;
    (void)searchFn;
    (void)data;
    return 0;
}
