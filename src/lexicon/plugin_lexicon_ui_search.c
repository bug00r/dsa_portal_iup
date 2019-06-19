#include "plugin_lexicon_ui_search.h"

void update_cat_and_group_selections() {
	
	lexicon_search_selection_t	*lss = (lexicon_search_selection_t	*)IupGetGlobal("lss");
	Ihandle * categories = IupGetHandle("categories");
	Ihandle * groups = IupGetHandle("groups");
	lexicon_ctx_t *lctx = (lexicon_ctx_t *)IupGetGlobal("lctx");
	
	int selectCategory = IupGetInt(categories, "VALUE");
	
	lss->categories.selected = ( selectCategory == 1 ? selectCategory-1 : selectCategory-2 );
	
	int cnt_files = (selectCategory == 1 ? (int)lctx->xml_result->cnt : 1);
	
	lss->categories.cnt_files = cnt_files;
	
	lss->groups.selected = IupGetInt(groups, "VALUE");
	
	
	DEBUG_LOG_ARGS("lss->categories.selected: %i\n", lss->categories.selected);
	DEBUG_LOG_ARGS("lss->categories.cnt_files: %i\n", lss->categories.cnt_files);
	DEBUG_LOG_ARGS("lss->groups.selected: %i\n", lss->groups.selected );

}

void add_node_attrs_to_handle(Ihandle *handle, xmlXPathObjectPtr xpathObj) {
	if ( xpathObj != NULL ) {
		xmlNodeSetPtr nodes = xpathObj->nodesetval;
		int size = (nodes) ? nodes->nodeNr : 0;
		xmlNodePtr cur;
		for(int i = 0; i < size; ++i) {
			cur = nodes->nodeTab[i];
			
			xmlChar *attr = xmlGetProp(cur, "name");
			IupSetStrAttribute(handle, "APPENDITEM", attr);
			xmlFree(attr);
		}
	}
}

void update_group_list() {

	lexicon_search_selection_t	*lss = (lexicon_search_selection_t	*)IupGetGlobal("lss");
	Ihandle * groups = IupGetHandle("groups");
	lexicon_ctx_t *lctx = (lexicon_ctx_t *)IupGetGlobal("lctx");
	
	IupSetAttribute(groups, "REMOVEITEM", "ALL");
	IupSetAttribute(groups, "APPENDITEM", "ALL");
	
	resource_search_result_t *result = lctx->xml_result;
	unsigned int file_offset = lss->categories.selected;
	
	int cnt_files = lss->categories.cnt_files;
	if ( file_offset >= 0 ) {
	
		for( int cur_file = 0; cur_file < cnt_files ; ++cur_file ) {
	
			xml_ctx_t *xml_ctx = lctx->ctxs[file_offset + cur_file];

			if (xml_ctx == NULL) continue;
			
			xmlXPathObjectPtr xpathObj = xml_ctx_xpath(xml_ctx, "//group");
			
			add_node_attrs_to_handle(groups, xpathObj);

			xmlXPathFreeObject(xpathObj);
			
		}
	
	}
	
	IupSetAttribute(groups, "VALUE", "1");
	
}

char* add_node_as_string(Ihandle *text, xmlNodePtr node) {
	
	IupSetStrAttribute(text, "VALUE", "");
	xmlAttr * attr = node->properties;
	
	#if 0
	//void IupSetStrfV (Ihandle* ih, const char* name, const char* format, va_list arglist);
	#endif

	while(attr != NULL) {
		xmlChar *sattr = xmlGetProp(node, attr->name);
		IupSetStrf(text, "APPEND", "%s:\t\t%s", attr->name, sattr);
		xmlFree(sattr);
		attr = attr->next;		
	}
}

void update_result_display(int sel_list_idx) {

	DEBUG_LOG_ARGS("----- sel result idx: %i\n", sel_list_idx);

	if ( sel_list_idx >= 0 ) {
	
		lexicon_search_result_selection_t *lsrs = (lexicon_search_result_selection_t *)IupGetGlobal("lsrs");
		
		xmlXPathObjectPtr *result = lsrs->xpath_result;
		
		int cnt_global_cache = 0;
		
		for ( unsigned int cnt_cache = 0; cnt_cache < lsrs->cnt_cache; ++cnt_cache) {
		
			xmlXPathObjectPtr cur_result = result[cnt_cache];
			
			DEBUG_LOG_ARGS("check result cache %p %i\n", cur_result, cnt_cache);
			
			if ( cur_result != NULL ) {
				xmlNodeSetPtr nodes = cur_result->nodesetval;
				int size = (nodes) ? nodes->nodeNr : 0;
				
				DEBUG_LOG_ARGS("cache exist with %i entries\n", size);
				
				if ( size > 0 ) {
					
					DEBUG_LOG_ARGS("global cache change %i => %i \n", cnt_global_cache, cnt_global_cache + size);
					
					cnt_global_cache += size;
				
					if ( sel_list_idx <= cnt_global_cache ) {
					
						DEBUG_LOG_ARGS("selected item is inside current cache %i < %i\n", sel_list_idx, cnt_global_cache);
					
						int cur_cache_index = sel_list_idx - (cnt_global_cache - size) - 1;
						
						DEBUG_LOG_ARGS("selected item cache index %i = %i - %i - 1\n", cur_cache_index, cnt_global_cache, sel_list_idx);
						
						add_node_as_string(IupGetHandle("result_text"), nodes->nodeTab[cur_cache_index]);
						
						break;
					}
				}
			
			}
			
		}
		
	}
}

void refresh_search_result_list() {
	lexicon_search_result_selection_t *lsrs = (lexicon_search_result_selection_t *)IupGetGlobal("lsrs");
	Ihandle *result_list = IupGetHandle("result_list");
	
	IupSetAttribute(result_list, "REMOVEITEM", "ALL");
	IupRefresh(IupGetParent(result_list));
	
	for( int cur_file = 0; cur_file < lsrs->cnt_cache ; ++cur_file ) {
		
		xmlXPathObjectPtr xpathObj = lsrs->xpath_result[cur_file];
		
		add_node_attrs_to_handle(result_list, xpathObj);
	
	}
	
	IupSetAttribute(result_list, "VALUE", "1");
	
	update_result_display(1);
	
}

void search() {

	lexicon_search_selection_t	*lss = (lexicon_search_selection_t	*)IupGetGlobal("lss");
	lexicon_ctx_t *lctx = (lexicon_ctx_t *)IupGetGlobal("lctx");
	
	resource_search_result_t *result = lctx->xml_result;
	unsigned int file_offset = lss->categories.selected;
	
	int cnt_files = lss->categories.cnt_files;
	if ( file_offset >= 0 ) {
		
		Ihandle * groups = IupGetHandle("groups");
		Ihandle * search_input = IupGetHandle("search_input");
		
		char *selected_group = IupGetAttribute(groups, "VALUESTRING");
		char *search_string = IupGetAttribute(search_input, "VALUE");
		
		lexicon_search_result_selection_t *lsrs = (lexicon_search_result_selection_t *)IupGetGlobal("lsrs");
		reset_search_result_selection(lsrs);
		lsrs->cnt_cache = cnt_files;
		
		for( int cur_file = 0; cur_file < cnt_files ; ++cur_file ) {
			
			#if 0
			//resource_file_t *file = result->files[file_offset + cur_file];
			#endif

			xml_ctx_t *xml_ctx = lctx->ctxs[file_offset + cur_file];

			if (xml_ctx == NULL) continue;
			
			xmlXPathObjectPtr xpathObj = NULL;

			if ( IupGetInt(groups, "VALUE") == 1 ) {

				xpathObj = xml_ctx_xpath_format(xml_ctx, "//group/*[regexmatch(@name,'%s')]", search_string);

			} else {

				xpathObj = xml_ctx_xpath_format(xml_ctx, "//group[@name = '%s']/*[regexmatch(@name,'%s')]", selected_group, search_string);

			}
			
			lsrs->xpath_result[cur_file] = xpathObj;
			
		}
	
	}
	
	refresh_search_result_list();

}


void reset_search_selection(lexicon_search_selection_t	*lss) {
	
	if(lss != NULL) {
		
		lss->categories.selected = -1;
		lss->categories.cnt_files = -1;
		
		lss->groups.selected= -1;
	}
	
}

lexicon_search_selection_t* create_search_selection() {
	
	lexicon_search_selection_t *lss = malloc(sizeof(lexicon_search_selection_t));
	
	reset_search_selection(lss);
	
	return lss;
}

void reset_search_result_selection(lexicon_search_result_selection_t	*lsrs) {
	
	if(lsrs != NULL) {
	
		DEBUG_LOG_ARGS("lsrs (%p) must clean result (%p)\n", lsrs, lsrs->xpath_result);
	
		for ( unsigned int cnt_cache = lsrs->cnt_cache ; cnt_cache--; ) {
			xmlXPathFreeObject(lsrs->xpath_result[cnt_cache]);	
			lsrs->xpath_result[cnt_cache] = NULL;
		}
	}
	
}

lexicon_search_result_selection_t* create_search_result_selection(unsigned int cnt_file_cache) {
	
	lexicon_search_result_selection_t *lsrs = malloc(sizeof(lexicon_search_result_selection_t));
	lsrs->xpath_result = malloc(cnt_file_cache * sizeof(xmlXPathObjectPtr));
	lsrs->cnt_cache = cnt_file_cache;
	
	for ( unsigned int cnt_cache = cnt_file_cache; cnt_file_cache--; ) {
		lsrs->xpath_result[cnt_file_cache] = NULL;
	}
	
	reset_search_result_selection(lsrs);
	
	return lsrs;
}