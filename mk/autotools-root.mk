# $Revision: 1063 $
# $Id: autotools-root.mk 1063 2005-10-13 08:46:17Z sjoyeux $

distclean: autotools-root-distclean
autotools-root-distclean:
	rm -f config.status libtool config.log $(CONFIG_OUTPUT_FILES) 

clean: pch-clean

include $(top_srcdir)/mk/autotools.mk

