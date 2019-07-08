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

#include "xml_source.h"

#include "xml_utils.h"

#include "iup_xml_builder.h"

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

typedef struct {
	Ihandle *categories;
	Ihandle *groups;
	Ihandle *search_input;
	Ihandle *result_list;
	Ihandle *result_text;
} lexicon_ctrls_t;

typedef struct _lexicon_ctx_ {
	Ihandle *frame;
	resource_search_result_t *xml_result;
	xml_ctx_t **ctxs;
	lexicon_search_selection_t	*lss;
	lexicon_search_result_selection_t *lsrs;
	xml_source_t *ui_res;
	iup_xml_builder_t *builder;
	lexicon_ctrls_t ctrls;
} lexicon_ctx_t;

void reset_search_selection(lexicon_search_selection_t	*lss);

lexicon_search_selection_t* create_search_selection();

void reset_search_result_selection(lexicon_search_result_selection_t	*lsrs);

lexicon_search_result_selection_t* create_search_result_selection(unsigned int cnt_file_cache);

void update_cat_and_group_selections(lexicon_ctx_t *lctx);

void add_node_attrs_to_handle(Ihandle *handle, xmlXPathObjectPtr xpathObj);

void update_group_list(lexicon_ctx_t *lctx);

char* add_node_as_string(Ihandle *text, xmlNodePtr node);

void update_result_display(lexicon_ctx_t *lctx, int sel_list_idx);

void refresh_search_result_list(lexicon_ctx_t *lctx);

void search(lexicon_ctx_t *lctx);

#endif