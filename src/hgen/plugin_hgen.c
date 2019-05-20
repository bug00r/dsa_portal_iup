#include "plugin_hgen.h"

static void _hgen_init_(void * data) {
	
	#if debug > 0
		printf("hgen init\n");
	#endif

	#if 0
		/** init context here
			All needed things:
		  */
	#endif
	hgen_ctx_t * mctx = (hgen_ctx_t *)data;
	mctx->frame=NULL;
}

static void _hgen_free_(void * data) {
}

static const char * _hgen_name_(void * data) {
	return "Hero-Generator";
}

void * _hgen_frame_(void * data) {
	hgen_ctx_t * mctx = (hgen_ctx_t *)data;
	if ( mctx->frame == NULL ) {
		mctx->frame = create_hgen_frame();
	}
	return mctx->frame;
}

void _hgen_prepare_(void * data) {
}

void _hgen_cleanup_(void * data) {
}

plugin_t * hgen_plugin(plugin_t * plugin) {
	plugin->name = _hgen_name_;
	plugin->frame = _hgen_frame_;
	plugin->init = _hgen_init_;
	plugin->free = _hgen_free_;
	plugin->prepare = _hgen_prepare_;
	plugin->cleanup = _hgen_cleanup_;
	plugin->data = malloc(sizeof(hgen_ctx_t));;
	return plugin;
}

