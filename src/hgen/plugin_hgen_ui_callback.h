#ifndef PLUGIN_HGEN_UI_CALLBACK_H
#define PLUGIN_HGEN_UI_CALLBACK_H

#include "dsa_core_defs.h"

#include <string.h>

#include "plugin_hgen_ui_types.h"
#include "plugin_hgen_ui_funcs.h"

#include "hero.h"

void hgen_add_new_hero_callback(Ihandle *ih);
void hgen_rem_sel_hero_callback(Ihandle *ih);

int hgen_select_hero_callback(Ihandle *ih, char *text, int item, int state);

int hgen_on_close_hero_tab(Ihandle* ih, int pos);
int hgen_on_change_hero_tab(Ihandle* ih, int new_pos, int old_pos);

#endif