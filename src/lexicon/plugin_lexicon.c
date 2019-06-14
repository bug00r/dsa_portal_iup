#include "plugin_lexicon.h"

static void _lexicon_init_(void * data) {

	#if debug > 0
		printf("lexicon init\n");
	#endif

	lexicon_ctx_t * mctx = (lexicon_ctx_t *)data;
	mctx->ctxs  = NULL;
	mctx->frame = NULL;
	
}

static void _lexicon_free_(void * data) {

	free(data);

	#if debug > 0
		printf("lexicon free\n");
	#endif
}

static const char * _lexicon_name_(void * data) {
	
	(void)data;

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
	
		mctx->frame = create_lexicon_frame();
	}
	return mctx->frame;
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
	
	lctx->ctxs = malloc(result->cnt*sizeof(xml_ctx_t *));

	for(unsigned int cnt_files = 0; cnt_files < result->cnt; ++cnt_files) {
	
		resource_file_t *file = result->files[cnt_files];
		
		xml_source_t * xml_src = xml_source_from_resfile(resource_file_copy_deep(file));
		lctx->ctxs[cnt_files] = xml_ctx_new(xml_src);
		
		IupSetAttribute(categories, "APPENDITEM", file->name);
	}
	IupSetAttribute(categories, "VALUE", "1");
	
	lexicon_search_selection_t *lss = create_search_selection();	
	IupSetGlobal("lss", (void *)lss);
	
	lexicon_search_result_selection_t *lsrs = create_search_result_selection(result->cnt);	
	IupSetGlobal("lsrs", (void *)lsrs);
	
	update_cat_and_group_selections();
	update_group_list();
	
}

void _lexicon_cleanup_(void * data) {
	
	#if debug > 0
		printf("lexicon plugin cleanup\n");
	#endif
	
	lexicon_ctx_t *lctx = (lexicon_ctx_t *)data;
	
	if ( lctx->ctxs != NULL ) {
		
		free(IupGetGlobal("lss"));
	
		lexicon_search_result_selection_t *lsrs = (lexicon_search_result_selection_t *)IupGetGlobal("lsrs");
		reset_search_result_selection(lsrs);
		free(lsrs->xpath_result);
		free(lsrs);
		
		unsigned int max_docs =  lctx->xml_result->cnt;
		
		for ( unsigned int cur_doc = 0; cur_doc < max_docs ; ++cur_doc) {
			
			free_xml_ctx_src(&lctx->ctxs[cur_doc]);

		}
	
		free(lctx->ctxs);
		lctx->ctxs = NULL;
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

