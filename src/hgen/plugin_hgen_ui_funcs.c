#include "plugin_hgen_ui_funcs.h"

static char* __hgen_hero_name_id(dsa_hero_t *hero) {

    xmlChar * xmlhaname = dsa_heros_get_name(hero);
    xmlChar * xmlhid = dsa_heros_get_id(hero);

    char * hero_name = format_string_new("%s (%s)", xmlhaname, xmlhid);

	xmlFree(xmlhaname);
    xmlFree(xmlhid);

    return hero_name;
}

#if 0 
//#####################################################################################################
//EOF private Section
//#####################################################################################################
#endif

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
    dsa_hero_t *hero = hero_item->hero;

    char * hero_name = __hgen_hero_name_id(hero);

	DEBUG_LOG_ARGS("create andor open hero %s\n", hero_name);

	Ihandle *tabs = hgen_ctx->ctrls.hero_tabs;

    int child_index = iup_tap_index_by_title(tabs, hero_name);
	
	DEBUG_LOG_ARGS("found tab index %i \n", child_index);

	if ( child_index == -1 ) {
        
        Ihandle * hero_frame = hero_item->detail_frame;
        
        if (hero_frame == NULL) {

			DEBUG_LOG_ARGS("create new : %s\n",hero_name);
	
		    hero_frame = hgen_hero_sheet_new(hero);
            IupSetStrAttribute(hero_frame, "hero_name", hero_name);
            hero_item->detail_frame = hero_frame;
        }

		if(hero_frame) {

			DEBUG_LOG_ARGS("map new : %s\n",hero_name);
			
			int tabpos = IupGetChildCount(tabs);

			IupSetAttribute(hero_frame, "EXPANDCHILDREN", "YES");
			IupSetStrAttribute(hero_frame, "TABTITLE", hero_name);
			IupSetInt(hero_frame, "TABPOS", tabpos);
			
			IupAppend(tabs, hero_frame);
			
			IupMap(hero_frame); //important: IupCreate  -> IupAppend -> IupMap
			
			IupSetInt(tabs, "VALUEPOS", tabpos);
			
		}
	} else {
		
		DEBUG_LOG_ARGS("set current tab %i\n", child_index);
		
		IupSetAttributeId(tabs, "TABVISIBLE", child_index, "YES");
		
		IupSetInt(tabs, "VALUEPOS", child_index);
	}
	
	IupRefresh(tabs);

    free(hero_name);
}
