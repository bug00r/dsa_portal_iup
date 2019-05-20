#include "plugin_ui_lexicon_callback.h"

int search_button_callback(Ihandle *search_button) {

	(void)search_button;

	#if debug > 0
		printf("trigger search\n");
	#endif

	search();
	
	return IUP_DEFAULT;
}

int search_input_key_callback(Ihandle *ih, int c) {

	(void)ih;

	if ( c == K_CR ) {
	
		#if debug > 0
			printf("hit enter key in search input\n");
		#endif
	
		search();
	}
	
	return IUP_DEFAULT;
}

int on_category_changed_cb(Ihandle *ih, char *text, int item, int state) {
	
	(void)ih;
	(void)text;
	(void)item;

	if ( state == 1 ) {
	
		#if debug > 0
			printf("changed to %s\n", IupGetAttribute(ih, "VALUESTRING"));
		#endif
		
		update_cat_and_group_selections();
		update_group_list();
	}
	
	return IUP_DEFAULT;
}

int on_group_changed_cb(Ihandle *ih, char *text, int item, int state) {
	
	(void)ih;
	(void)text;
	(void)item;

	if ( state == 1 ) {
	
		#if debug > 0
			printf("changed to %s\n", IupGetAttribute(ih, "VALUESTRING"));
		#endif
		
		update_cat_and_group_selections();
	}
	
	return IUP_DEFAULT;
}

int on_result_list_changed_cb(Ihandle *ih, char *text, int item, int state) {
	
	(void)ih;
	(void)text;

	if ( state == 1 ) {
	
		#if debug > 0
			printf("changed to %s or %s\n", IupGetAttribute(ih, "VALUESTRING"), text);
		#endif
		
		update_result_display(item);
		
	}
	
	return IUP_DEFAULT;
}