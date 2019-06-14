#include "plugin_hgen_ui_callback.h"

void hgen_add_new_hero_callback(Ihandle *ih) {
    
	hgen_ctx_t * mctx = (hgen_ctx_t *)IupGetGlobal("hero_ctx");

    Ihandle * hero_list = (Ihandle *)IupGetGlobal("hero_list");

    const unsigned char* defaultname = "New Hero";

    dsa_hero_t *newhero = dsa_hero_new(mctx->heros, defaultname);
    IupSetStrAttribute(hero_list, "APPENDITEM", (const char*)defaultname);

    int count = IupGetInt(hero_list, "COUNT");

    printf("new hero count: %i\n", count);

    dl_list_append(mctx->nav_heros, newhero);
    
}

int  hgen_select_hero_callback(Ihandle *ih, char *text, int item, int state) {
    int index = item - 1;
    if (state == 1 && index >= 0) {
        hgen_ctx_t * mctx = (hgen_ctx_t *)IupGetGlobal("hero_ctx");
        dsa_hero_t * selected_hero = (dsa_hero_t *)dl_list_get(mctx->nav_heros, index);
        xmlChar * name = dsa_heros_get_name(selected_hero);
        xmlChar * id = dsa_heros_get_id(selected_hero);
        printf("select(%i): %s id: %s\n", item, name, id);
        xmlFree(name);
        xmlFree(id);
    }
}