#include "plugin_hgen_ui_hero_details.h"

static int __hgen_on_change_hero_name(Ihandle *ih, int c, char *new_value) {

    DEBUG_LOG_ARGS("new name: %s ctrl: %i \n", new_value, c);

    if (strlen(new_value) > 0) {
        
        DEBUG_LOG("input valid\n");
        
        dsa_hero_t *hero = (dsa_hero_t *)IupGetAttribute(ih, "hero");

        DEBUG_LOG_ARGS("[I] Hero should not be null (%p)\n", hero);

        if ( hero != NULL ) {
            
            hgen_change_hero_name(hero, new_value);
        
        }

    }

    return IUP_DEFAULT;

}

Ihandle* hgen_hero_sheet_new(dsa_hero_t *hero) {
	Ihandle *lbl_name = IupLabel("Name:");
	
	Ihandle *txt_name = IupText(NULL);
	IupSetAttributes(txt_name,"RASTERSIZE=125, MULTILINE=NO");
    IupSetAttribute(txt_name, "hero", (void*)hero);
    IupSetCallback(txt_name,"ACTION",(Icallback)__hgen_on_change_hero_name);
    
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

    Ihandle *detail_frame = IupHbox(gbox, NULL);

    IupSetAttribute(detail_frame, "culture_list", (void*)lst_culture);
    IupSetAttribute(detail_frame, "breed_list", (void*)lst_breed);
    IupSetAttribute(detail_frame, "prof_list", (void*)lst_prof);
    IupSetAttribute(detail_frame, "name_txt", (void*)txt_name);

	return detail_frame;
}

void hgen_hero_sheet_init(Ihandle *sheet, dsa_heros_t *heros, dsa_hero_t *hero) {

    xmlChar * name =  dsa_heros_get_name(hero);
    IupSetStrAttribute((Ihandle *)IupGetAttribute(sheet, "name_txt"), "VALUE", (const char*) name);
    xmlFree(name);

    init_breeds((Ihandle *)IupGetAttribute(sheet, "breed_list"), heros);
	init_cultures((Ihandle *)IupGetAttribute(sheet, "culture_list"), heros);
	init_professions((Ihandle *)IupGetAttribute(sheet, "prof_list"), heros);

}