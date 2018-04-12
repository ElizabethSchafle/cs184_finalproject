# Install script for directory: /Users/elizabeth/Desktop/cs184/finalProj/cs184_finalproject/CGL/src

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/usr/local")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Debug")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "/Users/elizabeth/Desktop/cs184/finalProj/cs184_finalproject/cmake-build-debug/CGL/src/libCGL.a")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libCGL.a" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libCGL.a")
    execute_process(COMMAND "/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/ranlib" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libCGL.a")
  endif()
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/CGL" TYPE FILE FILES
    "/Users/elizabeth/Desktop/cs184/finalProj/cs184_finalproject/CGL/src/CGL.h"
    "/Users/elizabeth/Desktop/cs184/finalProj/cs184_finalproject/CGL/src/vector2D.h"
    "/Users/elizabeth/Desktop/cs184/finalProj/cs184_finalproject/CGL/src/vector3D.h"
    "/Users/elizabeth/Desktop/cs184/finalProj/cs184_finalproject/CGL/src/vector4D.h"
    "/Users/elizabeth/Desktop/cs184/finalProj/cs184_finalproject/CGL/src/matrix3x3.h"
    "/Users/elizabeth/Desktop/cs184/finalProj/cs184_finalproject/CGL/src/matrix4x4.h"
    "/Users/elizabeth/Desktop/cs184/finalProj/cs184_finalproject/CGL/src/quaternion.h"
    "/Users/elizabeth/Desktop/cs184/finalProj/cs184_finalproject/CGL/src/complex.h"
    "/Users/elizabeth/Desktop/cs184/finalProj/cs184_finalproject/CGL/src/color.h"
    "/Users/elizabeth/Desktop/cs184/finalProj/cs184_finalproject/CGL/src/osdtext.h"
    "/Users/elizabeth/Desktop/cs184/finalProj/cs184_finalproject/CGL/src/viewer.h"
    "/Users/elizabeth/Desktop/cs184/finalProj/cs184_finalproject/CGL/src/base64.h"
    "/Users/elizabeth/Desktop/cs184/finalProj/cs184_finalproject/CGL/src/tinyxml2.h"
    "/Users/elizabeth/Desktop/cs184/finalProj/cs184_finalproject/CGL/src/renderer.h"
    )
endif()

