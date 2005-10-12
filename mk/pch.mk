ifneq ($(USE_PCH),)
  PCH_DIR=     $(abs_top_builddir)/$(PCH_HEADER).gch
  PCH_CFLAGS=   -include $(top_builddir)/$(PCH_HEADER)
  
  CXXFLAGS_NOPCH := $(CXXFLAGS)
  CXXFLAGS += -Winvalid-pch $(PCH_CFLAGS) 

build: build-pch
recurse-build: build-pch

build-pch: DESCRIPTION="Building PCH from $(PCH_HEADER)"
build-pch: $(PCH_DIR)/stamp
	
$(PCH_DIR)/stamp: $(abs_top_srcdir)/$(PCH_HEADER) 
	-@mkdir -p $(PCH_DIR)
	$(COMMAND_PREFIX)$(CXX) $(CPPFLAGS) $(CXXFLAGS_NOPCH) \
		-fPIC -DPIC -x c++ -c -o $(PCH_DIR)/cxx-pic $(abs_top_srcdir)/$(PCH_HEADER)
	$(COMMAND_PREFIX)$(CXX) $(CPPFLAGS) $(CXXFLAGS_NOPCH) \
		-x c++ -c -o $(PCH_DIR)/cxx-nopic $(abs_top_srcdir)/$(PCH_HEADER)
	@touch  $(PCH_DIR)/stamp

endif

pch-clean:
	@rm -fR $(PCH_DIR)

