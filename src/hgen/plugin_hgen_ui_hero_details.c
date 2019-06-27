#include "plugin_hgen_ui_hero_details.h"

static int __hgen_on_change_gp(Ihandle *ih, int pos) {
	
	char * new_gp = format_string_new("%i", pos);

	DEBUG_LOG_ARGS("new gp: %s\n", new_gp);

	dsa_hero_t *hero = (dsa_hero_t *)IupGetAttribute(ih, "hero");

	dsa_heros_set_gp(hero, new_gp);

	IupSetInt(ih, "VALUE", pos);
	IupSetInt(ih, "SPINVALUE", pos);

	free(new_gp);

	return IUP_DEFAULT;
}

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

	char * val = IupGetAttribute(height_field, "VALUE");
	xmlChar * base_val = dsa_heros_get_base_height(hero);

	int spin_val = (int)((atof(val) - atof(base_val)) * 100.f);

	DEBUG_LOG_ARGS("spin value height: %i\n", spin_val);

	IupSetInt(height_field, "SPINVALUE", spin_val);

	xmlFree(base_val);
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

	Ihandle *height_txt = (Ihandle *)IupGetAttribute(ih, "height_txt");

	int height_min = dsa_heros_get_height_min(hero);

	DEBUG_LOG_ARGS("height min int: %i\n", height_min);

	IupSetInt(height_txt, "SPINVALUE", height_min);
	char * h_min_chr = format_string_new("%i", height_min);

	DEBUG_LOG_ARGS("height min str: %s\n", h_min_chr);

	dsa_heros_set_height_weight_by_value(hero, (const unsigned char*)h_min_chr);
	__hgen_set_height_raw(height_txt, hero);

	IupSetInt(height_txt, "SPINMIN", height_min);
	IupSetInt(height_txt, "SPINMAX", dsa_heros_get_height_max(hero));

	free(h_min_chr);

	return IUP_DEFAULT;
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


static void __hgen_details_set_baseinfo_gbox_atts(Ihandle *gbox) {

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
}

static Ihandle* __hgen_details_gen_baseinfo(Ihandle *detail_frame, dsa_heros_t *heros, dsa_hero_t *hero) {
	
	Ihandle *lbl_gp = IupLabel("GP:");

	Ihandle *txt_gp = IupText(NULL);
	IupSetAttributes(txt_gp,"RASTERSIZE=125, MULTILINE=NO, SPIN=yes, SPININC=1, SPINAUTO=no, SPINMIN=50, SPINMAX=200");
    IupSetAttribute(txt_gp, "hero", (void*)hero);
	IupSetCallback(txt_gp,"SPIN_CB",(Icallback)__hgen_on_change_gp);

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
	IupSetAttributes(txt_height,"RASTERSIZE=125, MULTILINE=NO, SPIN=yes, SPININC=1, SPINAUTO=no, SPINMAX=300");
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
	IupSetAttribute(lst_breed, "height_txt", (void*)txt_height);

	Ihandle *gbox = IupGridBox(
		lbl_gp, txt_gp, IupLabel(NULL),
		lbl_name, txt_name, btn_rnd_name,
		lbl_breed, lst_breed, btn_edt_breed,
		lbl_culture, lst_culture, btn_edt_culture,
		lbl_prof, lst_prof, btn_edt_prof,
		lbl_height, txt_height, btn_rnd_height,
		lbl_weight, lbl_weight_value, IupLabel(NULL),
		lbl_hair_col, lst_hair_col, btn_edt_hair_col,
		lbl_eye_col, lst_eye_col, btn_edt_eye_col,
	NULL);

	__hgen_details_set_baseinfo_gbox_atts(gbox);

	IupSetAttribute(detail_frame, "culture_list", (void*)lst_culture);
    IupSetAttribute(detail_frame, "breed_list", (void*)lst_breed);
    IupSetAttribute(detail_frame, "prof_list", (void*)lst_prof);
    IupSetAttribute(detail_frame, "name_txt", (void*)txt_name);
	IupSetAttribute(detail_frame, "gp_txt", (void*)txt_gp);

	return gbox;
}

static int __hgen_on_change_attr(Ihandle *ih, int pos) {
	
	int old_val = IupGetInt(ih, "VALUE");

	DEBUG_LOG_ARGS("attr: old %i new %i\n", old_val, pos);

	if ( old_val != pos ) {
		
		dsa_hero_t *hero = (dsa_hero_t *)IupGetAttribute(ih, "hero");
		
		const unsigned char* short_name = (const unsigned char*)IupGetAttribute(ih, "short_name");
		
		if ( (pos - old_val) > 0 ) {
			dsa_heros_attr_inc(hero, short_name);
		} else {
			dsa_heros_attr_dec(hero, short_name);
		}

		xmlChar * real = dsa_heros_get_attr(hero, short_name);

		DEBUG_LOG_ARGS("attr %s after edit %s\n", short_name, real);

		IupSetStrAttribute(ih, "VALUE", real);

		xmlFree(real);
	}

	return IUP_DEFAULT;
}

static Ihandle* __hgen_details_gen_attr_field(dsa_hero_t *hero, const char* short_name) {
	Ihandle *field = IupText(NULL);
	IupSetAttributes(field,"RASTERSIZE=40, MULTILINE=NO, SPIN=yes, SPINAUTO=no");
    IupSetAttribute(field, "hero", (void*)hero);
	IupSetAttribute(field, "short_name", (void*)short_name);
	IupSetCallback(field,"SPIN_CB",(Icallback)__hgen_on_change_attr);
	return field;
}

static Ihandle* __hgen_details_gen_properties(Ihandle *detail_frame, dsa_heros_t *heros, dsa_hero_t *hero) {
	
	Ihandle *lbl_mu = IupLabel("MU:");
	Ihandle *txt_mu = __hgen_details_gen_attr_field(hero, "MU");
	IupSetAttribute(detail_frame, "txt_mu", (void*)txt_mu);

	Ihandle *lbl_kl = IupLabel("KL:");
	Ihandle *txt_kl = __hgen_details_gen_attr_field(hero, "KL");
	IupSetAttribute(detail_frame, "txt_kl", (void*)txt_kl);

	Ihandle *lbl_kk = IupLabel("KK:");
	Ihandle *txt_kk = __hgen_details_gen_attr_field(hero, "KK");
	IupSetAttribute(detail_frame, "txt_kk", (void*)txt_kk);

	Ihandle *lbl_ge = IupLabel("GE:");
	Ihandle *txt_ge = __hgen_details_gen_attr_field(hero, "GE");
	IupSetAttribute(detail_frame, "txt_ge", (void*)txt_ge);

	Ihandle *lbl_ff = IupLabel("FF:");
	Ihandle *txt_ff = __hgen_details_gen_attr_field(hero, "FF");
	IupSetAttribute(detail_frame, "txt_ff", (void*)txt_ff);

	Ihandle *lbl_in = IupLabel("IN:");
	Ihandle *txt_in = __hgen_details_gen_attr_field(hero, "IN");
	IupSetAttribute(detail_frame, "txt_in", (void*)txt_in);

	Ihandle *lbl_ko = IupLabel("KO:");
	Ihandle *txt_ko = __hgen_details_gen_attr_field(hero, "KO");
	IupSetAttribute(detail_frame, "txt_ko", (void*)txt_ko);

	Ihandle *lbl_ch = IupLabel("CH:");
	Ihandle *txt_ch = __hgen_details_gen_attr_field(hero, "CH");
	IupSetAttribute(detail_frame, "txt_ch", (void*)txt_ch);

	Ihandle *lbl_so = IupLabel("SO:");
	Ihandle *txt_so = __hgen_details_gen_attr_field(hero, "SO");
	IupSetAttribute(detail_frame, "txt_so", (void*)txt_so);

	Ihandle *gbox = IupGridBox(
		lbl_mu, txt_mu,
		lbl_kl, txt_kl,
		lbl_in, txt_in,
		lbl_ch, txt_ch,
		lbl_ff, txt_ff,
		lbl_ge, txt_ge,
		lbl_ko, txt_ko,
		lbl_kk, txt_kk,
		lbl_so, txt_so,
	NULL);

	__hgen_details_set_baseinfo_gbox_atts(gbox);

	IupSetAttribute(gbox,"NUMDIV","2");
	IupSetAttribute(gbox, "GAPLIN", "6");

	return gbox;
}

static void __hgen_details_fill_prop(Ihandle *sheet, dsa_hero_t *hero, const char *handle_name, const unsigned char *short_name) {
	xmlChar * val = dsa_heros_get_attr(hero, short_name);
	IupSetStrAttribute((Ihandle *)IupGetAttribute(sheet, handle_name), "VALUE", (const char*) val);
    IupSetStrAttribute((Ihandle *)IupGetAttribute(sheet, handle_name), "SPINVALUE", (const char*) val);
    xmlFree(val);
}

#if 0
//############################################################################################################################
//EOF private SEction
//############################################################################################################################
#endif

Ihandle* hgen_hero_sheet_new(dsa_heros_t *heros, dsa_hero_t *hero) {

	Ihandle *detail_frame = IupVbox(NULL);

	Ihandle *bi_basic = __hgen_details_gen_baseinfo(detail_frame, heros, hero);

	Ihandle *bi_props = __hgen_details_gen_properties(detail_frame, heros, hero);

	Ihandle *baseinfo = IupHbox(bi_basic, bi_props, NULL);

    IupAppend(detail_frame, baseinfo);

	return detail_frame;
}

void hgen_hero_sheet_init(Ihandle *sheet, dsa_heros_t *heros, dsa_hero_t *hero) {

    xmlChar * val = dsa_heros_get_name(hero);
    IupSetStrAttribute((Ihandle *)IupGetAttribute(sheet, "name_txt"), "VALUE", (const char*) val);
    xmlFree(val);

	val = dsa_heros_get_gp(hero);
	IupSetStrAttribute((Ihandle *)IupGetAttribute(sheet, "gp_txt"), "VALUE", (const char*) val);
    IupSetStrAttribute((Ihandle *)IupGetAttribute(sheet, "gp_txt"), "SPINVALUE", (const char*) val);
    xmlFree(val);

	__hgen_details_fill_prop(sheet, hero, "txt_mu", (const unsigned char*)"MU");
	__hgen_details_fill_prop(sheet, hero, "txt_kl", (const unsigned char*)"KL");
	__hgen_details_fill_prop(sheet, hero, "txt_in", (const unsigned char*)"IN");
	__hgen_details_fill_prop(sheet, hero, "txt_ch", (const unsigned char*)"CH");
	__hgen_details_fill_prop(sheet, hero, "txt_ff", (const unsigned char*)"FF");
	__hgen_details_fill_prop(sheet, hero, "txt_ge", (const unsigned char*)"GE");
	__hgen_details_fill_prop(sheet, hero, "txt_ko", (const unsigned char*)"KO");
	__hgen_details_fill_prop(sheet, hero, "txt_kk", (const unsigned char*)"KK");
	__hgen_details_fill_prop(sheet, hero, "txt_so", (const unsigned char*)"SO");

    init_breeds((Ihandle *)IupGetAttribute(sheet, "breed_list"), heros);
	init_cultures((Ihandle *)IupGetAttribute(sheet, "culture_list"), heros);
	init_professions((Ihandle *)IupGetAttribute(sheet, "prof_list"), heros);

}