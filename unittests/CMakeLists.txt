cmake_minimum_required(VERSION 2.8.7)
project(clock_test)

find_package(Threads REQUIRED)
find_package(GTest REQUIRED)
SET(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR})
find_package(GMock REQUIRED)

if(MSVC)
    #vc 2012 fix for vararg templates
    set(MSVC_COMPILER_DEFS "-D_VARIADIC_MAX=10")
endif()

#SET( CMAKE_CXX_COMPILER /usr/bin/clang++)
#SET( CMAKE_C_COMPILER /usr/bin/clang)
add_definitions(-Wall -Wno-deprecated -pthread)
add_definitions(-DUNIX_API)
add_definitions(-DCMAKE_EXPORT_COMPILE_COMMANDS=ON)
add_definitions(-DGOOGLETEST)
add_definitions(-std=c++1y)
add_definitions(--coverage -g -fprofile-arcs -ftest-coverage)
SET( CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} --coverage")
SET( CMAKE_EXPORT_COMPILE_COMMANDS ON )
SET(COMMON_INCLUDES
../
../main/
)

SET(TEST_INCLUDES
${CMAKE_CURRENT_SOURCE_DIR}/tests
${CMAKE_CURRENT_SOURCE_DIR}/helpers
)

include_directories(${GTEST_INCLUDE_DIRS} ${COMMON_INCLUDES} ${TEST_INCLUDES})

file(GLOB SRC_FILES ../*.cpp)
file(GLOB TEST_SRC_FILES
    #${CMAKE_CURRENT_SOURCE_DIR}/../main/*.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/../main/EspAlarmService.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../main/SimpleAlarmSerializer.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../main/Clock.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../main/EspAlarm.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../main/EspPersistentStorage.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../main/Timer.cpp
${CMAKE_CURRENT_SOURCE_DIR}/tests/*.cpp
${CMAKE_CURRENT_SOURCE_DIR}/helpers/*.cpp
${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
)

add_executable(${CMAKE_PROJECT_NAME} ${SRC_FILES} ${HAL_FILES} ${TEST_SRC_FILES})

target_link_libraries(${CMAKE_PROJECT_NAME} ${GTEST_BOTH_LIBRARIES} ${GMOCK_BOTH_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT} gcov)
MESSAGE( STATUS ${GTEST_INCLUDE_DIRS})
MESSAGE( STATUS ${GTEST_ALL_LIBRARIES})
MESSAGE( STATUS ${GTEST_LIBRARIES})
MESSAGE( STATUS ${GMOCK_LIBRARIES})
MESSAGE( STATUS ${CMAKE_MODULE_PATH})
