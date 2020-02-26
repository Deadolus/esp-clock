file(GLOB SRC_FILES "*.c" "*.h")
set(COMPONENT_SRCS ${SRC_FILES})

set (CMAKE_CXX_STANDARD 14)
#add_definitions(-Wno-missing-field-initializers)

idf_component_register(SRCS "${SRC_FILES}"
                    INCLUDE_DIRS . ${CMAKE_CURRENT_BINARY_DIR}
                    REQUIRES  esp_common mbedtls) 

#register_component()
