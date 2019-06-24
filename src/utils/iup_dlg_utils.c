#include "iup_dlg_utils.h"

static Ihandle* __iup_dlg_create( const char *dlg_type, const char *title, const char *msg,
                                  const char *btns, Icallback helpcallback) {
    Ihandle* dlg = IupMessageDlg();

    IupSetAttribute(dlg, "DIALOGTYPE", dlg_type);
    IupSetAttribute(dlg, "TITLE", title);
    IupSetAttribute(dlg, "BUTTONS", btns);
    IupSetAttribute(dlg, "VALUE", msg);
    IupSetCallback(dlg, "HELP_CB", (Icallback)helpcallback);

    return dlg;
}

static int __iup_dlg_get_btn_press_state(Ihandle *dlg) {
    int dlg_response = -1;

    if (dlg) {
        IupPopup(dlg, IUP_CURRENT, IUP_CURRENT);
        dlg_response = IupGetInt(dlg, "BUTTONRESPONSE");
        IupDestroy(dlg);
    }

    return dlg_response;
}

bool iup_question_dlg_yn(const char* title, const char *msg) {
    
    Ihandle *dlg = __iup_dlg_create("QUESTION", title, msg, "YESNO", NULL);

    return (__iup_dlg_get_btn_press_state(dlg) == 1);

}