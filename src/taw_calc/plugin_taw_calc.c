#include "plugin_taw_calc.h"

static int on_param_change_cb(Ihandle *ih, char *text, int item, int state) {

	DEBUG_LOG("trigger calc\n");
	
	Ihandle *taw_column 		= (Ihandle*)IupGetAttribute(ih, "taw_column");//IupGetHandle("taw_column");
	int col = IupGetInt(taw_column, "VALUE");
	
	Ihandle *taw_start 			= (Ihandle*)IupGetAttribute(ih, "taw_start");
	int start = IupGetInt(taw_start, "VALUE");
	
	Ihandle *taw_end 			= (Ihandle*)IupGetAttribute(ih, "taw_end");
	int end = IupGetInt(taw_end, "VALUE");
	
	--col;
	--end;
	
	DEBUG_LOG_ARGS("col: %i, start: %i, end: %i \n", col, start, end);
	
	Ihandle *taw_calc_result 	= (Ihandle*)IupGetAttribute(ih, "result_text");
	
	taw_col_t column = (taw_col_t)col;
	taw_result_t *taw_result = taw_calc(&column, start-4, end-3);

	if(taw_result->complete) {

		IupSetStrf(taw_calc_result, "TITLE", "%i AP", taw_result->complete->ap);

	}
	
	taw_result_free(&taw_result);

	return IUP_DEFAULT;
}

static void _taw_calc_init_(void * data) {
	
	DEBUG_LOG("taw_calc init\n");

	taw_calc_ctx_t * mctx = (taw_calc_ctx_t *)data;
	mctx->frame=NULL;
}

static void _taw_calc_free_(void * data) {

	taw_calc_ctx_t * mctx = (taw_calc_ctx_t *)data;
	
	iup_xml_builder_free(&mctx->builder);

	xml_source_free(&mctx->ui_res);

	free(mctx);

}

static const char * _taw_calc_name_(void * data) {
	return "TAW-Calculation";
}

void * _taw_calc_frame_(void * data) {
	taw_calc_ctx_t * mctx = (taw_calc_ctx_t *)data;
	
	if ( mctx->frame == NULL ) {
		//mctx->frame = __create_taw_calc_frame();
		mctx->builder = iup_xml_builder_new();

		iup_xml_builder_add_bytes(mctx->builder, "main",  
						(const char *)mctx->ui_res->src_data, 
						*mctx->ui_res->src_size);

		iup_xml_builder_add_callback(mctx->builder, "on_change", (Icallback)on_param_change_cb);

		iup_xml_builder_parse(mctx->builder);

		Ihandle *mres = iup_xml_builder_get_result(mctx->builder, "main");

		mctx->frame = iup_xml_builder_get_main(mres);
	}

	return mctx->frame;
}

void _taw_calc_prepare_(void * data) {
}

void _taw_calc_cleanup_(void * data) {
}

plugin_t * taw_calc_plugin(plugin_t * plugin, void *data) {
	plugin->name = _taw_calc_name_;
	plugin->frame = _taw_calc_frame_;
	plugin->init = _taw_calc_init_;
	plugin->free = _taw_calc_free_;
	plugin->prepare = _taw_calc_prepare_;
	plugin->cleanup = _taw_calc_cleanup_;
	taw_calc_ctx_t *plg_data = malloc(sizeof(taw_calc_ctx_t));
	plugin->data = plg_data;
	archive_resource_t *archive = (archive_resource_t *)data;
	plg_data->ui_res = xml_source_from_resname(archive, "taw_ui");
	plg_data->builder = NULL;
	return plugin;
}

