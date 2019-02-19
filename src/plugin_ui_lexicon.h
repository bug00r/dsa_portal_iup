#ifndef PLUGIN_UI_LEXICON_H
#define PLUGIN_UI_LEXICON_H

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

typedef struct _lexicon_ctx_ {
	Ihandle *frame;
	resource_search_result_t *xml_result;
	xmlDocPtr *docs;
} lexicon_ctx_t;

plugin_t * lexicon_plugin(plugin_t * plugin);

#endif