#ifndef PLUGIN_HGEN_UI_CALLBACK_H
#define PLUGIN_HGEN_UI_CALLBACK_H

#include <string.h>

#include "plugin_hgen_ui_types.h"
#include "plugin_hgen_ui_funcs.h"

#include "hero.h"

void hgen_add_new_hero_callback(Ihandle *ih);
void hgen_rem_sel_hero_callback(Ihandle *ih);

int hgen_show_hero_callback(Ihandle *ih, int item, char *text);
int hgen_select_hero_callback(Ihandle *ih, char *text, int item, int state);

#endif