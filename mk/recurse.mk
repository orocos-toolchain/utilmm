# $Revision: 1063 $
# $Id: recurse.mk 1063 2005-10-13 08:46:17Z sjoyeux $

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

