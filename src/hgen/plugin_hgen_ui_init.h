#ifndef PLUGIN_HGEN_UI_INIT_H
#define PLUGIN_HGEN_UI_INIT_H

#include <iup.h>
#include "hero.h"

void init_breeds(Ihandle* ih, dsa_heros_t *heros);
void init_cultures(Ihandle* ih, dsa_heros_t *heros);
void init_professions(Ihandle* ih, dsa_heros_t *heros);

#endif