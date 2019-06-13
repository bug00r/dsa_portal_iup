#include "plugin_hgen_ui_callback.h"

void hgen_add_new_hero_callback(Ihandle *ih) {
    Ihandle * hero_list = (Ihandle *)IupGetGlobal("hero_list");
    IupSetAttribute(hero_list, "APPENDITEM", "Baradon\0 12");
}

int  hgen_select_hero_callback(Ihandle *ih, char *text, int item, int state) {
    size_t slen = strlen(text);
    printf("select: %s id: %s\n", text, text+(slen + 2));
}