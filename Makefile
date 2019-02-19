include ../make_config
#-ggdb  -mwindows
#-pg for profiling 

#MATH_ALGORITHM_DIR=$(patsubst %,algorithm/%,fractals noise)
#MATH_LIB_DIR=$(patsubst %,math/%, $(MATH_ALGORITHM_DIR) statistics utils mat vec)
#COLLECTION_LIB_DIR=$(patsubst %,collections/%,array)
#RENDERER_LIB_DIR=renderer scene mesh shape color texture $(MATH_LIB_DIR) $(COLLECTION_LIB_DIR)

IUP_INCLUDE=$(patsubst %,tec_tools/%/include/,iup cd im)
THIRD_PARTY_LIB_DIR=./../_third_/
INCLUDEDIR=-I.  
INCLUDEDIR+=$(patsubst %,-I$(THIRD_PARTY_LIB_DIR)%,$(IUP_INCLUDE))
INCLUDEDIR+=$(patsubst %,-I$(THIRD_PARTY_LIB_DIR)%,pcre2_bin/include libarchive_bin/include libxml_bin/include/libxml2 libxslt_bin/include)
# $(RENDERER_LIB_DIR))

_SRC_FILES=main app iup_app plugin resource plugin_ui_main
_SRC_PLUGIN_FILES=plugin_ui_lexicon plugin_ui_taw_calc
# regexp not working because invalid stack usage inside of function and using deprecated pcre not pcre2
#SRC=./../_third_/libxslt-plugin-regexp-0.3/regexp.c
SRC+=$(patsubst %,src/%,$(patsubst %,%.c,$(_SRC_FILES) $(_SRC_PLUGIN_FILES)))
#SRC+=$(patsubst %,src/%,$(patsubst %,%.h,$(_SRC_FILES)))
#$(info $$_SRC is [${_SRC}])
BIN=dsa_portal.exe

DSA_PORTAL_LIBS=
IUP_LIBS=cdcontextplus gdiplus im iupcd iup cd
THIRD_PARTY_LIBS=exslt xslt xml2 archive crypto nettle regex lzma z lz4 bz2 bcrypt freetype6 iconv
REGEX_LIBS=pcre2-8
#for pcre
#REGEX_LIBS=pcre
#this c flags is used by regex lib
CFLAGS+=-DPCRE2_STATIC
#for pcre
#CFLAGS+=-DPCRE_STATIC
OS_LIBS=kernel32 user32 gdi32 winspool comdlg32 advapi32 shell32 uuid ole32 oleaut32 comctl32 ws2_32

USED_LIBS=$(patsubst %,-l%,$(DSA_PORTAL_LIBS) $(IUP_LIBS) $(REGEX_LIBS) $(THIRD_PARTY_LIBS) $(OS_LIBS) )


#USED_LIBSDIR=-L$(BUILDDIR) $(patsubst %,%/$(BUILDDIR),$(patsubst %,-L./../%,$(RENDERER_LIB_DIR))) -L./../_third_/tec_tools/release/static/ 
USED_LIBSDIR=-L$(BUILDPATH) -L$(THIRD_PARTY_LIB_DIR)tec_tools/release/static/ -LC:/dev/opt/msys64/mingw64/lib
USED_LIBSDIR+=$(patsubst %,-L$(THIRD_PARTY_LIB_DIR)%,pcre2_bin/lib libarchive_bin/lib libxml_bin/lib libxslt_bin/lib)


#wc -c < filename => if needed for after compression size of bytes
RES=zip_resource
RES_O=$(RES).o
RES_O_PATH=$(BUILDPATH)$(RES_O)
RES_7Z=$(RES).7z
RES_FILES_PATTERN=./data/*
ZIP=7z
ZIP_ARGS=a -t7z
ZIP_CMD=$(ZIP) $(ZIP_ARGS)

ifeq ($(isdebug),1)
	debug+=-ggdb
endif

all: mkbuilddir mkzip addzip $(BUILDPATH)$(BIN)
	
$(BUILDPATH)$(BIN): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(BUILDPATH)$(BIN) $(INCLUDEDIR) $(RES_O_PATH) $(USED_LIBSDIR) -static $(USED_LIBS) $(debug) $(release)

test_resource: mkbuilddir mkzip addzip 
	$(CC) $(CFLAGS) ./test/test_resource.c ./src/resource.c -o $(BUILDPATH)test_resource.exe -I./src/ $(INCLUDEDIR) $(RES_O_PATH) $(USED_LIBSDIR) -static $(USED_LIBS) $(debug)
	$(BUILDPATH)test_resource.exe
	
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
	