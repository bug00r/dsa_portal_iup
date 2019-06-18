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

    iup_tab_remove_by_child(mctx->ctrls.hero_tabs, olddetail_frame);

    hgen_check_refresh_rem_hero_btn(mctx);

}

int  hgen_select_hero_callback(Ihandle *ih, char *text, int item, int state) {

    int index = item - 1;

    #if debug > 0
        printf("state: %i index: %i \n", state, index);
    #endif 

    if ( state == 1 && index >= 0) {
        hgen_ctx_t * mctx = (hgen_ctx_t *)IupGetGlobal("hero_ctx");

        mctx->selected_hero = (hero_nav_item_t *)dl_list_get(mctx->nav_heros, index);
        mctx->selected_list_pos = item;

        if (mctx->selected_hero != NULL) {
            dsa_heros_save_hero(mctx->heros, mctx->selected_hero->hero);
        }

        hgen_check_refresh_rem_hero_btn(mctx);

        create_andor_open_hero(mctx);
    }
}

int hgen_on_close_hero_tab(Ihandle* ih, int pos) {
    int _return = IUP_DEFAULT;

    if ( pos == 0 ) {
        _return = IUP_IGNORE;
    } else {    
        hgen_ctx_t * mctx = (hgen_ctx_t *)IupGetGlobal("hero_ctx");
        IupSetAttribute(mctx->ctrls.hero_list, "VALUE", "0");
        mctx->selected_hero = NULL;
        mctx->selected_list_pos = -1;
        hgen_check_refresh_rem_hero_btn(mctx);
    }

    return _return;
}

/*typedef struct {
	dsa_hero_t *hero;
	Ihandle * detail_frame;
} hero_nav_item_t; */
static bool __hgen_search_nav_hero_tabtitle(void *item, void* searchdata) {
    char *hero_id = (char*)searchdata;
    hero_nav_item_t *nav_item = (hero_nav_item_t *)item;

    const char * tabtitle = IupGetAttribute(nav_item->detail_frame, "hero_name");

    return (strcmp(tabtitle, hero_id) == 0);
}

int hgen_on_change_hero_tab(Ihandle* ih, int new_pos, int old_pos) {
    
    hgen_ctx_t * mctx = (hgen_ctx_t *)IupGetGlobal("hero_ctx");

    if (new_pos > 0) {
        Ihandle *child = IupGetChild(ih, new_pos); 
        char * tab_title = IupGetAttribute(child, "TABTITLE");
        
        dl_list_t * nav = mctx->nav_heros;
        int id = hgen_search_once_id(nav, (void*)tab_title, __hgen_search_nav_hero_tabtitle);
        if (id >= 0) {
            IupSetInt(mctx->ctrls.hero_list, "VALUE", id+1);
            mctx->selected_hero = (hero_nav_item_t *)dl_list_get(nav, id);
            mctx->selected_list_pos = id+1;
            hgen_check_refresh_rem_hero_btn(mctx);
        } 
    } else {

        IupSetInt(mctx->ctrls.hero_list, "VALUE", 0);
        mctx->selected_hero = NULL;
        mctx->selected_list_pos = -1;
        hgen_check_refresh_rem_hero_btn(mctx);
        
    }

    return IUP_DEFAULT;
}