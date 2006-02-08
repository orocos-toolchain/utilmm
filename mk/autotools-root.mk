# $Revision: 1395 $
# $Id: autotools-root.mk 1395 2006-02-08 08:07:59Z sjoyeux $

distclean: autotools-root-distclean
autotools-root-distclean:
	rm -f config.status libtool config.log $(CONFIG_OUTPUT_FILES) mk/init.mk Init.make

clean: pch-clean

include $(top_srcdir)/mk/autotools.mk

