#---------------------------------------
# Examples makefile example...
#
# This makefile builds the examples
# for the FNE C software client kit.
#
#---------------------------------------

# TARGETS:
#
# FNE Example application targets:
#     basic_client            produces bin/basic_client
#     client                  produces bin/client
#     caprequest              produces bin/caprequest
#     trials                  produces bin/trials
#     view                    produces bin/view
#     usage_capture_client    produces bin/usage_capture_client
#     served_buffer_request   produces bin/served_buffer_request
#     calc                    produces bin/calc
#     calc_one                produces bin/calc_one
#     lmflex                  produces bin/lmflex [not included in default target]
#
#  DSA Library options:
#      DSALIB=LK
#      DSALIB=TRL
#      DSALIB=RSA	-- default
PWD_VAR=pwd
CALL_VAR=
BSE=$(shell $(PWD_VAR))
BASE=$(subst \,/,$(BSE))

CC=gcc
CXX=g++
LD=gcc
AR=ar

DEBUG_FLAGS=-g
CFLAGS=-x c -Wstrict-aliasing $(DEBUG_FLAGS)
CXXFLAGS=-x c++ -Wstrict-aliasing $(DEBUG_FLAGS)
LFLAGS=
LFLAGSTRA=
DFLAGS=-DHAVE_STDINT -DHAVE_SETENV -DNDEBUG

RM=rm -f
RMDIR=rm -rf
MKDIR=mkdir -p

LIBDIR=

 
LFLAGSTRA+=-lstdc++ -lm

custom_signer_obj=ExampleFletcherChecksum.o

BINDIR=$(BASE)/bin
OBJDIR=$(BASE)/obj
vpath %.c $(BASE)/examples/client_samples/client
vpath %.c $(BASE)/examples/client_samples/capability_request
vpath %.c $(BASE)/examples/client_samples/trials
vpath %.c $(BASE)/examples/client_samples/view
vpath %.c $(BASE)/examples/client_samples/short_code
vpath %.c $(BASE)/examples/client_samples/usage_capture_client
vpath %.c $(BASE)/examples/client_samples/served_buffer_request
vpath %.c $(BASE)/examples/client_samples/lmflex
vpath %.c $(BASE)/examples/util
vpath %.c $(BASE)/examples/tra_samples/calc/auto_gen
vpath %.cpp $(BASE)/examples/tra_samples/calc
vpath %.cpp $(BASE)/examples/tra_samples/calc_one
vpath %.c $(BASE)/examples/tra_samples/tra_one/auto_gen
vpath %.c $(BASE)/examples/tra_samples/tra_one
vpath %.o $(OBJDIR)

TRAGEN=$(BINDIR)/tra-gen

#-------------------
# SOURCES
#-------------------
util_csrc = \
        $(BASE)/examples/util/ErrDisplay.c \
        $(BASE)/examples/util/FileIO.c
util_src = $(notdir $(util_csrc))
util_obj = $(util_src:.c=.o)
util_obj_dir=$(addprefix $(OBJDIR)/,$(util_obj))

calc_csrc = $(BASE)/examples/tra_samples/calc/auto_gen/calc_tra.c
calc_src = $(notdir $(calc_csrc))
calc_obj = $(calc_src:.c=.o)
calc_obj_dir = $(addprefix $(OBJDIR)/,$(calc_obj))

tra_one_csrc = $(BASE)/examples/tra_samples/tra_one/auto_gen/script.c $(BASE)/examples/tra_samples/tra_one/fne_one.c
tra_one_src = $(notdir $(tra_one_csrc))
tra_one_obj = $(tra_one_src:.c=.o)
tra_one_obj_dir = $(addprefix $(OBJDIR)/,$(tra_one_obj))

calc_one_src = $(BASE)/examples/tra_samples/calc_one/calc_one.cpp
calc_one_obj = $(calc_one_src:.c=.o)
calc_one_obj_dir = $(addprefix $(OBJDIR)/,$(calc_one_obj))

BASIC_CLIENT        =$(BASE)/examples/basic_client/BasicClient.c
CLIENT              =$(BASE)/examples/client_samples/client/Client.c
CAPREQUEST          =$(BASE)/examples/client_samples/capability_request/CapabilityRequest.c
TRIALS              =$(BASE)/examples/client_samples/trials/Trials.c
VIEW                =$(BASE)/examples/client_samples/view/View.c
USAGECAPTURECLIENT  =$(BASE)/examples/client_samples/usage_capture_client/UsageCaptureClient.c
SERVEDBUFFERREQUEST =$(BASE)/examples/client_samples/served_buffer_request/ServedBufferRequest.c
LMFLEX              =$(BASE)/examples/client_samples/lmflex/Lmflex.c
CALC                =$(BASE)/examples/tra_samples/calc/calc.cpp
CALC_ONE            =$(BASE)/examples/tra_samples/calc/calc_one.cpp

all_exe=$(CLIENT) $(CAPREQUEST) $(TRIALS) $(VIEW) \
        $(USAGECAPTURECLIENT) $(SERVEDBUFFERREQUEST) $(CALC) $(CALC_ONE)
exe_obj=$(addprefix $(OBJDIR)/, $(notdir $(all_exe:.c=.o)))
exe_obj+=$(addprefix $(OBJDIR)/, $(notdir $(all_exe:.cpp=.o)))

#-------------------
# INCLUDES
#-------------------
# some include files are in the examples directory
EXAMPLES_INCLUDE=$(BASE)/examples
COMMON_INC=$(BASE)/examples/util
KIT_INCLUDE=-I$(BASE)/include -I$(EXAMPLES_INCLUDE) -I$(COMMON_INC)
CALC_INCLUDE=-I$(BASE)/examples/tra_samples/calc/auto_gen
TRA_ONE_INCLUDE=-I$(BASE)/examples/tra_samples/tra_one/auto_gen -I$(BASE)/examples/tra_samples/tra_one
CALC_ONE_INCLUDE=$(KIT_INCLUDE) $(TRA_ONE_INCLUDE)


#-------------------
# LIBRARIES
#-------------------
SWCLIENT_LIB_DIR=$(BASE)/lib

FLXCLIENT=$(SWCLIENT_LIB_DIR)/libFlxClientXT.a
FLXCOMMON=$(SWCLIENT_LIB_DIR)/libFlxCommonXT.a
TRALIB=$(SWCLIENT_LIB_DIR)/libtra.a

FNESWLIB=$(FLXCLIENT) $(FLXCOMMON) $(TRALIB) -ldl -lpthread

LDIR+=-L$(SWCLIENT_LIB_DIR)


examples_identity=examples/identity
identity_h=$(examples_identity)/IdentityClient.h
serverid=$(examples_identity)/bin/IdentityBackOffice.bin
clientid=$(examples_identity)/bin/IdentityClient.bin
clientserverid=$(examples_identity)/bin/IdentityClientServer.bin

ifeq "$(DSALIB)" "TRL"
	little_lib=trl
	strength=-strength 1
endif
ifeq "$(DSALIB)" "LK"
	little_lib=lk
	strength=
endif
ifeq "$(DSALIB)" "RSA"
	little_lib=rsa
	strength=
endif
DONE=" Compile complete. \n\n"


ifneq "$(VENDOR_KEYS)" ""
    trial_bin=examples/client_samples/trials/trial.bin
    trial_lic=examples/licenses/trial.lic
    demo_lic=examples/licenses/demo.lic
    ts_demo_lic=examples/licenses/demo-1234567890.lic
    legacy_lic=examples/licenses/legacy.lic
    license_bin=examples/client_samples/client/license.bin
    view_bin=examples/client_samples/view/license.bin
    response_bin=examples/client_samples/capability_request/response.bin
    buffer_response_bin=examples/client_samples/served_buffer_request/buffer_response.bin
endif


all: dirs gen_files basic_client client caprequest \
	trials view usage_capture_client served_buffer_request calc calc_one
	@echo -e $(DONE)

gen_files:

ifneq "$(VENDOR_KEYS)" ""
	-$(CALL_VAR) bin/tools/pubidutil -silent -backOffice $(serverid) -client $(clientid) -clientServer $(clientserverid) -name demo -keys $(VENDOR_KEYS) -type $(little_lib) $(strength) -console -identityName demo
	-$(CALL_VAR) bin/tools/printbin -C -ident identity_data -o $(identity_h) $(clientid)
	-cp $(serverid)  examples/test_backoffice_server/webapps/ROOT/IdentityBackOffice.bin
	-$(CALL_VAR) bin/tools/trialfileutil -id $(serverid) -product SampleApp3000R -duration 2592000 -trial 1 -once $(trial_lic) $(trial_bin) || echo "Can't create $(trial_bin)."
	-$(CALL_VAR) bin/tools/licensefileutil -id $(serverid) $(demo_lic) $(license_bin) || echo "Can't create $(license_bin)."
	-$(CALL_VAR) bin/tools/licensefileutil -id $(serverid) $(demo_lic) $(view_bin) || echo "Can't create $(view_bin)."
	-$(CALL_VAR) bin/tools/capresponseutil -id $(serverid)  -device 1234567890 -lifetime 253800000 -attr key1 value1 -attr key2 value2 $(ts_demo_lic) $(response_bin) || echo "Can't create $(response_bin)"
	-$(CALL_VAR) bin/tools/capresponseutil -id $(serverid)  -device 1234567890 -lifetime 253800000 -buffer $(ts_demo_lic) $(buffer_response_bin) || echo "Can't create $(buffer_response_bin)"
endif

dirs:
	$(MKDIR) obj
	$(MKDIR) bin
ifneq "$(VENDOR_KEYS)" ""
	$(MKDIR) examples/identity/bin
endif

basic_client: dirs
	$(CC) -c $(DFLAGS) $(CFLAGS) examples/basic_client/BasicClient.c -o $(OBJDIR)/BasicClient.o $(WARNINGS) $(KIT_INCLUDE) $(INCLUDE)
	$(LD) $(LFLAGS) -o $(BASE)/bin/basicclient $(OBJDIR)/BasicClient.o \
	$(LIBDIR) $(LDIR) $(FNESWLIB)

client: dirs $(util_obj) Client.o
	$(LD) $(LFLAGS) -o $(BINDIR)/client $(OBJDIR)/Client.o \
	$(util_obj_dir) \
	$(LIBDIR) $(LDIR) $(FNESWLIB)

caprequest: dirs $(util_obj) CapabilityRequest.o
	$(LD) $(LFLAGS) -o $(BINDIR)/caprequest $(OBJDIR)/CapabilityRequest.o \
	$(util_obj_dir) \
	$(LIBDIR) $(LDIR) $(FNESWLIB)

trials: dirs $(util_obj) Trials.o
	$(LD) $(LFLAGS) -o $(BINDIR)/trials $(OBJDIR)/Trials.o \
	$(util_obj_dir) \
	$(LIBDIR) $(LDIR) $(FNESWLIB)

view: dirs $(util_obj) View.o
	$(LD) $(LFLAGS) -o $(BINDIR)/view $(OBJDIR)/View.o \
	$(util_obj_dir) \
	$(LIBDIR) $(LDIR) $(FNESWLIB)

usage_capture_client: dirs $(util_obj) UsageCaptureClient.o
	$(LD) $(LFLAGS) -o $(BINDIR)/usage_capture_client $(OBJDIR)/UsageCaptureClient.o \
	$(util_obj_dir) \
	$(LIBDIR) $(LDIR) $(FNESWLIB)

served_buffer_request: dirs $(util_obj) ServedBufferRequest.o
	$(LD) $(LFLAGS) -o $(BINDIR)/served_buffer_request $(OBJDIR)/ServedBufferRequest.o \
	$(util_obj_dir) \
	$(LIBDIR) $(LDIR) $(FNESWLIB)

calc_prebuild:
	$(TRAGEN) --identity_path $(BASE)/examples/identity --out $(BASE)/examples/tra_samples/calc/auto_gen $(BASE)/examples/tra_samples/calc/calc_gen.tra

calc_compile: dirs calc_prebuild
	@$(MAKE) -f build/examples.mk calc_compile_real

calc_compile_real: dirs calc.o $(calc_obj)
	$(LD) $(LFLAGS) -o $(BINDIR)/calc $(OBJDIR)/calc.o \
	$(calc_obj_dir) \
	$(LIBDIR) $(LDIR) $(FNESWLIB) $(LFLAGSTRA)

calc_postbuild:calc_compile
	$(TRAGEN) --identity_path $(BASE)/examples/identity --out $(BASE)/examples/tra_samples/calc/auto_gen --app $(BINDIR)/calc,$(BASE)/lib $(BASE)/examples/tra_samples/calc/calc_gen.tra

calc: calc_postbuild
	@echo ""

calc_one: tra_one_compile
	@$(MAKE) -f build/examples.mk calc_one_compile_real

calc_one_compile_real: dirs calc_one.o $(calc_one_obj)
	$(LD) $(LFLAGS) -o $(BINDIR)/calc_one $(OBJDIR)/calc_one.o $(OBJDIR)/tra_one.o $(OBJDIR)/fne_one.o $(OBJDIR)/script.o \
	$(util_obj_dir) \
	$(LIBDIR) $(LDIR) $(FNESWLIB) $(LFLAGSTRA)

clean_tra_one:
	@-$(RM) $(OBJDIR)/tra_one.*

tra_one_prebuild: clean_tra_one
	$(TRAGEN) --identity_path $(BASE)/examples/identity \
	--out $(BASE)/examples/tra_samples/tra_one/auto_gen \
	$(BASE)/examples/tra_samples/tra_one/tra_one_gen.tra

tra_one_compile: dirs $(util_obj) tra_one_prebuild
	@$(MAKE) -f build/examples.mk tra_one_compile_real

tra_one_compile_real: dirs tra_one.o $(tra_one_obj)
	@echo ""

lmflex: dirs $(util_obj) Lmflex.o
	$(LD) $(LFLAGS) -o $(BINDIR)/lmflex $(OBJDIR)/Lmflex.o \
	$(util_obj_dir) \
	$(LIBDIR) $(LDIR) $(FNESWLIB)

clean:
	@-$(RMDIR) $(OBJDIR)
	@-$(RM) $(BINDIR)/caprequest
	@-$(RM) $(BINDIR)/client
	@-$(RM) $(BINDIR)/trials
	@-$(RM) $(BINDIR)/view
	@-$(RM) $(BINDIR)/basicclient
	@-$(RM) $(BINDIR)/usage_capture_client
	@-$(RM) $(BINDIR)/served_buffer_request
	@-$(RM) $(BINDIR)/lmflex
	@-$(RM) $(BINDIR)/calc
	@-$(RMDIR) $(BASE)/examples/tra_samples/calc/auto_gen
	@-$(RMDIR) $(BASE)/examples/tra_samples/tra_one/auto_gen
	@-$(RMDIR) $(BASE)/examples/tra_samples/calc_one/auto_gen

ifneq "$(VENDOR_KEYS)" ""
	@-$(RM) $(BASE)/$(license_bin)
	@-$(RM) $(BASE)/$(view_bin)
	@-$(RM) $(BASE)/$(trial_bin)
	@-$(RM) $(BASE)/$(response_bin)
	@-$(RM) $(BASE)/$(buffer_response_bin)
	@-$(RMDIR) $(examples_identity)/bin
endif


%.o: %.c
	$(CC) $(CFLAGS) $(DFLAGS) $(LIBCURLFLAGS) -c $< -o $(OBJDIR)/$@ $(WARNINGS) $(KIT_INCLUDE) $(INCLUDE) $(TRA_ONE_INCLUDE)
	$(CC) -MM $(DFLAGS) $(KIT_INCLUDE) $(INCLUDE) $(TRA_ONE_INCLUDE) $< >$(OBJDIR)/$*.d

%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(DFLAGS) $(LIBCURLFLAGS) -c $< -o $(OBJDIR)/$@ $(WARNINGS) $(KIT_INCLUDE) $(CALC_INCLUDE) $(CALC_ONE_INCLUDE)
	$(CXX) -MM $(DFLAGS) $(KIT_INCLUDE) $(CALC_INCLUDE) $(CALC_ONE_INCLUDE) $< >$(OBJDIR)/$*.d

-include $(OBJECTS:.o=.d)
-include $(exe_obj:.o=.d)
