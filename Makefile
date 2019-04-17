#
# This is a project Makefile. It is assumed the directory this Makefile resides in is a
# project subdirectory.
#

PROJECT_NAME := e_ink_display

EXTRA_COMPONENT_DIRS := libraries httpd lwip fsdata ADXL345
#CXXFLAGS += "-std=c++1z"
CXXFLAGS += "-DLWIP_HTTPD_SSI"
CXXFLAGS += "-DLWIP_HTTPD_CGI"

include $(IDF_PATH)/make/project.mk


