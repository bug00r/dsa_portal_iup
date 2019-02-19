#include "plugin_ui_lexicon.h"

typedef struct {
	int selected;
	int cnt_files;
} lexicon_category_t;

typedef struct {
	int selected;
} lexicon_group_t;

typedef struct {
	lexicon_category_t 	categories;
	lexicon_group_t		groups;
} lexicon_search_selection_t;

typedef struct {
	int cnt_cache;
	xmlXPathObjectPtr *xpath_result;
} lexicon_search_result_selection_t;

static char * create_string(const char * msg, ...)
{
	va_list vl;
	va_start(vl, msg);
	int buffsize = vsnprintf(NULL, 0, msg, vl);
	va_end(vl);
	buffsize += 1;
	char * buffer = malloc(buffsize);
	va_start(vl, msg);
	vsnprintf(buffer, buffsize, msg, vl);
	va_end( vl);
	return buffer;
}

static void __reset_search_selection(lexicon_search_selection_t	*lss) {
	
	if(lss != NULL) {
		
		lss->categories.selected = -1;
		lss->categories.cnt_files = -1;
		
		lss->groups.selected= -1;
	}
	
}

static lexicon_search_selection_t* __create_search_selection() {
	
	lexicon_search_selection_t *lss = malloc(sizeof(lexicon_search_selection_t));
	
	__reset_search_selection(lss);
	
	return lss;
}

static void __reset_search_result_selection(lexicon_search_result_selection_t	*lsrs) {
	
	if(lsrs != NULL) {
	
		#if debug > 0
			printf("lsrs (%p) must clean result (%p)\n", lsrs, lsrs->xpath_result);
		#endif
	
		for ( unsigned int cnt_cache = lsrs->cnt_cache ; cnt_cache--; ) {
			xmlXPathFreeObject(lsrs->xpath_result[cnt_cache]);	
			lsrs->xpath_result[cnt_cache] = NULL;
		}
	}
	
}

static lexicon_search_result_selection_t* __create_search_result_selection(unsigned int cnt_file_cache) {
	
	lexicon_search_result_selection_t *lsrs = malloc(sizeof(lexicon_search_result_selection_t));
	lsrs->xpath_result = malloc(cnt_file_cache * sizeof(xmlXPathObjectPtr));
	lsrs->cnt_cache = cnt_file_cache;
	
	for ( unsigned int cnt_cache = cnt_file_cache; cnt_file_cache--; ) {
		lsrs->xpath_result[cnt_file_cache] = NULL;
	}
	
	__reset_search_result_selection(lsrs);
	
	return lsrs;
}

static void __update_cat_and_group_selections() {
	
	lexicon_search_selection_t	*lss = (lexicon_search_selection_t	*)IupGetGlobal("lss");
	Ihandle * categories = IupGetHandle("categories");
	Ihandle * groups = IupGetHandle("groups");
	lexicon_ctx_t *lctx = (lexicon_ctx_t *)IupGetGlobal("lctx");
	
	int selectCategory = IupGetInt(categories, "VALUE");
	
	lss->categories.selected = ( selectCategory == 1 ? selectCategory-1 : selectCategory-2 );
	
	int cnt_files = (selectCategory == 1 ? (int)lctx->xml_result->cnt : 1);
	
	lss->categories.cnt_files = cnt_files;
	
	lss->groups.selected = IupGetInt(groups, "VALUE");
	
	#if debug > 0
		printf("lss->categories.selected: %i\n", lss->categories.selected);
		printf("lss->categories.cnt_files: %i\n", lss->categories.cnt_files);
		printf("lss->groups.selected: %i\n", lss->groups.selected );
	#endif

}

static void __update_group_list() {

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
	
			xmlDocPtr doc = lctx->docs[file_offset + cur_file];
			
			if (doc == NULL) continue;
			
			xmlXPathContextPtr xpathCtx = xmlXPathNewContext(doc);
			if ( xpathCtx != NULL ) {
				xmlXPathObjectPtr xpathObj = xmlXPathEvalExpression((const xmlChar*)"//group", xpathCtx);
				
				if ( xpathObj != NULL ) {
					xmlNodeSetPtr nodes = xpathObj->nodesetval;
					int size = (nodes) ? nodes->nodeNr : 0;
					xmlNodePtr cur;
					for(int i = 0; i < size; ++i) {
						cur = nodes->nodeTab[i];
						
						xmlChar *attr = xmlGetProp(cur, "name");
						IupSetStrAttribute(groups, "APPENDITEM", attr);
						xmlFree(attr);
					}
				}
				xmlXPathFreeObject(xpathObj);
			}
			xmlXPathFreeContext(xpathCtx);
			
		}
	
	}
	
	IupSetAttribute(groups, "VALUE", "1");
	
}

static bool __xpath_match_pattern(const char *_pattern, const char *_text) {
	bool found = false;
	
	pcre2_code *re;
	pcre2_match_data *match_data;
	
	PCRE2_SIZE erroffset;
	int errorcode;
	
	PCRE2_SPTR pattern = (PCRE2_SPTR)_pattern;
	re = pcre2_compile(pattern, -1, 0, &errorcode, &erroffset, NULL);
	
	if (re == NULL) {
		PCRE2_UCHAR buffer[120];
		(void)pcre2_get_error_message(errorcode, buffer, 120);
		
		#if debug > 0
			printf("pcre2 compile error: %s\n", buffer);
		#endif

	} else {
		int rc;
		PCRE2_SPTR value = (PCRE2_SPTR)_text;
		match_data = pcre2_match_data_create(20, NULL);
		rc = pcre2_match(re, value, -1, 0, 0, match_data, NULL);
		found = rc > 0;
	}
	
	pcre2_match_data_free(match_data);
	pcre2_code_free(re);
	
	return found;
}

static void	regexmatch_xpath_func(xmlXPathParserContextPtr ctxt, int nargs) {
	if ( nargs != 2 ) return;
	
	xmlChar *regex = xmlXPathPopString(ctxt);
    if (xmlXPathCheckError(ctxt) || (regex == NULL)) {
        return;
    }
	
	xmlChar *text = xmlXPathPopString(ctxt);
    if (xmlXPathCheckError(ctxt) || (text == NULL)) {
		xmlFree(regex);
        return;
    }
		
	xmlXPathReturnBoolean(ctxt, __xpath_match_pattern(regex, text));
	
	xmlFree(regex);
	xmlFree(text);
}


static char* __add_node_as_string(Ihandle *text, xmlNodePtr node) {
	
	IupSetStrAttribute(text, "VALUE", "");
	xmlAttr * attr = node->properties;
	//void IupSetStrfV (Ihandle* ih, const char* name, const char* format, va_list arglist);
	while(attr != NULL) {
		xmlChar *sattr = xmlGetProp(node, attr->name);
		IupSetStrf(text, "APPEND", "%s:\t\t%s", attr->name, sattr);
		xmlFree(sattr);
		attr = attr->next;		
	}
}

static void __update_result_display(int sel_list_idx) {

	#if debug > 0
		printf("----- sel result idx: %i\n", sel_list_idx);
	#endif

	if ( sel_list_idx >= 0 ) {
	
		lexicon_search_result_selection_t *lsrs = (lexicon_search_result_selection_t *)IupGetGlobal("lsrs");
		
		xmlXPathObjectPtr *result = lsrs->xpath_result;
		
		int cnt_global_cache = 0;
		
		for ( unsigned int cnt_cache = 0; cnt_cache < lsrs->cnt_cache; ++cnt_cache) {
		
			xmlXPathObjectPtr cur_result = result[cnt_cache];
			
			#if debug > 0
				printf("check result cache %p %i\n", cur_result, cnt_cache);
			#endif
			
			if ( cur_result != NULL ) {
				xmlNodeSetPtr nodes = cur_result->nodesetval;
				int size = (nodes) ? nodes->nodeNr : 0;
				
				#if debug > 0
					printf("cache exist with %i entries\n", size);
				#endif
				
				if ( size > 0 ) {
					
					#if debug > 0
						printf("global cache change %i => %i \n", cnt_global_cache, cnt_global_cache + size);
					#endif
					
					cnt_global_cache += size;
				
					if ( sel_list_idx <= cnt_global_cache ) {
					
						#if debug > 0
							printf("selected item is inside current cache %i < %i\n", sel_list_idx, cnt_global_cache);
						#endif
					
						int cur_cache_index = sel_list_idx - (cnt_global_cache - size) - 1;
						
						#if debug > 0
							printf("selected item cache index %i = %i - %i - 1\n", cur_cache_index, cnt_global_cache, sel_list_idx);
						#endif
						
						__add_node_as_string(IupGetHandle("result_text"), nodes->nodeTab[cur_cache_index]);
						
						break;
					}
				}
			
			}
			
		}
		
	}
}

static void __refresh_search_result_list() {
	lexicon_search_result_selection_t *lsrs = (lexicon_search_result_selection_t *)IupGetGlobal("lsrs");
	Ihandle *result_list = IupGetHandle("result_list");
	
	IupSetAttribute(result_list, "REMOVEITEM", "ALL");
	IupRefresh(IupGetParent(result_list));
	
	for( int cur_file = 0; cur_file < lsrs->cnt_cache ; ++cur_file ) {
		
		xmlXPathObjectPtr xpathObj = lsrs->xpath_result[cur_file];
		
		if ( xpathObj != NULL ) {
			xmlNodeSetPtr nodes = xpathObj->nodesetval;
			int size = (nodes) ? nodes->nodeNr : 0;
			xmlNodePtr cur;
			for(int i = 0; i < size; ++i) {
				cur = nodes->nodeTab[i];
				
				xmlChar *attr = xmlGetProp(cur, "name");
				IupSetStrAttribute(result_list, "APPENDITEM", attr);
				xmlFree(attr);
			}
		}
	
	}
	
	IupSetAttribute(result_list, "VALUE", "1");
	__update_result_display(1);
	
}

static void __search() {

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
		__reset_search_result_selection(lsrs);
		lsrs->cnt_cache = cnt_files;
		
		for( int cur_file = 0; cur_file < cnt_files ; ++cur_file ) {
	
			//resource_file_t *file = result->files[file_offset + cur_file];
			xmlDocPtr doc = lctx->docs[file_offset + cur_file];
			
			if (doc == NULL) continue;
			
			xmlXPathContextPtr xpathCtx = xmlXPathNewContext(doc);
			xmlXPathRegisterAllFunctions(xpathCtx);
			xmlXPathRegisterFunc(xpathCtx,(const xmlChar *) "regexmatch", regexmatch_xpath_func);
			//xmlXPathRegisterNs(xpathCtx, (xmlChar *)"re", (xmlChar *)"http://exslt.org/regular-expressions");
			
			if ( xpathCtx != NULL ) {
				
				
				char *gen_xpath = NULL;
				if ( IupGetInt(groups, "VALUE") == 1 ) {
					//regex is not working in current version, have to add own regex function
					gen_xpath = create_string("//group/*[regexmatch(@name,'%s')]", search_string);
					//gen_xpath = create_string("//group/*[contains(@name,'%s')]", search_string);
				} else {
					//regex is not working in current version, have to add own regex function
					gen_xpath = create_string("//group[@name = '%s']/*[regexmatch(@name,'%s')]", selected_group, search_string);
					//gen_xpath = create_string("//group[@name = '%s']/*[contains(@name,'%s')]", selected_group, search_string);
				}
				
				#if debug > 0
					printf("used search xpath: %s\n", gen_xpath);
				#endif
				
				xmlXPathObjectPtr xpathObj = xmlXPathEvalExpression((const xmlChar*)gen_xpath, xpathCtx);
				free(gen_xpath);
				
				lsrs->xpath_result[cur_file] = xpathObj;
					
			}
			
			xmlXPathFreeContext(xpathCtx);
			
		}
	
	}
	
	__refresh_search_result_list();

}

static int __search_button_callback(Ihandle *search_button) {

	#if debug > 0
		printf("trigger search\n");
	#endif

	__search();
	
	return IUP_DEFAULT;
}

static int __search_input_key_callback(Ihandle *ih, int c) {

	if ( c == K_CR ) {
	
		#if debug > 0
			printf("hit enter key in search input\n");
		#endif
	
		__search();
	}
	
	return IUP_DEFAULT;
}

static Ihandle * __create_lexicon_searchbar()
{
	Ihandle *search_input = IupText(NULL);
	IupSetCallback(search_input, "K_ANY", (Icallback)__search_input_key_callback);
	IupSetAttribute(search_input, "EXPAND", "HORIZONTAL");
	Ihandle *search_label = IupLabel("Search:");
	Ihandle *search = IupHbox(search_label, search_input, NULL);
	IupSetAttribute(search, "ALIGNMENT", "ACENTER");
	
	Ihandle *category_input = IupList(NULL);
	IupSetAttributes(category_input, "DROPDOWN=YES, EXPAND=HORIZONTAL, VISIBLEITEMS=15");
	
	Ihandle *category_label = IupLabel("Category:");
	Ihandle *category = IupHbox(category_label, category_input, NULL);
	
	IupSetAttribute(category, "ALIGNMENT", "ACENTER");
	
	Ihandle *group_input = IupList(NULL);
	IupSetAttributes(group_input, "DROPDOWN=YES, EXPAND=HORIZONTAL, VISIBLEITEMS=15");
	Ihandle *group_label = IupLabel("Group:");
	Ihandle *group = IupHbox(group_label, group_input, NULL);
	IupSetAttribute(group, "ALIGNMENT", "ACENTER");
	
	Ihandle *search_button = IupButton("search", NULL);
	IupSetCallback(search_button, "ACTION", (Icallback)__search_button_callback);
	IupSetAttributes(search_button, "ALIGNMENT=ACENTER, EXPAND=HORIZONTAL");
	
	Ihandle *searchbar = IupHbox(search, category,group, search_button, NULL);
	IupSetAttribute(searchbar, "GAP", "5");
	
	IupSetHandle("categories", category_input);
	IupSetHandle("groups", group_input);
	IupSetHandle("search_input", search_input);
	
	return searchbar;
}

static Ihandle* __create_lexicon_result_frame() {
	
	Ihandle *result_list = IupList(NULL);
	IupSetAttributes(result_list, "EXPAND=YES, VISIBLELINES=1");
	
	Ihandle *result_text = IupMultiLine(NULL);
	IupSetAttributes(result_text, "READONLY=YES, EXPAND=YES, WORDWRAP=YES");
	
	Ihandle * split = IupSplit(result_list, result_text);
	IupSetAttributes(split, "MINMAX=100:400, VALUE=150");
	
	IupSetHandle("result_list", result_list);
	IupSetHandle("result_text", result_text);
	
	return split;
}

static Ihandle* __create_lexicon_frame() {
	
	Ihandle *searchbar = __create_lexicon_searchbar();
	
	Ihandle *result_frame = __create_lexicon_result_frame();
	
	Ihandle *maindlg = IupVbox( searchbar, result_frame, NULL);
	IupSetAttributes(maindlg, "GAP=5, MARGIN=2x2");
	
	return maindlg;

}

//###########

static void _lexicon_init_(void * data) {

	#if debug > 0
		printf("lexicon init\n");
	#endif

	lexicon_ctx_t * mctx = (lexicon_ctx_t *)data;
	mctx->docs  = NULL;
	mctx->frame = NULL;
	
}

static void _lexicon_free_(void * data) {
	#if debug > 0
		printf("lexicon free\n");
	#endif
}

static const char * _lexicon_name_(void * data) {
	
	#if debug > 0
		printf("lexicon name\n");
	#endif
	
	return "Lexicon";
}

void * _lexicon_frame_(void * data) {

	#if debug > 0
		printf("lexicon frame\n");
	#endif
	
	lexicon_ctx_t * mctx = (lexicon_ctx_t *)data;
	if ( mctx->frame == NULL ) {
		
		#if debug > 0
			printf("lexicon frame create new\n");
		#endif
	
		mctx->frame = __create_lexicon_frame();
	}
	return mctx->frame;
}

static int on_category_changed_cb(Ihandle *ih, char *text, int item, int state) {
	
	if ( state == 1 ) {
	
		#if debug > 0
			printf("changed to %s\n", IupGetAttribute(ih, "VALUESTRING"));
		#endif
		
		__update_cat_and_group_selections();
		__update_group_list();
	}
	
	return IUP_DEFAULT;
}

static int on_group_changed_cb(Ihandle *ih, char *text, int item, int state) {
	
	if ( state == 1 ) {
	
		#if debug > 0
			printf("changed to %s\n", IupGetAttribute(ih, "VALUESTRING"));
		#endif
		
		__update_cat_and_group_selections();
	}
	
	return IUP_DEFAULT;
}

static int on_result_list_changed_cb(Ihandle *ih, char *text, int item, int state) {
	
	if ( state == 1 ) {
	
		#if debug > 0
			printf("changed to %s or %s\n", IupGetAttribute(ih, "VALUESTRING"), text);
		#endif
		
		__update_result_display(item);
		
	}
	
	return IUP_DEFAULT;
}

void _lexicon_prepare_(void * data) {
	
	#if debug > 0
		printf("lexicon prepare\n");
	#endif
	
	IupSetGlobal("lctx", data);
	
	Ihandle *categories = IupGetHandle("categories");
	IupSetCallback(categories, "ACTION",  (Icallback)on_category_changed_cb);
	
	Ihandle *groups = IupGetHandle("groups");
	IupSetCallback(groups, "ACTION",  (Icallback)on_group_changed_cb);
	
	Ihandle *result_list = IupGetHandle("result_list");
	IupSetCallback(result_list, "ACTION",  (Icallback)on_result_list_changed_cb);
	
	IupSetAttribute(categories, "APPENDITEM", "ALL");
	
	lexicon_ctx_t *lctx = (lexicon_ctx_t *)data;
	resource_search_result_t *result = lctx->xml_result;
	
	lctx->docs = malloc(result->cnt*sizeof(xmlDocPtr));
	
	for(unsigned int cnt_files = 0; cnt_files < result->cnt; ++cnt_files) {
	
		resource_file_t *file = result->files[cnt_files];
		
		lctx->docs[cnt_files] = xmlReadMemory(file->data, file->file_size, "noname.xml", NULL, 0);
		
		#if debug > 0
		
			printf("full: %s(%i)\npath: %s(%i)\nfile: %s(%i)\nname: %s(%i)\ntype: %s(%i)\nfile_size: %i\n", file->complete, strlen(file->complete), file->path, strlen(file->path), file->file, strlen(file->file), file->name, strlen(file->name), file->type, strlen(file->type), file->file_size);
		
			if (lctx->docs[cnt_files] == NULL) {
				printf("Failed to parse document\n");
			} else {
				printf("success to parse document %p\n", lctx->docs[cnt_files]);
			}
			
		#endif
		
		IupSetAttribute(categories, "APPENDITEM", file->name);
	}
	IupSetAttribute(categories, "VALUE", "1");
	
	lexicon_search_selection_t *lss = __create_search_selection();	
	IupSetGlobal("lss", (void *)lss);
	
	lexicon_search_result_selection_t *lsrs = __create_search_result_selection(result->cnt);	
	IupSetGlobal("lsrs", (void *)lsrs);
	
	__update_cat_and_group_selections();
	__update_group_list();
	
}

void _lexicon_cleanup_(void * data) {
	
	#if debug > 0
		printf("lexicon plugin cleanup\n");
	#endif
	
	lexicon_ctx_t *lctx = (lexicon_ctx_t *)data;
	
	if ( lctx->docs != NULL ) {
		
		free(IupGetGlobal("lss"));
	
		lexicon_search_result_selection_t *lsrs = (lexicon_search_result_selection_t *)IupGetGlobal("lsrs");
		__reset_search_result_selection(lsrs);
		free(lsrs->xpath_result);
		free(lsrs);
		
		unsigned int max_docs =  lctx->xml_result->cnt;
		
		#if debug > 0
			printf("lctx->docs != NULL \n");
			printf("max_docs: %i\n", max_docs);
		#endif
		
		for ( unsigned int cur_doc = 0; cur_doc < max_docs ; ++cur_doc) {
			
			xmlDocPtr doc = lctx->docs[cur_doc];
			
			#if debug > 0
				printf("doc (%i): %p\n", cur_doc, doc);
			#endif
			
			if ( doc != NULL ) {
				
				#if debug > 0
					printf("try to delete\n");
				#endif
				
				xmlFreeDoc(doc);
			}
		}
		
		free(lctx->docs);
		lctx->docs = NULL;
	
	}
}

/**
	void * data;
	const char * (*name)(void * data);
	void * (*frame)(void * data);
	void (*init)(void * data);
	void (*free)(void * data);
	void (*prepare)(void * data);
	void (*cleanup)(void * data);
*/
plugin_t * lexicon_plugin(plugin_t * plugin) {
	plugin->name = _lexicon_name_;
	plugin->frame = _lexicon_frame_;
	plugin->init = _lexicon_init_;
	plugin->free = _lexicon_free_;
	plugin->prepare = _lexicon_prepare_;
	plugin->cleanup = _lexicon_cleanup_;
	plugin->data = malloc(sizeof(lexicon_ctx_t));
	return plugin;
}

