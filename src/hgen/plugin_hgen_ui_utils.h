#ifndef PLUGIN_HGEN_UI_UTILS_H
#define PLUGIN_HGEN_UI_UTILS_H

#include "dsa_core_defs.h"

#include <string.h>

#include "string_utils.h"
#include "xml_utils.h"
#include "plugin_hgen_ui_types.h"

#include "hero.h"

char* hgen_hero_name_id(dsa_hero_t *hero);
void hgen_change_hero_name(dsa_hero_t *hero, const char* newname);

void init_breeds(Ihandle* ih, dsa_heros_t *heros);
void init_cultures(Ihandle* ih, dsa_heros_t *heros);
void init_professions(Ihandle* ih, dsa_heros_t *heros);

#endif