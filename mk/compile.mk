# $Revision: 1318 $
# $Id: compile.mk 1318 2006-01-31 17:21:48Z sjoyeux $

%.lo: DESCRIPTION="Compiling $(notdir $@) (libtool)"
	
%.lo: %.c
	$(COMMAND_PREFIX)$(LTCC) $(PREFER_NON_PIC) $(CFLAGS) $($(MODULE)_CFLAGS) -c $(CPPFLAGS) $($(MODULE)_CPPFLAGS) -o $@ $<
	
%.lo: %.cc
	$(COMMAND_PREFIX)$(LTCXX) $(PREFER_NON_PIC) $(CXXFLAGS) $($(MODULE)_CXXFLAGS) -c $(CPPFLAGS) $($(MODULE)_CPPFLAGS) -o $@ $<

