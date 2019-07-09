#ifndef PLUGIN_HGEN_UI_TYPES_H
#define PLUGIN_HGEN_UI_TYPES_H

#include <iup.h>
#include <cd.h>
#include <cdiup.h>
#include <wd.h>

#include "dl_list.h"

#include "hero.h"
//#include "plugin.h"
//#include "plugin_hgen_ui.h"
//#include "plugin_hgen_ui_init.h"

typedef struct {
	Ihandle *hero_list;
	Ihandle *hero_rem_button;
	Ihandle *hero_save_button;
	Ihandle *hero_tabs;
} hgen_ui_ctrls_t;

typedef struct {
	dsa_hero_t *hero;
	Ihandle * detail_frame;
	//todo add single builder for each hero nav.
} hero_nav_item_t;

typedef struct _hgen_ctx_ {
	Ihandle *frame;
	archive_resource_t* archive;
	dsa_heros_t * heros;
	hero_nav_item_t * selected_hero;
	int selected_list_pos;
	dl_list_t * nav_heros;
	hgen_ui_ctrls_t ctrls;
} hgen_ctx_t;

#endif