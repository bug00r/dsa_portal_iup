#include "plugin_hgen_ui_funcs.h"

static char* __hgen_hero_name_id(dsa_hero_t *hero) {

    xmlChar * xmlhaname = dsa_heros_get_name(hero);
    xmlChar * xmlhid = dsa_heros_get_id(hero);

    char * hero_name = format_string_new("%s (%s)", xmlhaname, xmlhid);

	xmlFree(xmlhaname);
    xmlFree(xmlhid);

    return hero_name;
}

static Ihandle* __hgen_hero_sheet_new(dsa_hero_t *hero) {
	Ihandle *lbl_name = IupLabel("Name:");
	
	Ihandle *txt_name = IupText(NULL);
	IupSetAttributes(txt_name,"RASTERSIZE=125, MULTILINE=NO");

	Ihandle *btn_rnd_name = IupButton("<?>",NULL);
	IupSetCallback(btn_rnd_name,"ACTION",(Icallback)default_action_callback);
	
	Ihandle *lbl_breed = IupLabel("Breed:");
	
	Ihandle *lst_breed = IupList(NULL);
	IupSetAttributes(lst_breed, "RASTERSIZE=125, DROPDOWN=YES, VISIBLEITEMS=15");

	Ihandle *btn_edt_breed = IupButton("<e>",NULL);
	IupSetCallback(btn_edt_breed,"ACTION",(Icallback)default_action_callback);

	Ihandle *lbl_culture = IupLabel("Culture:");
	
	Ihandle *lst_culture = IupList(NULL);
	IupSetAttributes(lst_culture, "RASTERSIZE=125, DROPDOWN=YES, VISIBLEITEMS=15");

	Ihandle *btn_edt_culture = IupButton("<e>",NULL);
	IupSetCallback(btn_edt_culture,"ACTION",(Icallback)default_action_callback);

	Ihandle *lbl_prof = IupLabel("Profession:");
	
	Ihandle *lst_prof = IupList(NULL);
	IupSetAttributes(lst_prof, "RASTERSIZE=125, DROPDOWN=YES, VISIBLEITEMS=15");

	Ihandle *btn_edt_prof = IupButton("<e>",NULL);
	IupSetCallback(btn_edt_prof,"ACTION",(Icallback)default_action_callback);

	Ihandle *gbox = IupGridBox(
		lbl_name, txt_name, btn_rnd_name,
		lbl_breed, lst_breed, btn_edt_breed,
		lbl_culture, lst_culture, btn_edt_culture,
		lbl_prof, lst_prof, btn_edt_prof,
	NULL);

	IupSetAttribute(gbox,"ORIENTATION","HORIZONTAL");
	IupSetAttribute(gbox,"NUMDIV","3");
	IupSetAttribute(gbox,"EXPAND","NO");
	IupSetAttribute(gbox, "SIZELIN", "-1");
	IupSetAttribute(gbox, "SIZECOL", "-1");
	IupSetAttribute(gbox, "GAPLIN", "5");
	IupSetAttribute(gbox, "GAPCOL", "5");
	IupSetAttribute(gbox, "MARGIN", "5x5");
	IupSetAttribute(gbox,"ALIGNMENTLIN","ACENTER");
	IupSetAttribute(gbox,"ALIGNMENTCOL","ALEFT");

	IupSetHandle("breeds", lst_breed);
	IupSetHandle("cultures", lst_culture);
	IupSetHandle("professions", lst_prof);

	return IupHbox(gbox, NULL);
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

	#if debug > 0
		printf("create andor open hero %s\n", hero_name);
	#endif
	
	
	bool exist = false;
	Ihandle * child = NULL;
	
	Ihandle *tabs = hgen_ctx->ctrls.hero_tabs;
	int i = IupGetChildCount(tabs);
	
	#if debug > 0
		printf("child count %i\n", i);
	#endif
	
	for (;i--;) {
		
		child = IupGetChild(tabs, i);
		
		if ( child ) {
		
			char * tabname = IupGetAttribute(child,"TABTITLE");
			
			#if debug > 0
				printf("found child tab: %s\n",tabname);
			#endif
			
			exist = (strcmp(tabname, hero_name) == 0);
			
            #if debug > 0
				printf("tab: %s heroname: %s  exist: %i\n",tabname, hero_name, exist);
			#endif

			if ( exist )
				break;
		}
	
	}
	
	if ( !exist ) {
        
        Ihandle * hero_frame = hero_item->detail_frame;
        
        if (hero_frame == NULL) {

            #if debug > 0
				printf("create new : %s\n",hero_name);
			#endif

		    hero_frame = __hgen_hero_sheet_new(hero);
            hero_item->detail_frame = hero_frame;
        }

		if(hero_frame) {

            #if debug > 0
				printf("map new : %s\n",hero_name);
			#endif
			
			int tabpos = IupGetChildCount(tabs);

			IupSetAttribute(hero_frame, "EXPANDCHILDREN", "YES");
			IupSetStrAttribute(hero_frame, "TABTITLE", hero_name);
			IupSetInt(hero_frame, "TABPOS", tabpos);
			
			IupAppend(tabs, hero_frame);
			
			IupMap(hero_frame); //important: IupCreate  -> IupAppend -> IupMap
			
			IupSetInt(tabs, "VALUEPOS", tabpos);
			
		}
	} else {
	
		int tabpos = IupGetInt(child, "TABPOS");
		
		#if debug > 0
			printf("set current tab %i\n", tabpos);
		#endif
		
		IupSetAttributeId(tabs, "TABVISIBLE", tabpos, "YES");
		
		IupSetInt(tabs, "VALUEPOS", tabpos);
	}
	
	IupRefresh(tabs);

    free(hero_name);
}
