#ifndef PLUGIN_UI_LEXICON_H
#define PLUGIN_UI_LEXICON_H

#include "dsa_core_defs.h"

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

#include "plugin_lexicon_ui_search.h"
#include "plugin_lexicon_ui_callback.h"
#include "plugin_lexicon_ui.h"

plugin_t * lexicon_plugin(plugin_t * plugin);

#endif