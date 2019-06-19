#include "plugin_hgen_ui_funcs.h"


void hgen_check_refresh_rem_hero_btn(hgen_ctx_t *hgen_ctx) {

    Ihandle *list = hgen_ctx->ctrls.hero_list;
    Ihandle *btn = hgen_ctx->ctrls.hero_rem_button;

    int sel = IupGetInt(list, "VALUE");

    if (sel > 0) {
        IupSetAttribute(btn, "ACTIVE", "yes");
    } else {
        IupSetAttribute(btn, "ACTIVE", "no");
    }

}

void create_andor_open_hero(hgen_ctx_t *hgen_ctx) {
	
    hero_nav_item_t *hero_item = hgen_ctx->selected_hero;

	Ihandle *tabs = hgen_ctx->ctrls.hero_tabs;

    Ihandle * hero_frame = hero_item->detail_frame;

	if ( hero_frame == NULL ) {
        
        dsa_hero_t *hero = hero_item->hero;
        char * hero_name = hgen_hero_name_id(hero);

        DEBUG_LOG_ARGS("create new : %s\n",hero_name);

        hero_frame = hgen_hero_sheet_new(hero);
        IupSetStrAttribute(hero_frame, "hero_name", hero_name);
        hero_item->detail_frame = hero_frame;

		if(hero_frame) {

			DEBUG_LOG_ARGS("map new : %s\n",hero_name);

			IupSetAttribute(hero_frame, "EXPANDCHILDREN", "YES");
			IupSetStrAttribute(hero_frame, "TABTITLE", hero_name);
			
			IupAppend(tabs, hero_frame);
			
			IupMap(hero_frame); //important: IupCreate  -> IupAppend -> IupMap
			
            hgen_hero_sheet_init(hero_frame, hgen_ctx->heros, hero);

            IupSetAttribute(tabs, "VALUE_HANDLE", (void*)hero_frame);

		}

        free(hero_name);

	} else {
		
        IupSetAttribute(tabs, "VALUE_HANDLE", (void*)hero_frame);
         
	}
	
	IupRefresh(tabs);

}
