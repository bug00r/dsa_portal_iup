#include "plugin_ui_taw_calc.h"

static const unsigned int taws[] = {
	5,    5,    10,   15,    20,    25,    40,    50,   100,
	5,    5,    10,   15,    20,    25,    40,    50,   100,
	5,    5,    10,   15,    20,    25,    40,    50,   100,
	5,    5,    10,   15,    20,    25,    40,    50,   100,
	1,    1,     2,    2,     3,     4,     6,     8,    16,
	1,    2,     4,    6,     7,     9,    14,    18,    35,
	1,    3,     6,    9,    12,    15,    22,    30,    60,
	2,    4,     8,   13,    17,    21,    32,    42,    85,
	4,    6,    11,   17,    22,    28,    41,    55,   110,
	5,    7,    14,   21,    27,    34,    50,    70,   140,
	6,    8,    17,   25,    33,    41,    60,    85,   165,
	8,   10,    19,   29,    39,    48,    75,    95,   195,
	9,   11,    22,   34,    45,    55,    85,   110,   220,
   11,   13,    25,   38,    50,    65,    95,   125,   250,
   12,   14,    28,   43,    55,    70,   105,   140,   280,
   14,   16,    32,   47,    65,    80,   120,   160,   320,
   15,   17,    35,   51,    70,    85,   130,   175,   350,
   17,   19,    38,   55,    75,    95,   140,   190,   380,
   19,   21,    41,   60,    85,   105,   155,   210,   410,
   20,   22,    45,   65,    90,   110,   165,   220,   450,
   22,   24,    48,   70,    95,   120,   180,   240,   480,
   24,   26,    51,   75,   105,   130,   195,   260,   510,
   25,   27,    55,   80,   110,   135,   210,   270,   550,
   27,   29,    58,   85,   115,   145,   220,   290,   580,
   29,   31,    62,   95,   125,   155,   230,   310,   620,
   31,   33,    65,  100,   130,   165,   250,   330,   650,
   32,   34,    69,  105,   140,   170,   260,   340,   690,
   34,   36,    73,  110,   145,   180,   270,   360,   720,
   36,   38,    76,  115,   150,   190,   290,   380,   760,
   38,   40,    80,  120,   160,   200,   300,   400,   800,
   40,   42,    84,  125,   165,   210,   310,   420,   830,
   42,   44,    87,  130,   170,   220,   330,   440,   870,
   43,   45,    91,  135,   180,   230,   340,   460,   910,
   45,   47,    95,  140,   190,   240,   350,   480,   950,
   48,   50,   100,  150,   200,   250,   375,   500,   1000
};

static int __calc_taw_button_callback(Ihandle *search_button) {

	#if debug > 0
		printf("trigger calc\n");
	#endif
	
	Ihandle *taw_column 		= IupGetHandle("taw_column");
	int col = IupGetInt(taw_column, "VALUE");
	
	Ihandle *taw_start 			= IupGetHandle("taw_start");
	int start = IupGetInt(taw_start, "VALUE");
	
	Ihandle *taw_end 			= IupGetHandle("taw_end");
	int end = IupGetInt(taw_end, "VALUE");
	
	int delta = end - start;
	
	int add = ( delta < 0 ? -1 : 1 );
	
	--col;
	//--start;
	--end;
	
	#if debug > 0
		printf("col: %i, start: %i, end: %i \n", col, start, end);
	#endif
	
	int sum = 0;
	int cur = start;
	Ihandle *taw_calc_result 	= IupGetHandle("taw_calc_result");
	int show_details = IupGetInt(IupGetHandle("taw_details"), "VALUE");
	IupSetStrf(taw_calc_result, "APPEND", "%s: ", IupGetAttribute(taw_column, "VALUESTRING"));
	int cur_value = 0;
	for (int i = 0; i < delta; ++i) {
		
		cur_value = taws[cur * 9 + col];
		
		sum += cur_value;
		
		#if debug > 0
			printf("(%i -> %i = %i)\n", (cur-3-1), (cur-3-1+add), cur_value);
		#endif
		
		if(show_details == 1) {
			IupSetStrf(taw_calc_result, "APPEND", "(%i->%i=%i) ", cur-4, cur-4+add, cur_value);
		}
		
		cur += add;
	}

	if(show_details == 0) {
		IupSetStrf(taw_calc_result, "APPEND", "(%i->%i=%i) ", start-4, end-3, cur_value);
	}
	
	IupSetStrf(taw_calc_result, "APPEND", "sum: %i\n", sum);
	
	#if debug > 0
		printf("sum: %i\n", sum);
	#endif
	
	
	
	return IUP_DEFAULT;
}

static Ihandle * __create_taw_calc_searchbar()
{	
	Ihandle *column_input = IupList(NULL);
	IupSetAttributes(column_input, "DROPDOWN=YES, EXPAND=HORIZONTAL, VISIBLEITEMS=15, VALUE=1");
	IupSetAttributes(column_input, "1=A*, 2=A, 3=B, 4=C, 5=D, 6=E, 7=F, 8=G, 9=H");
	
	Ihandle *column_label = IupLabel("column:");
	Ihandle *column = IupHbox(column_label, column_input, NULL);
	
	IupSetAttribute(column, "ALIGNMENT", "ACENTER");
	
	Ihandle *taw_start_input = IupList(NULL);
	IupSetAttributes(taw_start_input, "DROPDOWN=YES, EXPAND=HORIZONTAL, VISIBLEITEMS=15, VALUE=1");
	IupSetAttributes(taw_start_input, "1=-3, 2=-2, 3=-1, 4=0, 5=1, 6=2, 7=3, 8=4, 9=5, 10=6, 11=7, 12=8, 13=9, 14=10, 15=11");
	IupSetAttributes(taw_start_input, "16=12, 17=13, 18=14, 19=15, 20=16, 21=17, 22=18, 23=19, 24=20, 25=21, 26=22, 27=23, 28=24, 29=25");
	IupSetAttributes(taw_start_input, "30=26, 31=27, 32=28, 33=29, 34=30, 35=31+");
	Ihandle *taw_start_label = IupLabel("taw_start:");
	Ihandle *taw_start = IupHbox(taw_start_label, taw_start_input, NULL);
	IupSetAttribute(taw_start, "ALIGNMENT", "ACENTER");
	
	Ihandle *taw_end_input = IupList(NULL);
	IupSetAttributes(taw_end_input, "DROPDOWN=YES, EXPAND=HORIZONTAL, VISIBLEITEMS=15, VALUE=1");
	IupSetAttributes(taw_end_input, "1=-3, 2=-2, 3=-1, 4=0, 5=1, 6=2, 7=3, 8=4, 9=5, 10=6, 11=7, 12=8, 13=9, 14=10, 15=11");
	IupSetAttributes(taw_end_input, "16=12, 17=13, 18=14, 19=15, 20=16, 21=17, 22=18, 23=19, 24=20, 25=21, 26=22, 27=23, 28=24, 29=25");
	IupSetAttributes(taw_end_input, "30=26, 31=27, 32=28, 33=29, 34=30, 35=31+");
	
	Ihandle *taw_end_label = IupLabel("taw_end:");
	Ihandle *taw_end = IupHbox(taw_end_label, taw_end_input, NULL);
	IupSetAttribute(taw_end, "ALIGNMENT", "ACENTER");
		
	Ihandle *details_toggle = IupToggle("Details", NULL);
	IupSetAttributes(details_toggle, "ALIGNMENT=ACENTER");
	
	Ihandle *search_button = IupButton("search", NULL);
	IupSetCallback(search_button, "ACTION", (Icallback)__calc_taw_button_callback);
	IupSetAttributes(search_button, "ALIGNMENT=ACENTER:ACENTER, EXPAND=HORIZONTAL");
	
	Ihandle *searchbar = IupHbox(column, taw_start, taw_end, details_toggle, search_button, NULL);
	IupSetAttribute(searchbar, "GAP", "5");
	
	IupSetHandle("taw_column", column_input);
	IupSetHandle("taw_start", taw_start_input);
	IupSetHandle("taw_end", taw_end_input);
	IupSetHandle("taw_details", details_toggle);
	
	return searchbar;
}

static Ihandle * __create_taw_calc_frame()
{
	Ihandle *searchbar = __create_taw_calc_searchbar();
	
	Ihandle *result_text = IupMultiLine(NULL);
	IupSetAttributes(result_text, "READONLY=YES, EXPAND=YES, WORDWRAP=YES, APPENDNEWLINE=NO");
	
	IupSetHandle("taw_calc_result", result_text);
	
	Ihandle * maindlg = IupVbox(searchbar, IupHbox(result_text, NULL), NULL);
	return maindlg;
}

//###########

static void _taw_calc_init_(void * data) {
	printf("taw_calc init\n");
	#if 0
		/** init context here
			All needed things:
		  */
	#endif
	taw_calc_ctx_t * mctx = (taw_calc_ctx_t *)data;
	mctx->frame=NULL;
}

static void _taw_calc_free_(void * data) {
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
	plugin->data = malloc(sizeof(taw_calc_ctx_t));;
	return plugin;
}

