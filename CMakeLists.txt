# The following lines of boilerplate have to be in your project's
# CMakeLists in this exact order for cmake to work correctly
cmake_minimum_required(VERSION 3.5)

include($ENV{IDF_PATH}/tools/cmake/project.cmake)
set (EXTRA_COMPONENT_DIRS "httpd")

set (CMAKE_CXX_STANDARD 11)
#Workaround to really get c++11 features
add_definitions(-D_GLIBCXX_USE_C99)

add_definitions(-DLWIP_HTTPD_SSI)
add_definitions(-DLWIP_HTTPD_CGI)
project(esp-alarm-clock)
