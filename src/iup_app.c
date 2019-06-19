#include "iup_app.h"

#if 0
	/**
	* This Section includes iup app preparation. 
	* 
	* iup_init_app		=> prepare vom app initialisation (runs before iup main loop)
	*			=> GUI-Creation
				=> Plugin-Structure creation
					=> ui .....
	* iup_init_param 	=> param preparation if needed (runs before iup main loop)
	* iup_run_app 		=> start iup application (starts the iup main loop)
	*/
#endif

void iup_init_app(app_t * app, app_param_t * param) {
	
	DEBUG_LOG("calling iup init app\n");

	#if 0
		/** 
			Iup should opened here to setup iup app environment.
				- open Iup Environment
				- Plugin preparation callbacks for lazy ui elements
					- plugin configuration should implement like app => every plugin got func
						example:  renderer_plugin(...) return inf renderer plugin prototype
								  texturing_plugin(...) return inf texturing plugin prototype
				- Main GUI Initialisation
		 */
	#endif
	IupOpen(&app->argc, &app->argv);
	
	#if 0
		//works for windows and motif, bad, so we have to use multiple languages
	#endif

	IupSetGlobal("UTF8MODE", "YES"); 
	
	plugin_t * plugin = new_plugin(); 
	main_plugin(plugin);
	plugin->init(plugin->data);
	main_ctx_t * mainctx = (main_ctx_t *)plugin->data;
	
	DEBUG_LOG_ARGS("load main ctx with: %i\n", mainctx->frame);

	Ihandle * maindlg = plugin->frame(plugin->data);
	IupShowXY(maindlg, IUP_CENTER, IUP_CENTER);

	plugin->prepare(plugin->data);
	
	param->app_data = (void*)plugin;
}

void iup_init_app_param(app_param_t * param) {
	UNUSED(param);
	
	DEBUG_LOG("calling iup init param\n");
}

bool iup_run_app(app_t * app)
{ 
	UNUSED(app);

  	DEBUG_LOG("call main loop\n");

	IupMainLoop(); 

	return true;
}

void iup_free_app(app_t * app) {

	
	DEBUG_LOG("iup app free\n");
	
	app_param_t * param = app->param;
	plugin_t *plugin = (plugin_t *)param->app_data;
	plugin->cleanup(plugin->data);
	plugin->free(plugin->data);
	free_plugin(plugin);
	plugin = NULL;
	IupClose();
}

