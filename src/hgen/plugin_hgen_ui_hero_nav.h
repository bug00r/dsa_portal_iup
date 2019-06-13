#ifndef PLUGIN_HGEN_UI_HERO_NAV_H
#define PLUGIN_HGEN_UI_HERO_NAV_H

#include <string.h>

#include "hero.h"

typedef struct __hgen_hero_item_t {
    dsa_hero_t *hero;
    struct __hgen_hero_item_t *next;
} hgen_hero_item_t;

void hgen_hero_nav_add(hgen_hero_item_t *start_item, dsa_hero_t *hero);
void hgen_hero_nav_remove(hgen_hero_item_t *start_item, unsigned int index);
void hgen_hero_nav_get(hgen_hero_item_t *start_item, unsigned int index);

#endif