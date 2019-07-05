#include "plugin_main.h"

EXTERN_BLOB(zip_resource, 7z);

EXTERN_BLOB(zip_ui_resource, 7z);

static void _main_init_(void * data) {
	
	DEBUG_LOG("main init\n");
	
	main_ctx_t * mctx = (main_ctx_t *)data;
	
	mctx->archive = archive_resource_memory(&_binary_zip_resource_7z_start, (size_t)&_binary_zip_resource_7z_size);
	mctx->ui_archive = archive_resource_memory(&_binary_zip_ui_resource_7z_start, (size_t)&_binary_zip_ui_resource_7z_size);
	
	mctx->xml_result = archive_resource_search(mctx->archive, "xml/.*.xml");
	
	mctx->cntplugins = 3;
	mctx->plugins = malloc( mctx->cntplugins * sizeof(plugin_t*));
	
	mctx->plugins[0] = new_plugin();
	plugin_t *plugin = mctx->plugins[0];
	lexicon_plugin(plugin);
	((lexicon_ctx_t*)plugin->data)->xml_result = mctx->xml_result;
	plugin->init(plugin->data);

	mctx->plugins[1] = new_plugin();
	plugin = mctx->plugins[1];
	taw_calc_plugin(plugin, mctx->ui_archive);
	plugin->init(plugin->data);

	mctx->plugins[2] = new_plugin();
	plugin = mctx->plugins[2];
	hgen_plugin(plugin);
	((hgen_ctx_t*)plugin->data)->archive = mctx->archive;
	plugin->init(plugin->data);
	
	mctx->frame=NULL;
}

static void _main_free_(void * data) {
	
	DEBUG_LOG("main free\n");

	#if 0
		/** remove all special allocated things from init method..
		  */
	#endif
	main_ctx_t * mctx = (main_ctx_t *)data;
	for ( unsigned int i = mctx->cntplugins; i--;) {
		
		void * plugin_data = mctx->plugins[i]->data;
		
		mctx->plugins[i]->free(plugin_data);

		free_plugin(mctx->plugins[i]);
	
	}
	
	DEBUG_LOG("free plugin container\n");

	free(mctx->plugins);

	DEBUG_LOG("free  xml_result\n");

	//resource_search_result_free(&mctx->xml_result);
	resource_search_result_full_free(&mctx->xml_result);

	DEBUG_LOG("free archive\n");
	
	archive_resource_free(&mctx->archive);
	archive_resource_free(&mctx->ui_archive);
	
	DEBUG_LOG("main context\n");
	
	free(mctx);
}

static const char * _main_name_(void * data) {
		
	DEBUG_LOG("main name\n");
	
	return "DSA - Tools of Alveran";
}
//default exit callback
int exit_cb(void) {
	
	return IUP_CLOSE;
}

static Ihandle *create_file_menu() {
	Ihandle * item_exit = IupItem("Exit", NULL);
	IupSetCallback(item_exit, "ACTION", (Icallback)exit_cb);
	Ihandle * fmenu = IupMenu(item_exit, NULL); 
	Ihandle * submenu = IupSubmenu("File", fmenu);
	return submenu;
}

static void create_andor_open_plugin(plugin_t * plugin) {
	
	const char * pName = plugin->name(NULL);
	
	DEBUG_LOG_ARGS("create_andor_open_plugin %s\n", pName);
	
	Ihandle *tabs = IupGetHandle("tabs");

	Ihandle * plugin_frame = plugin->frame(plugin->data);

	int childid = IupGetChildPos(tabs, plugin_frame);

	if ( childid == -1 ) {
		
		DEBUG_LOG_ARGS("create new %s\n", pName);

		IupSetAttribute(plugin_frame, "EXPANDCHILDREN", "YES");
		IupSetAttribute(plugin_frame, "TABTITLE", pName);
		
		IupAppend(tabs, plugin_frame);
		
		IupMap(plugin_frame); //important: IupCreate  -> IupAppend -> IupMap

		plugin->prepare(plugin->data);

		IupSetAttribute(tabs, "VALUE_HANDLE", (void*)plugin_frame);

	} else {

		DEBUG_LOG_ARGS("use existing %s\n", pName);

		IupSetAttribute(tabs, "VALUE_HANDLE", (void*)plugin_frame);

		IupSetAttributeId(tabs, "TABVISIBLE", childid, "YES");
	}
	
	IupRefresh(tabs);

}

static int plugin_cb(Ihandle * handle) {

	plugin_t * plugin = (plugin_t *)IupGetAttribute(handle, "PLUGIN_OBJ");
	
	if ( plugin ) {
		create_andor_open_plugin(plugin);
	}
	
	return IUP_DEFAULT;
	
}

static Ihandle * create_plugin_menu(main_ctx_t * mctx) {
	
	Ihandle * pmenu = IupMenu(NULL);
	
	for ( unsigned int i = 0; i < mctx->cntplugins ; ++i ) {
	
		Ihandle *mitem = IupItem(mctx->plugins[i]->name(NULL), NULL);
		IupSetCallback(mitem, "ACTION", (Icallback)plugin_cb);
		IupSetAttribute(mitem, "PLUGIN_OBJ", (void *)mctx->plugins[i]);
		
		IupAppend( pmenu, mitem);
	
	}
	
	Ihandle * submenu = IupSubmenu("Plugins", pmenu);
	
	return submenu;
}

void * _main_frame_(void * data) {

	DEBUG_LOG("main frame\n");
	
	main_ctx_t * mctx = (main_ctx_t *)data;
	Ihandle * frame = mctx->frame;
	if ( mctx->frame == NULL ) {
		
		DEBUG_LOG("create new\n");
		
		Ihandle * home = IupScrollBox(IupVbox(IupLabel("Home Tab"), IupFill(),NULL));
		IupSetAttributes(home, "EXPANDCHILDREN=yes, TABTITLE=Alveran-News");
		
		Ihandle * tabs = IupTabs(home, NULL);
		IupSetAttribute(tabs, "SHOWCLOSE", "yes");
		IupSetCallback(tabs,"TABCLOSE_CB",(Icallback)do_not_close_first_tab_callback);
		
		Ihandle * navtree = IupTree();
		IupSetAttributes(navtree, "EXPAND=VERTICAL, SIZE=100x");
		
		Ihandle * expand_tree = IupExpander(navtree);
		IupSetAttributes(expand_tree, "STATE=CLOSE, BARPOSITION=LEFT, EXPAND=VERTICAL");
		
		Ihandle * main_box = IupHbox(expand_tree, tabs, NULL);
		frame = IupDialog(main_box);
		IupSetAttribute(frame, "TITLE", _main_name_(data));
		IupSetAttribute(frame, "SIZE", "HALFxHALF");
		
		Ihandle * menu = IupMenu(create_file_menu(), create_plugin_menu(mctx), NULL);
		IupSetAttributeHandle(frame, "MENU", menu);
		
		mctx->frame = frame;
		
		IupSetHandle("tree",navtree);
		IupSetHandle("tabs",tabs);
		IupSetHandle("main_frame",mctx->frame);
	
	}
	
	return frame;
}

void _main_prepare_(void * data) {

	DEBUG_LOG("handle main prepare\n");
	
	Ihandle *navtree = IupGetHandle("tree");
	IupSetAttribute(navtree, "TITLE", "System");

	main_ctx_t * mctx = (main_ctx_t *)data;
	
	for ( unsigned int i = mctx->cntplugins; i--;) {
	
		IupSetAttribute(navtree, "ADDLEAF", mctx->plugins[i]->name(NULL));

	}	
	
	IupSetAttribute(navtree, "ADDLEAF","Home");
}

void _main_cleanup_(void * data) {

	DEBUG_LOG("main plugin cleanup\n");
	
	main_ctx_t * mctx = (main_ctx_t *)data;
	
	for ( unsigned int i = mctx->cntplugins; i--;) {
		
		void * plugin_data = mctx->plugins[i]->data;
		
		mctx->plugins[i]->cleanup(plugin_data);
	
	}
}

plugin_t * main_plugin(plugin_t * plugin) {
	plugin->name = _main_name_;
	plugin->frame = _main_frame_;
	plugin->init = _main_init_;
	plugin->free = _main_free_;
	plugin->prepare = _main_prepare_;
	plugin->cleanup = _main_cleanup_;
	plugin->data = malloc(sizeof(main_ctx_t));
	return plugin;
}
