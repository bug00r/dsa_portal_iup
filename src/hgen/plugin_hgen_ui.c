#include "plugin_hgen_ui.h"

Ihandle* create_hgen_sheet() {
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

Ihandle* create_hgen_frame() {

	Ihandle *add_hero = IupButton("+", NULL);
	IupSetAttributes(add_hero, "RASTERSIZE=25");
	IupSetCallback(add_hero,"ACTION",(Icallback)hgen_add_new_hero_callback);

	Ihandle *rem_hero = IupButton("-", NULL);
	IupSetAttributes(rem_hero, "RASTERSIZE=25");
	IupSetCallback(rem_hero,"ACTION",(Icallback)default_action_callback);

	Ihandle * btn_list = IupHbox(add_hero, rem_hero, NULL);
	IupSetAttributes(btn_list, "EXPAND=no");

	Ihandle *hero_list = IupList(NULL);
	IupSetAttributes(hero_list, "EXPAND=YES, VISIBLELINES=1");
	IupSetCallback(hero_list,"ACTION",(Icallback)hgen_select_hero_callback);
	
	Ihandle *hero_nav = IupVbox(btn_list, hero_list, NULL);

	Ihandle * home = IupScrollBox(IupVbox(IupLabel("Infos about Hgen"), IupFill(),NULL));
	IupSetAttribute(home, "EXPANDCHILDREN", "yes");
	IupSetAttribute(home, "TABTITLE", "Hgen-News");

	Ihandle * hero_tabs = IupTabs(home, NULL);
	IupSetAttributes(hero_tabs, "SHOWCLOSE=yes, EXPAND=YES");
	IupSetCallback(hero_tabs,"TABCLOSE_CB",(Icallback)do_not_close_first_tab_callback);

	Ihandle * split = IupSplit(hero_nav, hero_tabs);
	IupSetAttributes(split, "MINMAX=100:400, VALUE=150");

	Ihandle* frame = IupHbox(split, NULL);

	IupSetGlobal("hero_list", (void*)hero_list);

	return frame;
}
