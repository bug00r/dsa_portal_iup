#ifndef PLUGIN_UI_MAIN_H
#define PLUGIN_UI_MAIN_H

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <iup.h>
#include <cd.h>
#include <cdiup.h>
#include <im_util.h>
#include <wd.h>
#include <archive.h>
#include <archive_entry.h>

#include "dsa_portal_defs.h"
#include "resource.h"
#include "plugin.h"

#include "resource.h"

//plugins
#include "plugin_ui_lexicon.h"
#include "plugin_ui_taw_calc.h"

typedef struct _main_ctx_ {
	Ihandle *frame;
	unsigned int cntplugins;
	plugin_t *plugins;
	archive_resource_t *archive;
	resource_search_result_t *xml_result;
} main_ctx_t;

plugin_t * main_plugin(plugin_t * plugin);

#endif