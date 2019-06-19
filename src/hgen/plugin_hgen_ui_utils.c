#include "plugin_hgen_ui_utils.h"

char* hgen_hero_name_id(dsa_hero_t *hero) {

    xmlChar * xmlhaname = dsa_heros_get_name(hero);
    xmlChar * xmlhid = dsa_heros_get_id(hero);

    char * hero_name = format_string_new("%s (%s)", xmlhaname, xmlhid);

	xmlFree(xmlhaname);
    xmlFree(xmlhid);

    return hero_name;
}

void hgen_change_hero_name(dsa_hero_t *hero, const char* newname) {
    dsa_heros_set_name(hero, (const unsigned char*)newname);
    //change list entry,
    hgen_ctx_t * mctx = (hgen_ctx_t *)IupGetGlobal("hero_ctx");
    char * pos = IupGetAttribute(mctx->ctrls.hero_list, "VALUE");
    IupSetStrAttribute(mctx->ctrls.hero_list, pos, newname);
    IupSetStrAttribute(mctx->ctrls.hero_list, "VALUE", pos);
    
    int tabpos = IupGetInt(mctx->ctrls.hero_tabs, "VALUEPOS");
    char * hero_name = hgen_hero_name_id(hero);
    IupSetStrAttributeId(mctx->ctrls.hero_tabs, "TABTITLE", tabpos, hero_name);

    IupSetStrAttribute(mctx->selected_hero->detail_frame, "hero_name", hero_name);
    //change tabtitle
    free(hero_name);
}