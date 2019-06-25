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

static int __hgen_on_change_breed(Ihandle *ih, char *text, int item, int state) {

	dsa_hero_t *hero = (dsa_hero_t *)IupGetAttribute(ih, "hero");
	dsa_heros_t *heros = (dsa_heros_t *)IupGetAttribute(ih, "heros");

	dsa_heros_add_breed(heros, hero, (const unsigned char *)text);

	Ihandle *hair_list = (Ihandle *)IupGetAttribute(ih, "hair_list");
	Ihandle *eye_list = (Ihandle *)IupGetAttribute(ih, "eye_list");

	IupSetAttribute(hair_list,"REMOVEITEM", NULL);
	IupSetAttribute(eye_list,"REMOVEITEM", NULL);

	init_hair_colors(hair_list, hero);
	init_eye_colors(eye_list, hero);

	return IUP_DEFAULT;
}

static int __hgen_on_change_culture(Ihandle *ih, char *text, int item, int state) {

	dsa_hero_t *hero = (dsa_hero_t *)IupGetAttribute(ih, "hero");
	dsa_heros_t *heros = (dsa_heros_t *)IupGetAttribute(ih, "heros");

	dsa_heros_add_culture(heros, hero, (const unsigned char *)text);

	return IUP_DEFAULT;
}

static int __hgen_on_change_profession(Ihandle *ih, char *text, int item, int state) {

	dsa_hero_t *hero = (dsa_hero_t *)IupGetAttribute(ih, "hero");
	dsa_heros_t *heros = (dsa_heros_t *)IupGetAttribute(ih, "heros");

	dsa_heros_add_profession(heros, hero, (const unsigned char *)text);

	return IUP_DEFAULT;
}

static void __hgen_set_height_raw(Ihandle *height_field, dsa_hero_t *hero) {

	xmlChar *val = dsa_heros_get_height(hero);

	DEBUG_LOG_ARGS("height from hero: %s\n", val);

	IupSetStrAttribute(height_field, "VALUE", val);

	xmlFree(val);

	val = dsa_heros_get_weight(hero);

	DEBUG_LOG_ARGS("weight from hero: %s\n", val);

	Ihandle *weight = (Ihandle *)IupGetAttribute(height_field, "weight");

	IupSetStrAttribute(weight, "TITLE", val);

	xmlFree(val);

	IupRefresh(weight);
} 

static int __hgen_on_change_height(Ihandle *ih, int pos) {
	
	char * new_height = format_string_new("%i", pos);

	DEBUG_LOG_ARGS("new height: %s\n", new_height);

	dsa_hero_t *hero = (dsa_hero_t *)IupGetAttribute(ih, "hero");

	dsa_heros_set_height_weight_by_value(hero, new_height);

	free(new_height);

	__hgen_set_height_raw(ih, hero);

	return IUP_DEFAULT;
}

static void __hgen_on_change_height_dice(Ihandle *ih) {

	Ihandle *height_field = (Ihandle *)IupGetAttribute(ih, "height");

	dsa_hero_t *hero = (dsa_hero_t *)IupGetAttribute(height_field, "hero");

	dsa_heros_set_height_weight_by_dice(hero);

	__hgen_set_height_raw(height_field, hero);
}

static int __hgen_on_change_hair_col(Ihandle *ih, char *text, int item, int state) {
	
	if ( item > 1  && state == 1) {
		dsa_hero_t *hero = (dsa_hero_t *)IupGetAttribute(ih, "hero");

		dsa_heros_set_col_hair_by_name(hero, (const unsigned char*) text);

	}

	return IUP_DEFAULT;
	
}

static int __hgen_on_change_eye_col(Ihandle *ih, char *text, int item, int state) {
	
	if ( item > 1  && state == 1) {
		dsa_hero_t *hero = (dsa_hero_t *)IupGetAttribute(ih, "hero");

		dsa_heros_set_col_eye_by_name(hero, (const unsigned char*) text);

	}

	return IUP_DEFAULT;
	
}

static void __hgen_on_change_hair_col_dice(Ihandle *ih) {

	Ihandle *hair_list = (Ihandle *)IupGetAttribute(ih, "hair_list");

	dsa_hero_t *hero = (dsa_hero_t *)IupGetAttribute(hair_list, "hero");

	dsa_heros_set_col_hair_by_dice(hero);

	xmlChar * newhaircol = dsa_heros_get_hair_col(hero);

	IupSetStrAttribute(hair_list, "VALUESTRING", newhaircol); 

	DEBUG_LOG_ARGS("New rnd hair: %s\n", newhaircol);

	xmlFree(newhaircol);
}

static void __hgen_on_change_eye_col_dice(Ihandle *ih) {

	Ihandle *eye_list = (Ihandle *)IupGetAttribute(ih, "eye_list");

	dsa_hero_t *hero = (dsa_hero_t *)IupGetAttribute(eye_list, "hero");

	dsa_heros_set_col_eye_by_dice(hero);

	xmlChar * neweyecol = dsa_heros_get_eye_col(hero);

	IupSetStrAttribute(eye_list, "VALUESTRING", neweyecol); 

	DEBUG_LOG_ARGS("New rnd eye: %s\n", neweyecol);

	xmlFree(neweyecol);
}


#if 0
//############################################################################################################################
//EOF private SEction
//############################################################################################################################
#endif

Ihandle* hgen_hero_sheet_new(dsa_heros_t *heros, dsa_hero_t *hero) {
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
	IupSetAttribute(lst_breed, "hero", (void*)hero);
	IupSetAttribute(lst_breed, "heros", (void*)heros);
	IupSetCallback(lst_breed, "ACTION", (Icallback)__hgen_on_change_breed);

	Ihandle *btn_edt_breed = IupButton("<e>",NULL);
	IupSetCallback(btn_edt_breed,"ACTION",(Icallback)default_action_callback);

	Ihandle *lbl_culture = IupLabel("Culture:");
	
	Ihandle *lst_culture = IupList(NULL);
	IupSetAttributes(lst_culture, "RASTERSIZE=125, DROPDOWN=YES, VISIBLEITEMS=15");
	IupSetAttribute(lst_culture, "hero", (void*)hero);
	IupSetAttribute(lst_culture, "heros", (void*)heros);
	IupSetCallback(lst_culture, "ACTION", (Icallback)__hgen_on_change_culture);

	Ihandle *btn_edt_culture = IupButton("<e>",NULL);
	IupSetCallback(btn_edt_culture,"ACTION",(Icallback)default_action_callback);

	Ihandle *lbl_prof = IupLabel("Profession:");
	
	Ihandle *lst_prof = IupList(NULL);
	IupSetAttributes(lst_prof, "RASTERSIZE=125, DROPDOWN=YES, VISIBLEITEMS=15");
	IupSetAttribute(lst_prof, "hero", (void*)hero);
	IupSetAttribute(lst_prof, "heros", (void*)heros);
	IupSetCallback(lst_prof, "ACTION", (Icallback)__hgen_on_change_profession);

	Ihandle *btn_edt_prof = IupButton("<e>",NULL);
	IupSetCallback(btn_edt_prof,"ACTION",(Icallback)default_action_callback);

	Ihandle *lbl_height = IupLabel("Height:");

	Ihandle *txt_height = IupText(NULL);
	IupSetAttributes(txt_height,"RASTERSIZE=125, MULTILINE=NO, SPIN=yes, SPININC=1, SPINAUTO=no");
    IupSetAttribute(txt_height, "hero", (void*)hero);
	IupSetCallback(txt_height,"SPIN_CB",(Icallback)__hgen_on_change_height);

	Ihandle *btn_rnd_height = IupButton("<?>",NULL);
	IupSetAttribute(btn_rnd_height, "height", (void*)txt_height);	
	IupSetCallback(btn_rnd_height,"ACTION",(Icallback)__hgen_on_change_height_dice);

	Ihandle *lbl_weight = IupLabel("Weight:");
	Ihandle *lbl_weight_value = IupLabel("-");
	IupSetAttribute(txt_height, "weight", (void*)lbl_weight_value);

	Ihandle *lbl_hair_col = IupLabel("Haircolor:");
	
	Ihandle *lst_hair_col = IupList(NULL);
	IupSetAttributes(lst_hair_col, "RASTERSIZE=125, DROPDOWN=YES, VISIBLEITEMS=15");
	IupSetAttribute(lst_hair_col, "hero", (void*)hero);
	IupSetCallback(lst_hair_col, "ACTION", (Icallback)__hgen_on_change_hair_col);

	Ihandle *btn_edt_hair_col = IupButton("<?>",NULL);
	IupSetCallback(btn_edt_hair_col,"ACTION",(Icallback)__hgen_on_change_hair_col_dice);
	IupSetAttribute(btn_edt_hair_col, "hero", (void*)hero);
	IupSetAttribute(btn_edt_hair_col, "hair_list", (void*)lst_hair_col);

	Ihandle *lbl_eye_col = IupLabel("Eyecolor:");
	
	Ihandle *lst_eye_col = IupList(NULL);
	IupSetAttributes(lst_eye_col, "RASTERSIZE=125, DROPDOWN=YES, VISIBLEITEMS=15");
	IupSetAttribute(lst_eye_col, "hero", (void*)hero);
	IupSetCallback(lst_eye_col, "ACTION", (Icallback)__hgen_on_change_eye_col);

	Ihandle *btn_edt_eye_col = IupButton("<?>",NULL);
	IupSetCallback(btn_edt_eye_col,"ACTION",(Icallback)__hgen_on_change_eye_col_dice);
	IupSetAttribute(btn_edt_eye_col, "hero", (void*)hero);
	IupSetAttribute(btn_edt_eye_col, "eye_list", (void*)lst_eye_col);

	IupSetAttribute(lst_breed, "eye_list", (void*)lst_eye_col);
	IupSetAttribute(lst_breed, "hair_list", (void*)lst_hair_col);


	Ihandle *gbox = IupGridBox(
		lbl_name, txt_name, btn_rnd_name,
		lbl_breed, lst_breed, btn_edt_breed,
		lbl_culture, lst_culture, btn_edt_culture,
		lbl_prof, lst_prof, btn_edt_prof,
		lbl_height, txt_height, btn_rnd_height,
		lbl_weight, lbl_weight_value, IupLabel(NULL),
		lbl_hair_col, lst_hair_col, btn_edt_hair_col,
		lbl_eye_col, lst_eye_col, btn_edt_eye_col,
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