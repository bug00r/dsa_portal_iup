#ifndef PLUGIN_HGEN_UI_FUNCS_H
#define PLUGIN_HGEN_UI_FUNCS_H

#include "dsa_core_defs.h"

#include <iup.h>
#include <iup.h>
#include <cd.h>
#include <cdiup.h>
#include <wd.h>

#include <string.h>

#include "string_utils.h"
#include "xml_utils.h"

#include "iup_tab_utils.h"

#include "plugin_hgen_ui_types.h"
#include "plugin_hgen_ui_utils.h"
#include "plugin_hgen_ui_hero_details.h"


#include "hero.h"

void hgen_check_refresh_rem_hero_btn(hgen_ctx_t *hgen_ctx);
void create_andor_open_hero(hgen_ctx_t *hgen_ctx);

#endif