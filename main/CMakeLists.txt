file(GLOB SRC_FILES "*.cpp" "*.h")
set(COMPONENT_SRCS ${SRC_FILES})
set(COMPONENT_ADD_INCLUDEDIRS ". ../libraries ../httpd ../ADXL345")

register_component()
