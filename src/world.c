/**
 * @file world.c
 * @author Stoll Simon (stols4@bfh.ch)
 * @brief Die Spielwelt
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

#include "world.h"


/*
 * Implementation öffentlicher Funktionen
 *
 */

int World_Init() {
	return 0;
}

int World_Load(char *worldID) {
	(void) worldID;
	return 0;
}

int World_DrawBackground() {
	return 0;
}

int World_DrawForeground() {
	return 0;
}

int CheckCollision(SDL_Rect aabb, collisionInfo_t *collision) {
	(void) aabb;
	(void) collision;
	return 0;
}
