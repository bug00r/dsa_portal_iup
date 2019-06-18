#include "plugin_hgen_ui.h"

Ihandle* create_hgen_frame() {

	hgen_ctx_t * mctx = (hgen_ctx_t *)IupGetGlobal("hero_ctx");

	Ihandle *add_hero = IupButton("+", NULL);
	IupSetAttributes(add_hero, "RASTERSIZE=25");
	IupSetCallback(add_hero,"ACTION",(Icallback)hgen_add_new_hero_callback);

	Ihandle *rem_hero = IupButton("-", NULL);
	IupSetAttributes(rem_hero, "RASTERSIZE=25, ACTIVE=no");
	IupSetCallback(rem_hero,"ACTION",(Icallback)hgen_rem_sel_hero_callback);
	mctx->ctrls.hero_rem_button = rem_hero;

	Ihandle * btn_list = IupHbox(add_hero, rem_hero, NULL);
	IupSetAttributes(btn_list, "EXPAND=no");

	Ihandle *hero_list = IupList(NULL);
	IupSetAttributes(hero_list, "EXPAND=YES, VISIBLELINES=1");
	IupSetCallback(hero_list,"ACTION",(Icallback)hgen_select_hero_callback);
	mctx->ctrls.hero_list = hero_list;

	Ihandle *hero_nav = IupVbox(btn_list, hero_list, NULL);

	Ihandle * home = IupScrollBox(IupVbox(IupLabel("Infos about Hgen"), IupFill(),NULL));
	IupSetAttribute(home, "EXPANDCHILDREN", "yes");
	IupSetAttribute(home, "TABTITLE", "Hgen-News");

	Ihandle * hero_tabs = IupTabs(home, NULL);
	IupSetAttributes(hero_tabs, "SHOWCLOSE=yes, EXPAND=YES");
	//IupSetCallback(hero_tabs,"TABCLOSE_CB",(Icallback)do_not_close_first_tab_callback);
	IupSetCallback(hero_tabs,"TABCLOSE_CB",(Icallback)hgen_on_close_hero_tab);
	IupSetCallback(hero_tabs,"TABCHANGEPOS_CB",(Icallback)hgen_on_change_hero_tab);

	mctx->ctrls.hero_tabs = hero_tabs;

	Ihandle * split = IupSplit(hero_nav, hero_tabs);
	IupSetAttributes(split, "MINMAX=100:400, VALUE=150");

	Ihandle* frame = IupHbox(split, NULL);

	return frame;
}
