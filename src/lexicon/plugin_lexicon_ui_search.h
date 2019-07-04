#ifndef PLUGIN_LEXICON_UI_SEARCH_H
#define PLUGIN_LEXICON_UI_SEARCH_H

#include "defs.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <iup.h>
#include <cd.h>
#include <cdiup.h>
#include <wd.h>

#include <libxml/tree.h>
#include <libxml/parser.h>
#include <libxml/xpath.h>
#include <libxml/xpathInternals.h>

#include "plugin.h"
#include "resource.h"
#include "regex_utils.h"
#include "xpath_utils.h"

#include "xml_utils.h"

typedef struct _lexicon_ctx_ {
	Ihandle *frame;
	resource_search_result_t *xml_result;
	xml_ctx_t **ctxs;
} lexicon_ctx_t;

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

void reset_search_selection(lexicon_search_selection_t	*lss);

lexicon_search_selection_t* create_search_selection();

void reset_search_result_selection(lexicon_search_result_selection_t	*lsrs);

lexicon_search_result_selection_t* create_search_result_selection(unsigned int cnt_file_cache);

void update_cat_and_group_selections();

void add_node_attrs_to_handle(Ihandle *handle, xmlXPathObjectPtr xpathObj);

void update_group_list();

char* add_node_as_string(Ihandle *text, xmlNodePtr node);

void update_result_display(int sel_list_idx);

void refresh_search_result_list();

void search();

#endif