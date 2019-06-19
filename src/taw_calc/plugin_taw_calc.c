#include "plugin_taw_calc.h"

static int on_param_change_cb(Ihandle *ih, char *text, int item, int state) {

	DEBUG_LOG("trigger calc\n");
	
	Ihandle *taw_column 		= IupGetHandle("taw_column");
	int col = IupGetInt(taw_column, "VALUE");
	
	Ihandle *taw_start 			= IupGetHandle("taw_start");
	int start = IupGetInt(taw_start, "VALUE");
	
	Ihandle *taw_end 			= IupGetHandle("taw_end");
	int end = IupGetInt(taw_end, "VALUE");
	
	--col;
	--end;
	
	DEBUG_LOG_ARGS("col: %i, start: %i, end: %i \n", col, start, end);
	
	Ihandle *taw_calc_result 	= IupGetHandle("taw_calc_result");
	
	taw_col_t column = (taw_col_t)col;
	taw_result_t *taw_result = taw_calc(&column, start-4, end-3);

	if(taw_result->complete) {

		IupSetStrf(taw_calc_result, "TITLE", "%i AP", taw_result->complete->ap);

	}
	
	taw_result_free(&taw_result);

	return IUP_DEFAULT;
}

static Ihandle * __create_taw_calc_searchbar()
{	
	Ihandle *column_input = IupList(NULL);
	IupSetAttributes(column_input, "DROPDOWN=YES, EXPAND=HORIZONTAL, VISIBLEITEMS=15, VALUE=1");
	IupSetAttributes(column_input, "1=A*, 2=A, 3=B, 4=C, 5=D, 6=E, 7=F, 8=G, 9=H");
	IupSetCallback(column_input, "ACTION", (Icallback)on_param_change_cb);

	Ihandle *column_label = IupLabel("column:");
	Ihandle *column = IupHbox(column_label, column_input, NULL);
	
	IupSetAttribute(column, "ALIGNMENT", "ACENTER");
	
	Ihandle *taw_start_input = IupList(NULL);
	IupSetAttributes(taw_start_input, "DROPDOWN=YES, EXPAND=HORIZONTAL, VISIBLEITEMS=15, VALUE=1");
	IupSetAttributes(taw_start_input, "1=-3, 2=-2, 3=-1, 4=0, 5=1, 6=2, 7=3, 8=4, 9=5, 10=6, 11=7, 12=8, 13=9, 14=10, 15=11");
	IupSetAttributes(taw_start_input, "16=12, 17=13, 18=14, 19=15, 20=16, 21=17, 22=18, 23=19, 24=20, 25=21, 26=22, 27=23, 28=24, 29=25");
	IupSetAttributes(taw_start_input, "30=26, 31=27, 32=28, 33=29, 34=30, 35=31+");
	IupSetCallback(taw_start_input, "ACTION", (Icallback)on_param_change_cb);

	Ihandle *taw_start_label = IupLabel("taw_start:");
	Ihandle *taw_start = IupHbox(taw_start_label, taw_start_input, NULL);
	IupSetAttribute(taw_start, "ALIGNMENT", "ACENTER");
	
	
	Ihandle *taw_end_input = IupList(NULL);
	IupSetAttributes(taw_end_input, "DROPDOWN=YES, EXPAND=HORIZONTAL, VISIBLEITEMS=15, VALUE=1");
	IupSetAttributes(taw_end_input, "1=-3, 2=-2, 3=-1, 4=0, 5=1, 6=2, 7=3, 8=4, 9=5, 10=6, 11=7, 12=8, 13=9, 14=10, 15=11");
	IupSetAttributes(taw_end_input, "16=12, 17=13, 18=14, 19=15, 20=16, 21=17, 22=18, 23=19, 24=20, 25=21, 26=22, 27=23, 28=24, 29=25");
	IupSetAttributes(taw_end_input, "30=26, 31=27, 32=28, 33=29, 34=30, 35=31+");
	IupSetCallback(taw_end_input, "ACTION", (Icallback)on_param_change_cb);

	Ihandle *taw_end_label = IupLabel("taw_end:");
	Ihandle *taw_end = IupHbox(taw_end_label, taw_end_input, NULL);
	IupSetAttribute(taw_end, "ALIGNMENT", "ACENTER");
	
	Ihandle *searchbar = IupHbox(column, taw_start, taw_end, NULL);
	IupSetAttribute(searchbar, "GAP", "5");
	
	IupSetHandle("taw_column", column_input);
	IupSetHandle("taw_start", taw_start_input);
	IupSetHandle("taw_end", taw_end_input);
	
	return searchbar;
}

static Ihandle * __create_taw_calc_frame()
{
	Ihandle *searchbar = __create_taw_calc_searchbar();
	
	Ihandle *result_text = IupLabel("0 AP");
	IupSetAttributes(result_text, "FONTSIZE=36, EXPAND=YES, ALIGNMENT=ACENTER:ACENTER");
	IupSetHandle("taw_calc_result", result_text);
	
	Ihandle * maindlg = IupVbox(searchbar, IupHbox(result_text, NULL), NULL);
	return maindlg;
}

//###########

static void _taw_calc_init_(void * data) {
	
	DEBUG_LOG("taw_calc init\n");

	#if 0
		/** init context here
			All needed things:
		  */
	#endif
	taw_calc_ctx_t * mctx = (taw_calc_ctx_t *)data;
	mctx->frame=NULL;
}

static void _taw_calc_free_(void * data) {
	free(data);
}

static const char * _taw_calc_name_(void * data) {
	return "TAW-Calculation";
}

void * _taw_calc_frame_(void * data) {
	taw_calc_ctx_t * mctx = (taw_calc_ctx_t *)data;
	if ( mctx->frame == NULL ) {
		mctx->frame = __create_taw_calc_frame();
	}
	return mctx->frame;
}

void _taw_calc_prepare_(void * data) {
}

void _taw_calc_cleanup_(void * data) {
}

plugin_t * taw_calc_plugin(plugin_t * plugin) {
	plugin->name = _taw_calc_name_;
	plugin->frame = _taw_calc_frame_;
	plugin->init = _taw_calc_init_;
	plugin->free = _taw_calc_free_;
	plugin->prepare = _taw_calc_prepare_;
	plugin->cleanup = _taw_calc_cleanup_;
	plugin->data = malloc(sizeof(taw_calc_ctx_t));
	return plugin;
}

