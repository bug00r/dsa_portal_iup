#ifndef PLUGIN_HGEN_UI_HERO_DETAILS_H
#define PLUGIN_HGEN_UI_HERO_DETAILS_H

#include "dsa_core_defs.h"

#include <string.h>

#include <iup.h>

#include "iup_std_callbacks.h"
#include "plugin_hgen_ui_types.h"
#include "plugin_hgen_ui_utils.h"
#include "hero.h"

Ihandle* hgen_hero_sheet_new(dsa_hero_t *hero);

void hgen_hero_sheet_init(Ihandle *sheet, dsa_heros_t *heros, dsa_hero_t *hero);

#endif