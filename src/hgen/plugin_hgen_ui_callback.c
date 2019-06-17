#include "plugin_hgen_ui_callback.h"

void hgen_add_new_hero_callback(Ihandle *ih) {
    
	hgen_ctx_t * mctx = (hgen_ctx_t *)IupGetGlobal("hero_ctx");

    Ihandle * hero_list = mctx->ctrls.hero_list;

    const unsigned char* defaultname = "New Hero";

    dsa_hero_t *newhero = dsa_hero_new(mctx->heros, defaultname);
    
    IupSetStrAttribute(hero_list, "APPENDITEM", (const char*)defaultname);

    hero_nav_item_t *hnavitem = malloc(sizeof(hero_nav_item_t));
    hnavitem->hero = newhero;
    hnavitem->detail_frame = NULL;

    dl_list_append(mctx->nav_heros, hnavitem);
    
    hgen_check_refresh_rem_hero_btn(mctx);

    int selection = IupGetInt(hero_list, "COUNT");
    IupSetInt(hero_list, "VALUE", selection);

    mctx->selected_hero = hnavitem;
    mctx->selected_list_pos = selection;

    create_andor_open_hero(mctx);

}

void hgen_rem_sel_hero_callback(Ihandle *ih) {
    
	hgen_ctx_t * mctx = (hgen_ctx_t *)IupGetGlobal("hero_ctx");

    Ihandle * hero_list = mctx->ctrls.hero_list;

    xmlChar * id = dsa_heros_get_id(mctx->selected_hero->hero);
    const int hero_id = atoi((const char*)id);
    dsa_heros_delete_hero(mctx->heros, hero_id);
    xmlFree(id);
    
    IupSetInt(hero_list, "REMOVEITEM", mctx->selected_list_pos);
    
    hero_nav_item_t *removed_hero = dl_list_remove(mctx->nav_heros, mctx->selected_list_pos - 1);

    Ihandle *olddetail_frame = removed_hero->detail_frame;

    dsa_hero_free(&removed_hero->hero);
    free(removed_hero);

    mctx->selected_hero = NULL;
    mctx->selected_list_pos = -1;

    //TODO REMOVE Tab and destroy Ihandle  => olddetail_frame
    //manually found childindex by iteration an compare by name. extracting utility function "tabindex by tab title" 
    //replace in give other main and hgen
    //closing found index and destroy Ihandle => 1. IupUnmap 2.IupDetach 3.IupDestroy

    hgen_check_refresh_rem_hero_btn(mctx);

}

int  hgen_select_hero_callback(Ihandle *ih, char *text, int item, int state) {

     int index = item - 1;

    if ( state == 1 && index >= 0) {
        hgen_ctx_t * mctx = (hgen_ctx_t *)IupGetGlobal("hero_ctx");

        mctx->selected_hero = (hero_nav_item_t *)dl_list_get(mctx->nav_heros, index);
        mctx->selected_list_pos = item;

        if (mctx->selected_hero != NULL) {
            dsa_heros_save_hero(mctx->heros, mctx->selected_hero->hero);
        }

        hgen_check_refresh_rem_hero_btn(mctx);
    }
}

int  hgen_show_hero_callback(Ihandle *ih, int item, char *text) {
    int index = item - 1;
    if (/* state == 1 && */index >= 0) {

        hgen_ctx_t * mctx = (hgen_ctx_t *)IupGetGlobal("hero_ctx");
        create_andor_open_hero(mctx);

    }
}