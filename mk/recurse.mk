# -*- Makefile -*-
# $Id: recurse.mk 1010 2005-10-10 13:25:49Z sjoyeux $

ifdef SUBDIRS 
build: recurse-build
clean: recurse-clean
distclean: recurse-distclean
install: recurse-install

recurse-%:
	@set -e ; for dir in $(SUBDIRS) ; do \
	  $(MAKE) -C $$dir $*; \
	 done 
endif

