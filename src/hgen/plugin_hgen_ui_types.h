#ifndef PLUGIN_HGEN_UI_TYPES_H
#define PLUGIN_HGEN_UI_TYPES_H

#include <iup.h>
#include <cd.h>
#include <cdiup.h>
#include <wd.h>

#include "dl_list.h"

#include "hero.h"
#include "plugin.h"
#include "plugin_hgen_ui.h"
#include "plugin_hgen_ui_init.h"

typedef struct _hgen_ctx_ {
	Ihandle *frame;
	archive_resource_t* archive;
	dsa_heros_t * heros;
	dl_list_t * nav_heros;
} hgen_ctx_t;

#endif