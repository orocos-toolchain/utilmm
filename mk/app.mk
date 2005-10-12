APP_OBJS = $(APP_SRC:%.cc=%.o)

build: $(APP_NAME)
$(APP_NAME): DESCRIPTION='Linking application $(APP_NAME) (libtool)'
$(APP_NAME): $(APP_OBJS) $(APP_EXTRAS) recurse-build
	$(COMMAND_PREFIX)$(LTLD) $(LDFLAGS) $(APP_LDFLAGS) -o $@ $(APP_OBJS) $(APP_EXTRAS) $(LIBS) $(APP_LIBS)

include $(top_srcdir)/mk/compile.mk

############### Clean
clean: app-clean
app-clean: DESCRIPTION='Cleaning $(PWD) (libtool)'
app-clean:
	$(COMMAND_PREFIX)$(LTRM) $(APP_NAME) $(APP_OBJS)

############### Install
install: app-install
app-install: DESCRIPTION='Installing $(APP_NAME) (libtool)'
app-install: $(APP_NAME)
	$(INSTALL_DIR) $(bindir)
	$(COMMAND_PREFIX)$(INSTALL_PROGRAM) $(APP_NAME) $(bindir)/$(APP_NAME)

############### Dependencies
DEP_SRC += $(APP_SRC)
DEP_CPPFLAGS += $(APP_CPPFLAGS)

ifneq ($(SUBDIRS),)
app-clean: recurse-clean
app-install: recurse-install
app-distclean: recurse-distclean
app-build: recurse-build
endif

