#include "plugin_hgen_ui_utils.h"

static void __init_raw_by_xpath_res(Ihandle* ih, dsa_heros_t *heros, xmlXPathObjectPtr (*nodefunc)(dsa_heros_t *heros) ) {
	
	if ( ih != NULL && heros != NULL && nodefunc != NULL ) {
		
		IupSetStrAttribute(ih, "APPENDITEM", "--- please choose ---");

		xmlXPathObjectPtr _nodes = nodefunc(heros);

		if ( xml_xpath_has_result(_nodes) ) {

			const int maxEntries = _nodes->nodesetval->nodeNr;
			xmlNodePtr *nodes = _nodes->nodesetval->nodeTab;

			for ( int cntentry = 0; cntentry < maxEntries; ++cntentry ) {
				xmlChar *name = xmlGetProp(nodes[cntentry], (xmlChar*)"name");

				IupSetStrAttribute(ih, "APPENDITEM", name);

				xmlFree(name);
			}

		}

		IupSetStrAttribute(ih, "VALUE", "1");
		
		xmlXPathFreeObject(_nodes);
	}

}

static void __init_raw_hero_by_xpath_res(Ihandle* ih, dsa_hero_t *hero, xmlXPathObjectPtr (*nodefunc)(dsa_hero_t *hero) ) {
	
	DEBUG_LOG("try refresh iup list\n");

	if ( ih != NULL && hero != NULL && nodefunc != NULL ) {
		
		IupSetStrAttribute(ih, "APPENDITEM", "--- please choose ---");

		xmlXPathObjectPtr _nodes = nodefunc(hero);

		DEBUG_LOG("try finding  entry\n");

		if ( xml_xpath_has_result(_nodes) ) {

			const int maxEntries = _nodes->nodesetval->nodeNr;
			xmlNodePtr *nodes = _nodes->nodesetval->nodeTab;

			for ( int cntentry = 0; cntentry < maxEntries; ++cntentry ) {
				xmlChar *name = xmlGetProp(nodes[cntentry], (xmlChar*)"name");
				
				DEBUG_LOG_ARGS("found entry: %s\n", name);

				IupSetStrAttribute(ih, "APPENDITEM", name);

				xmlFree(name);
			}

		}

		IupSetStrAttribute(ih, "VALUE", "1");
		
		xmlXPathFreeObject(_nodes);
	}

}

#if 0

//##############################################################################################################
//EOF private section
//##############################################################################################################

#endif


void init_breeds(Ihandle* ih, dsa_heros_t *heros) {

	__init_raw_by_xpath_res(ih, heros, dsa_heros_get_breeds);

}

void init_cultures(Ihandle* ih, dsa_heros_t *heros) {
	
	__init_raw_by_xpath_res(ih, heros, dsa_heros_get_cultures);

}

void init_professions(Ihandle* ih, dsa_heros_t *heros) {

	__init_raw_by_xpath_res(ih, heros, dsa_heros_get_professions);

}

void init_hair_colors(Ihandle* ih, dsa_hero_t *hero) {

	__init_raw_hero_by_xpath_res(ih, hero, dsa_heros_get_hair_colors);

}

void init_eye_colors(Ihandle* ih, dsa_hero_t *hero) {

	__init_raw_hero_by_xpath_res(ih, hero, dsa_heros_get_eye_colors);

}

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