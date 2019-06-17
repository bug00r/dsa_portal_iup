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

	mctx->heros = dsa_heros_new_archiv(mctx->archive);
	mctx->nav_heros = dl_list_new();
	mctx->selected_hero = NULL;
	mctx->selected_list_pos = -1;

	IupSetGlobal("hero_ctx", data);
}

static void _hgen_free_(void * data) {
	free(data);
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

	hgen_ctx_t * mctx = (hgen_ctx_t *)data;
	
	init_breeds(IupGetHandle("breeds"), mctx->heros);
	init_cultures(IupGetHandle("cultures"), mctx->heros);
	init_professions(IupGetHandle("professions"), mctx->heros);

}

void _hgen_cleanup_(void * data) {
	hgen_ctx_t * mctx = (hgen_ctx_t *)data;
	dsa_heros_free(&mctx->heros);

	dl_list_item_t *cur_item = mctx->nav_heros->first;

	while(cur_item != NULL) {
		hero_nav_item_t * curnav = cur_item->data;
		dsa_hero_free((dsa_hero_t **)&curnav->hero);
		free(curnav);
		cur_item = cur_item->next;
	}

	dl_list_free(&mctx->nav_heros);
}

plugin_t * hgen_plugin(plugin_t * plugin) {
	plugin->name = _hgen_name_;
	plugin->frame = _hgen_frame_;
	plugin->init = _hgen_init_;
	plugin->free = _hgen_free_;
	plugin->prepare = _hgen_prepare_;
	plugin->cleanup = _hgen_cleanup_;
	plugin->data = malloc(sizeof(hgen_ctx_t));
	return plugin;
}

