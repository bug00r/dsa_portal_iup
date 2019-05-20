#include "plugin_lexicon_ui.h"

Ihandle * create_lexicon_searchbar()
{
	Ihandle *search_input = IupText(NULL);
	IupSetCallback(search_input, "K_ANY", (Icallback)search_input_key_callback);
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
	IupSetCallback(search_button, "ACTION", (Icallback)search_button_callback);
	IupSetAttributes(search_button, "ALIGNMENT=ACENTER, EXPAND=HORIZONTAL");
	
	Ihandle *searchbar = IupHbox(search, category,group, search_button, NULL);
	IupSetAttribute(searchbar, "GAP", "5");
	
	IupSetHandle("categories", category_input);
	IupSetHandle("groups", group_input);
	IupSetHandle("search_input", search_input);
	
	return searchbar;
}

Ihandle* create_lexicon_result_frame() {
	
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

Ihandle* create_lexicon_frame() {
	
	Ihandle *searchbar = create_lexicon_searchbar();
	
	Ihandle *result_frame = create_lexicon_result_frame();
	
	Ihandle *maindlg = IupVbox( searchbar, result_frame, NULL);
	IupSetAttributes(maindlg, "GAP=5, MARGIN=2x2");
	
	return maindlg;

}