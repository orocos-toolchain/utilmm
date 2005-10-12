distclean: autotools-root-distclean
autotools-root-distclean:
	rm -f config.status libtool config.log $(CONFIG_OUTPUT_FILES) 

clean: pch-clean

include $(top_srcdir)/mk/autotools.mk

