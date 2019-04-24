file(GLOB SRC_FILES "*.c" "*.cpp" "*.h")
set(COMPONENT_SRCS ${SRC_FILES})

set (CMAKE_CXX_STANDARD 14)

register_component()
