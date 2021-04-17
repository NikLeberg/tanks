/**
 * @file test_sprite.c
 * @author Stoll Simon (stols4@bfh.ch)
 * @brief Tests für sprite-Modul
 * @version 0.0
 * @date 2021-04-03
 *
 * @copyright Copyright (c) 2021 Stoll Simon
 *
 */

/*
 * Includes
 * 
 */

#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>

#include "sdlWrapper.h"
#include "error.h"


/*
 * Tests
 * 
 */

/**
 * @brief Testet die Positionsänderung von einem Sprite.
 * 
 * @param state unbenutzt
 */
static void testRelativePoint(void **state) {
    (void)state;
    sprite_t sprite = (sprite_t){0};
    sprite_t transformedSprite = (sprite_t){0};

    sprite.pivot.x = 10;
    sprite.destination.x = -20;
    sprite.destination.y = -10;

    assert_int_equal(Sprite_SetRelativeToPivot(sprite, 0, (SDL_Point){0}, NULL), ERR_NULLPARAMETER);

    Sprite_SetRelativeToPivot(sprite, 0, (SDL_Point){0, 10}, &transformedSprite);
    assert_float_equal(transformedSprite.rotation, 0, 0.01f);
    assert_int_equal(transformedSprite.position.x, 0);
    assert_int_equal(transformedSprite.position.y, 0);

    Sprite_SetRelativeToPivot(sprite, 45, (SDL_Point){0, 10}, &transformedSprite);
    assert_float_equal(transformedSprite.rotation, 45, 0.01f);
    assert_int_equal(transformedSprite.position.x, 17);
    assert_int_equal(transformedSprite.position.y, -1);

    Sprite_SetRelativeToPivot(sprite, 90, (SDL_Point){0, 10}, &transformedSprite);
    assert_float_equal(transformedSprite.rotation, 90, 0.01f);
    assert_int_equal(transformedSprite.position.x, 30);
    assert_int_equal(transformedSprite.position.y, 10);
}

/**
 * @brief Testet die berechnung der einzelnen Subsprites eines Multisprites.
 * 
 * @param state 
 * 
 */
static void test_multiSprite(void **state) {
    (void)state;
    sprite_t sprite = (sprite_t){0};
    sprite.destination.w = 100;
    sprite.destination.h = 100;
    sprite.multiSpriteSize = (SDL_Point){4, 2};
    sprite.multiSpriteCount = 5;
    assert_int_equal(Sprite_SetFrame(NULL, 0), ERR_NULLPARAMETER);
    assert_int_equal(Sprite_NextFrame(&sprite), ERR_PARAMETER);
    assert_int_equal(Sprite_SetFrame(&sprite, 0), ERR_PARAMETER);
    sprite.texture = (void *)1;
    {
        assert_int_equal(Sprite_SetFrame(&sprite, 0), ERR_OK);
        assert_int_equal(sprite.source.x, 0);
        assert_int_equal(sprite.source.y, 0);
        assert_int_equal(sprite.source.w, 25);
        assert_int_equal(sprite.source.h, 50);

        assert_int_equal(Sprite_SetFrame(&sprite, 1), ERR_OK);
        assert_int_equal(sprite.source.x, 25);
        assert_int_equal(sprite.source.y, 0);
        assert_int_equal(sprite.source.w, 25);
        assert_int_equal(sprite.source.h, 50);

        assert_int_equal(Sprite_SetFrame(&sprite, 2), ERR_OK);
        assert_int_equal(sprite.source.x, 50);
        assert_int_equal(sprite.source.y, 0);
        assert_int_equal(sprite.source.w, 25);
        assert_int_equal(sprite.source.h, 50);

        assert_int_equal(Sprite_SetFrame(&sprite, 3), ERR_OK);
        assert_int_equal(sprite.source.x, 75);
        assert_int_equal(sprite.source.y, 0);
        assert_int_equal(sprite.source.w, 25);
        assert_int_equal(sprite.source.h, 50);

        assert_int_equal(Sprite_SetFrame(&sprite, 4), ERR_OK);
        assert_int_equal(sprite.source.x, 0);
        assert_int_equal(sprite.source.y, 50);
        assert_int_equal(sprite.source.w, 25);
        assert_int_equal(sprite.source.h, 50);
    }
    assert_int_equal(Sprite_SetFrame(&sprite, 5), ERR_PARAMETER);
    assert_int_equal(Sprite_NextFrame(&sprite), ERR_OK);
    assert_int_equal(sprite.multiSpriteIndex, 0);
}

/**
 * @brief Testprogramm
 * 
 * @return int Anzahl fehlgeschlagener Tests
 */
int main(void) {
    const struct CMUnitTest sprite[] = {
        cmocka_unit_test(testRelativePoint),
        cmocka_unit_test(test_multiSprite)
    };
    return cmocka_run_group_tests(sprite, NULL, NULL);
}
