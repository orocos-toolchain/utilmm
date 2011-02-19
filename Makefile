ifdef ROS_ROOT
default: install
include $(shell rospack find mk)/cmake.mk
EXTRA_CMAKE_FLAGS=-DCMAKE_INSTALL_PREFIX=`rospack find utilmm`/install
install: all
	cd build; ${MAKE} install
else
$(warning This Makefile only works with ROS rosmake. Without rosmake, create a build directory and run cmake ..)
endif
