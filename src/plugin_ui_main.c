#include "plugin_ui_main.h"

EXTERN_BLOB(zip_resource, 7z);

static void _main_init_(void * data) {
	
	#if debug > 0
		printf("main init\n");
	#endif
	
	#if 0
		/** init context here
			All needed things:
		  */
	#endif
	main_ctx_t * mctx = (main_ctx_t *)data;
	mctx->cntplugins = 0;
	mctx->plugins  = NULL;
	
	mctx->archive = archive_resource_memory(&_binary_zip_resource_7z_start, (size_t)&_binary_zip_resource_7z_size);
	mctx->xml_result = archive_resource_search(mctx->archive, "xml/.*.xml");
	
	mctx->cntplugins = 2;
	mctx->plugins = malloc( mctx->cntplugins * sizeof(plugin_t));
	
	plugin_t *plugin = &mctx->plugins[0];
	lexicon_plugin(plugin);
	((lexicon_ctx_t*)plugin->data)->xml_result = mctx->xml_result;
	plugin->init(plugin->data);

	plugin = &mctx->plugins[1];
	taw_calc_plugin(plugin);
	plugin->init(plugin->data);
	
	mctx->frame=NULL;
}

static void _main_free_(void * data) {
	
	#if debug > 0
		printf("main free\n");
	#endif

	#if 0
		/** remove all special allocated things from init method..
		  */
	#endif
	main_ctx_t * mctx = (main_ctx_t *)data;
	for ( unsigned int i = mctx->cntplugins; i--;) {
		
		void * plugin_data = mctx->plugins[i].data;
		
		mctx->plugins[i].free(plugin_data);
	
	}
	
	#if debug > 0
		printf("free plugin container\n");
	#endif
	
	free(mctx->plugins);
	
	#if debug > 0
		printf("free  xml_result\n");
	#endif
	
	resource_search_result_full_free(&mctx->xml_result);
	
	#if debug > 0
		printf("free archive\n");
	#endif
	
	archive_resource_free(&mctx->archive);
	
	#if debug > 0
		printf("main context\n");
	#endif
	
	free(mctx);
}

static const char * _main_name_(void * data) {
	
	#if debug > 0
		printf("main name\n");
	#endif
	
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
	
	#if debug > 0
		printf("create_andor_open_plugin %s\n", pName);
	#endif
	
	
	bool exist = false;
	Ihandle * child = NULL;
	
	Ihandle *tabs = IupGetHandle("tabs");
	int i = IupGetChildCount(tabs);
	
	#if debug > 0
		printf("child count %i\n", i);
	#endif
	
	for (;i--;) {
		
		child = IupGetChild(tabs, i);
		
		if ( child ) {
		
			char * tabname = IupGetAttribute(child,"TABTITLE");
			
			#if debug > 0
				printf("found child tab: %s\n",tabname);
			#endif
			
			exist = imStrEqual(tabname, pName);
			
			if ( exist )
				break;
		}
	
	}
	
	if ( !exist ) {
	
		Ihandle * plugin_frame = plugin->frame(plugin->data);
		
		if(plugin_frame) {
			int tabpos = IupGetChildCount(tabs);
			IupSetAttribute(plugin_frame, "EXPANDCHILDREN", "YES");
			IupSetAttribute(plugin_frame, "TABTITLE", pName);
			IupSetInt(plugin_frame, "TABPOS", tabpos);
			
			IupAppend(tabs, plugin_frame);
			
			IupMap(plugin_frame); //important: IupCreate  -> IupAppend -> IupMap
			
			IupSetInt(tabs, "VALUEPOS", tabpos);
			
			plugin->prepare(plugin->data);
		}
	} else {
	
		int tabpos = IupGetInt(child, "TABPOS");
		
		#if debug > 0
			printf("set current tab %i\n", tabpos);
		#endif
		
		IupSetAttributeId(tabs, "TABVISIBLE", tabpos, "YES");
		
		IupSetInt(tabs, "VALUEPOS", tabpos);
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
	
		Ihandle *mitem = IupItem(mctx->plugins[i].name(NULL), NULL);
		IupSetCallback(mitem, "ACTION", (Icallback)plugin_cb);
		IupSetAttribute(mitem, "PLUGIN_OBJ", (void *)&mctx->plugins[i]);
		
		IupAppend( pmenu, mitem);
	
	}
	
	Ihandle * submenu = IupSubmenu("Plugins", pmenu);
	
	return submenu;
}

void * _main_frame_(void * data) {

	#if debug > 0
		printf("main frame\n");
	#endif
	
	main_ctx_t * mctx = (main_ctx_t *)data;
	Ihandle * frame = mctx->frame;
	if ( mctx->frame == NULL ) {
		
		#if debug > 0
			printf("create new\n");
		#endif
		
		Ihandle * home = IupScrollBox(IupVbox(IupLabel("Home Tab"), IupFill(),NULL));
		IupSetAttribute(home, "EXPANDCHILDREN", "yes");
		IupSetAttribute(home, "TABTITLE", "Home");
		
		Ihandle * tabs = IupTabs(home, NULL);
		IupSetAttribute(tabs, "SHOWCLOSE", "yes");
		
		Ihandle * navtree = IupTree();
		IupSetAttribute(navtree, "EXPAND", "VERTICAL");
		IupSetAttribute(navtree, "SIZE", "100x");
		
		Ihandle * expand_tree = IupExpander(navtree);
		IupSetAttribute(expand_tree, "STATE", "CLOSE");
		IupSetAttribute(expand_tree, "BARPOSITION", "LEFT");
		IupSetAttribute(expand_tree, "EXPAND", "VERTICAL");
		
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

	#if debug > 0
		printf("handle main prepare\n");
	#endif
	
	Ihandle *navtree = IupGetHandle("tree");
	IupSetAttribute(navtree, "TITLE","System");

	main_ctx_t * mctx = (main_ctx_t *)data;
	
	for ( unsigned int i = mctx->cntplugins; i--;) {
	
		IupSetAttribute(navtree, "ADDLEAF", mctx->plugins[i].name(NULL));
	
	}	
	
	IupSetAttribute(navtree, "ADDLEAF","Home");
}

void _main_cleanup_(void * data) {

	#if debug > 0
		printf("main plugin cleanup\n");
	#endif
	
	main_ctx_t * mctx = (main_ctx_t *)data;
	
	for ( unsigned int i = mctx->cntplugins; i--;) {
		
		void * plugin_data = mctx->plugins[i].data;
		
		mctx->plugins[i].cleanup(plugin_data);
	
	}
}

plugin_t * main_plugin(plugin_t * plugin) {
	plugin->name = _main_name_;
	plugin->frame = _main_frame_;
	plugin->init = _main_init_;
	plugin->free = _main_free_;
	plugin->prepare = _main_prepare_;
	plugin->cleanup = _main_cleanup_;
	plugin->data = malloc(sizeof(main_ctx_t)); //here malloc
	return plugin;
}
