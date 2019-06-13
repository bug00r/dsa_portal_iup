#include "plugin_hgen_ui_callback.h"

void default_action_callback(Ihandle* ih) {
	IupMessage(IupGetAttribute(ih, "TITLE"), "Not implement yet :(");
}

int do_not_close_first_tab_callback(Ihandle* ih, int pos) {
    
    int _return = IUP_DEFAULT;

    if ( pos == 0 ) {
        _return = IUP_IGNORE;
    }

    return _return;
}