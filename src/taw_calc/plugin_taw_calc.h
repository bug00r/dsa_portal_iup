#ifndef PLUGIN_UI_TAW_CALC_H
#define PLUGIN_UI_TAW_CALC_H

#include "defs.h"

#include <stdlib.h>
#include <stdio.h>
#include <iup.h>
#include <cd.h>
#include <cdiup.h>
#include <wd.h>

#include "plugin.h"
#include "taw.h"
#include "xml_utils.h"
#include "iup_xml_builder.h"

typedef struct _taw_calc_ctx_ {
	Ihandle *frame;
	xml_source_t *ui_res;
	iup_xml_builder_t *builder;
} taw_calc_ctx_t;

plugin_t * taw_calc_plugin(plugin_t * plugin, void *data);

#endif