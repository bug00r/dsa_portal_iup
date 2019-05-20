#ifndef PLUGIN_LEXICON_UI_H
#define PLUGIN_LEXICON_UI_H

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <iup.h>
#include <cd.h>
#include <cdiup.h>
#include <wd.h>

#include "plugin_lexicon_ui_callback.h"

Ihandle* create_lexicon_searchbar();

Ihandle* create_lexicon_result_frame();

Ihandle* create_lexicon_frame();

#endif
