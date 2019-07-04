#ifndef PLUGIN_LEXICON_UI_CALLBACK_H
#define PLUGIN_LEXICON_UI_CALLBACK_H

#include "defs.h"

#include <iup.h>
#include <cd.h>
#include <cdiup.h>
#include <wd.h>

#include "plugin_lexicon_ui_search.h"

int search_button_callback(Ihandle *search_button);

int search_input_key_callback(Ihandle *ih, int c);

int on_category_changed_cb(Ihandle *ih, char *text, int item, int state);

int on_group_changed_cb(Ihandle *ih, char *text, int item, int state);

int on_result_list_changed_cb(Ihandle *ih, char *text, int item, int state);

#endif