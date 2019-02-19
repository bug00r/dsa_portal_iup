#ifndef PLUGIN_UI_TAW_CALC_H
#define PLUGIN_UI_TAW_CALC_H

#include <stdlib.h>
#include <stdio.h>
#include <iup.h>
#include <cd.h>
#include <cdiup.h>
#include <wd.h>

#include "plugin.h"

typedef struct _taw_calc_ctx_ {
	Ihandle *frame;
} taw_calc_ctx_t;

plugin_t * taw_calc_plugin(plugin_t * plugin);

#endif