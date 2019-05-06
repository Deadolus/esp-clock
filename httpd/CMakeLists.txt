file(GLOB SRC_FILES "*.c" "*.h")
set(COMPONENT_SRCS ${SRC_FILES})

set (CMAKE_CXX_STANDARD 14)
add_definitions(-Wno-missing-field-initializers)

register_component()
