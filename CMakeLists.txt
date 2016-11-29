# @date November 2016
# @author Lazaros Penteridis <lp@ortelio.co.uk>
#
PROJECT(pixy_cam)
set (PIXY_CAM_VERSION_MAJOR 0)
set (PIXY_CAM_VERSION_MINOR 5)
set (PIXY_CAM_VERSION_PATCH 0)
set (PIXY_CAM_VERSION_STRING ${PIXY_CAM_VERSION_MAJOR}.${PIXY_CAM_VERSION_MINOR}.${PIXY_CAM_VERSION_PATCH})
cmake_minimum_required(VERSION 3.1)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake")

# library paths
set(LIBRARY_PATH ${LIBRARY_PATH}
              /lib
              /usr/lib
              /usr/lib64
              /usr/local/lib
              /usr/local/lib64
              /usr/lib/x86_64-linux-gnu)

set(INCLUDE_HEADERS ${INCLUDE_HEADERS} /usr/include/)
include_directories(SYSTEM ${INCLUDE_HEADERS})
set(INCLUDES ${INCLUDES} includes)

# set root at `/includes`
add_subdirectory(${INCLUDES})
set(CMAKE_MACOSX_RPATH 1)

message("Build directory: ${CMAKE_CURRENT_BINARY_DIR}")

# include everything from "/includes"
include_directories(".")

set (SOURCES ${SOURCES} 
             ${INCLUDES}/pixy_cam.cpp)

find_package(OpenCV REQUIRED)
find_package(libpixyusb REQUIRED)
find_package(libusb-1.0 REQUIRED)
find_package(Boost 1.49 COMPONENTS thread system chrono REQUIRED)

add_library(lpixy_cam OBJECT ${SOURCES})

#
# RPATH
#
set(CMAKE_SKIP_BUILD_RPATH  TRUE)
set(CMAKE_BUILD_WITH_INSTALL_RPATH TRUE)
set(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib/")
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH FALSE)
#
# use shared boost - build shared libs - set correct flags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}-std=c++1y -Wall -fPIC")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS}")

set(LIBTARGET ${LIBTARGET} pixy_cam)
add_library(${LIBTARGET} SHARED $<TARGET_OBJECTS:lpixy_cam>)

set_target_properties(${LIBTARGET} PROPERTIES VERSION ${PIXY_CAM_VERSION_STRING} SOVERSION ${PIXY_CAM_VERSION_STRING})

# libpixyusb should always come before libboost and libusb #
target_link_libraries(${LIBTARGET} pixyusb)
target_link_libraries(${LIBTARGET} ${OpenCV_LIBS})
target_link_libraries(${LIBTARGET} ${Boost_LIBRARIES})
target_link_libraries(${LIBTARGET} ${LIBUSB_1_LIBRARY})
set_target_properties(${LIBTARGET} PROPERTIES INSTALL_PATH "${CMAKE_INSTALL_PREFIX}/lib")
message(STATUS "Building Shared library: lib${LIBTARGET}")
#

file(STRINGS "cmake/VERSION" LIBPIXY_VERSION)
add_definitions(-D__LIBPIXY_VERSION__="${LIBPIXY_VERSION}")

add_executable(snap snap.cpp)
target_link_libraries(snap ${LIBTARGET} ${CMAKE_THREAD_LIBS_INIT})

include_directories (src
                     include
                     ../../common
                     ${Boost_INCLUDE_DIR}
                     ${LIBUSB_1_INCLUDE_DIRS})
###
# Install library 
#
set(LIB_VERSION ${PIXY_CAM_VERSION_MAJOR}.${PIXY_CAM_VERSION_MINOR}.${PIXY_CAM_VERSION_PATCH})
#
# symbolic link library version to the generic name
#
install(TARGETS ${LIBTARGET} DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/")
install(CODE 
    "EXECUTE_PROCESS(COMMAND ln -sf ${CMAKE_INSTALL_PREFIX}/lib/libpixy_cam.so.${LIB_VERSION} 
                                    ${CMAKE_INSTALL_PREFIX}/lib/libpixy_cam.so)")

message(STATUS "Installing library in: `${CMAKE_INSTALL_PREFIX}/lib`")
#
# Headers go under /usr/local/include
#
install(DIRECTORY ${INCLUDES}/ DESTINATION "${CMAKE_INSTALL_PREFIX}/include/${PROJECT_NAME}" 
                  FILES_MATCHING PATTERN "*.hpp" PATTERN "*.ihh" PATTERN "*.impl")
message(STATUS "Installing Headers in: `${CMAKE_INSTALL_PREFIX}/include/${PROJECT_NAME}`")
