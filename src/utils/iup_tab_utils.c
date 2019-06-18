#include "iup_tab_utils.h"

int iup_tap_index_by_title(Ihandle *tabs, const char *tabtitle) {
    
    const int maxCnt = IupGetChildCount(tabs);
	bool found = false;
    int i = 0;
	for (;i < maxCnt; i++) {
		
		Ihandle *child = IupGetChild(tabs, i);
		
		if ( child ) {
		
			char * tabname = IupGetAttribute(child,"TABTITLE");
			
			bool exist = (strcmp(tabname, tabtitle) == 0);
		
			if ( exist ) {
                found = true;
                break;
            }
				
		}
	
	}

    return (found ? i : -1);
}

void iup_tab_remove_by_child(Ihandle *tabs, Ihandle *child) {
    
    int tab_pos = IupGetChildPos(tabs, child);
    IupSetAttributeId(tabs, "TABVISIBLE", tab_pos, "no");
    
    IupUnmap(child);
    IupDetach(child);
    IupDestroy(child);

}

void iup_tab_remove_by_title(Ihandle *tabs, const char *tabtitle) {
    int child_index = iup_tap_index_by_title(tabs, tabtitle);
    if (child_index >= 0) {
        Ihandle *child = IupGetChild(tabs, child_index);
        iup_tab_remove_by_child(tabs, child);
    }
}