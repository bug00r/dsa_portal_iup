#include "plugin_hgen_ui_init.h"

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