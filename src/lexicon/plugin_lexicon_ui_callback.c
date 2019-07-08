#include "plugin_lexicon_ui_callback.h"

int search_button_callback(Ihandle *search_button) {

	lexicon_ctx_t *lctx = (lexicon_ctx_t*)IupGetAttribute(search_button, "lctx");

	DEBUG_LOG("trigger search\n");

	search(lctx);
	
	return IUP_DEFAULT;
}

int search_input_key_callback(Ihandle *ih, int c) {

	if ( c == K_CR ) {
	
		DEBUG_LOG("hit enter key in search input\n");

		lexicon_ctx_t *lctx = (lexicon_ctx_t*)IupGetAttribute(ih, "lctx");

		search(lctx);
	}
	
	return IUP_DEFAULT;
}

int on_category_changed_cb(Ihandle *ih, char *text, int item, int state) {
	
	(void)text;
	(void)item;

	if ( state == 1 ) {
	
		DEBUG_LOG_ARGS("changed to %s\n", IupGetAttribute(ih, "VALUESTRING"));
		
		lexicon_ctx_t *lctx = (lexicon_ctx_t*)IupGetAttribute(ih, "lctx");

		update_cat_and_group_selections(lctx);
		update_group_list(lctx);
	}
	
	return IUP_DEFAULT;
}

int on_group_changed_cb(Ihandle *ih, char *text, int item, int state) {
	
	(void)text;
	(void)item;

	if ( state == 1 ) {
	
		DEBUG_LOG_ARGS("changed to %s\n", IupGetAttribute(ih, "VALUESTRING"));
		
		lexicon_ctx_t *lctx = (lexicon_ctx_t*)IupGetAttribute(ih, "lctx");

		update_cat_and_group_selections(lctx);
	}
	
	return IUP_DEFAULT;
}

int on_result_list_changed_cb(Ihandle *ih, char *text, int item, int state) {

	(void)text;

	if ( state == 1 ) {
	
		DEBUG_LOG_ARGS("changed to %s or %s\n", IupGetAttribute(ih, "VALUESTRING"), text);
		
		lexicon_ctx_t *lctx = (lexicon_ctx_t*)IupGetAttribute(ih, "lctx");

		update_result_display(lctx, item);
		
	}
	
	return IUP_DEFAULT;
}