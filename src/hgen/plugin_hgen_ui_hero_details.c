#include "plugin_hgen_ui_hero_details.h"

static int __hgen_on_change_hero_name(Ihandle *ih, int c, char *new_value) {

    DEBUG_LOG_ARGS("new name: %s ctrl: %i \n", new_value, c);

    if (strlen(new_value) > 0) {
        
        DEBUG_LOG("input valid\n");
        
        dsa_hero_t *hero = (dsa_hero_t *)IupGetAttribute(ih, "hero");

        DEBUG_LOG_ARGS("[I] Hero should not be null (%p)\n", hero);

        if ( hero != NULL ) {
            //change hero name
            //change list entry,
            //change tabtitle
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

	IupSetHandle("breeds", lst_breed);
	IupSetHandle("cultures", lst_culture);
	IupSetHandle("professions", lst_prof);

    Ihandle *detail_frame = IupHbox(gbox, NULL);

	return detail_frame;
}
