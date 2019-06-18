#ifndef IUP_TAB_UTILS_H
#define IUP_TAB_UTILS_H

#include <stdbool.h>
#include <string.h>

#include <iup.h>

int iup_tap_index_by_title(Ihandle *tabs, const char *tabtitle);
void iup_tab_remove_by_child(Ihandle *tabs, Ihandle *child);
void iup_tab_remove_by_title(Ihandle *tabs, const char *tabtitle);
#endif