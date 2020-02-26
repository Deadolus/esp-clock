#include_directories("src")
file(GLOB SRC_FILES "*.c" "*.cpp" "*.h")
set(COMPONENT_SRCS ${SRC_FILES})
#set(COMPONENT_ADD_INCLUDEDIRS "src")
set(includes ".")


set (CMAKE_CXX_STANDARD 14)

idf_component_register(SRCS "${SRC_FILES}"
                    INCLUDE_DIRS ${includes}
                    REQUIRES  soc) 
