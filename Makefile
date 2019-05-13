include ../make_config

CFLAGS+=-std=c11

#-ggdb  -mwindows
#-pg for profiling 

#iup2.4
#IUP_INCLUDE=$(patsubst %,tec_tools/%/include/,iup cd im)
#iup2.7
IUP_INCLUDE=tec_tools/v27/release/include/
THIRD_PARTY_LIB_DIR=./../_third_/
INCLUDEDIR=-I. -I../dsa_core/src 
INCLUDEDIR+=$(patsubst %,-I$(THIRD_PARTY_LIB_DIR)%,$(IUP_INCLUDE) pcre2_bin/include libarchive_bin/include libxml_bin/include/libxml2 libxslt_bin/include)

_SRC_FILES=main app iup_app plugin plugin_ui_main 
#regex_utils string_utils file_path_utils xpath_utils taw resource
_SRC_PLUGIN_FILES=plugin_ui_lexicon plugin_ui_taw_calc

SRC+=$(patsubst %,src/%,$(patsubst %,%.c,$(_SRC_FILES) $(_SRC_PLUGIN_FILES)))
BIN=dsa_portal.exe

DSA_PORTAL_LIBS=dsa_core
IUP_LIBS=cdcontextplus gdiplus im iupcd iup cd
THIRD_PARTY_LIBS=exslt xslt xml2 archive crypto nettle regex lzma z lz4 bz2 bcrypt freetype6 iconv
REGEX_LIBS=pcre2-8
#this c flags is used by regex lib
CFLAGS+=-DPCRE2_STATIC

OS_LIBS=kernel32 user32 gdi32 winspool comdlg32 advapi32 shell32 uuid ole32 oleaut32 comctl32 ws2_32

USED_LIBS=$(patsubst %,-l%,$(DSA_PORTAL_LIBS) $(IUP_LIBS) $(REGEX_LIBS) $(THIRD_PARTY_LIBS) $(OS_LIBS) )

#iup2.4
#USED_LIBSDIR=-L$(BUILDPATH) -L$(THIRD_PARTY_LIB_DIR)tec_tools/release/static/ -LC:/dev/opt/msys64/mingw64/lib
#iup2.7
USED_LIBSDIR=-L$(BUILDPATH) -L$(THIRD_PARTY_LIB_DIR)tec_tools/v27/release/lib -LC:/dev/opt/msys64/mingw64/lib
USED_LIBSDIR+=$(patsubst %,-L$(THIRD_PARTY_LIB_DIR)%,pcre2_bin/lib libarchive_bin/lib libxml_bin/lib libxslt_bin/lib)
USED_LIBSDIR+=-L./../dsa_core/$(BUILDPATH)

#wc -c < filename => if needed for after compression size of bytes
RES=zip_resource
RES_O=$(RES).o
RES_O_PATH=$(BUILDPATH)$(RES_O)
RES_7Z=$(RES).7z
RES_FILES_PATTERN=./../dsa_core/data/*
ZIP=7z
ZIP_ARGS=a -t7z
ZIP_CMD=$(ZIP) $(ZIP_ARGS)

ifeq ($(isdebug),1)
	debug+=-ggdb
endif

all: mkbuilddir mkzip addzip $(BUILDPATH)$(BIN)
	
$(BUILDPATH)$(BIN): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(BUILDPATH)$(BIN) $(INCLUDEDIR) $(RES_O_PATH) $(USED_LIBSDIR) -static $(USED_LIBS) $(debug) $(release)

.PHONY: clean mkbuilddir mkzip addzip small
	
addzip:
	cd $(BUILDPATH); \
	ld -r -b binary $(RES_7Z) -o $(RES_O)

mkzip:
	-$(ZIP_CMD) $(BUILDPATH)$(RES_7Z) $(RES_FILES_PATTERN)

mkbuilddir:
	mkdir -p $(BUILDDIR)

small:
	-strip $(BUILDPATH)$(BIN)
	
clean:
	-rm -dr $(BUILDROOT)
	