#
# "main" pseudo-component makefile.
#
# (Uses default behaviour of compiling all source files in directory, adding 'include' to include path.)
COMPONENT_ADD_INCLUDEDIRS += ../libraries ../httpd
#CXXFLAGS += -std=c++14 -D__UCLIBC__
CXXFLAGS += -D__UCLIBC__
