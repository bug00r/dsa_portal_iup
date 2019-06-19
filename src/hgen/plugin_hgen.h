#ifndef PLUGIN_UI_HGEN_H
#define PLUGIN_UI_HGEN_H

#include "dsa_core_defs.h"

#include <stdlib.h>
#include <stdio.h>
#include <iup.h>
#include <cd.h>
#include <cdiup.h>
#include <wd.h>

#include "dl_list.h"

#include "hero.h"
#include "plugin.h"
#include "plugin_hgen_ui.h"

#include "plugin_hgen_ui_types.h"

/* typedef struct _hgen_ctx_ {
	Ihandle *frame;
	archive_resource_t* archive;
	dsa_heros_t * heros;
	dl_list_t * nav_heros;
} hgen_ctx_t;
*/

plugin_t * hgen_plugin(plugin_t * plugin);

#endif