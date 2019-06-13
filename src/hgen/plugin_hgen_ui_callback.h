#ifndef PLUGIN_HGEN_UI_CALLBACK_H
#define PLUGIN_HGEN_UI_CALLBACK_H

#include <string.h>

#include <iup.h>

#include "hero.h"

#include "plugin_hgen_ui_hero_nav.h"

void hgen_add_new_hero_callback(Ihandle *ih);
int  hgen_select_hero_callback(Ihandle *ih, char *text, int item, int state);

#endif