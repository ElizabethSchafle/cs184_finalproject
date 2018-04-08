# Install script for directory: /Users/JoseChavez/Desktop/cs184/proj2-meshedit-jchavezberkeley/CGL/src

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
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "/Users/JoseChavez/Desktop/cs184/proj2-meshedit-jchavezberkeley/cmake-build-debug/CGL/src/libCGL.a")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libCGL.a" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libCGL.a")
    execute_process(COMMAND "/Library/Developer/CommandLineTools/usr/bin/ranlib" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libCGL.a")
  endif()
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/CGL" TYPE FILE FILES
    "/Users/JoseChavez/Desktop/cs184/proj2-meshedit-jchavezberkeley/CGL/src/CGL.h"
    "/Users/JoseChavez/Desktop/cs184/proj2-meshedit-jchavezberkeley/CGL/src/vector2D.h"
    "/Users/JoseChavez/Desktop/cs184/proj2-meshedit-jchavezberkeley/CGL/src/vector3D.h"
    "/Users/JoseChavez/Desktop/cs184/proj2-meshedit-jchavezberkeley/CGL/src/vector4D.h"
    "/Users/JoseChavez/Desktop/cs184/proj2-meshedit-jchavezberkeley/CGL/src/matrix3x3.h"
    "/Users/JoseChavez/Desktop/cs184/proj2-meshedit-jchavezberkeley/CGL/src/matrix4x4.h"
    "/Users/JoseChavez/Desktop/cs184/proj2-meshedit-jchavezberkeley/CGL/src/quaternion.h"
    "/Users/JoseChavez/Desktop/cs184/proj2-meshedit-jchavezberkeley/CGL/src/complex.h"
    "/Users/JoseChavez/Desktop/cs184/proj2-meshedit-jchavezberkeley/CGL/src/color.h"
    "/Users/JoseChavez/Desktop/cs184/proj2-meshedit-jchavezberkeley/CGL/src/osdtext.h"
    "/Users/JoseChavez/Desktop/cs184/proj2-meshedit-jchavezberkeley/CGL/src/viewer.h"
    "/Users/JoseChavez/Desktop/cs184/proj2-meshedit-jchavezberkeley/CGL/src/base64.h"
    "/Users/JoseChavez/Desktop/cs184/proj2-meshedit-jchavezberkeley/CGL/src/tinyxml2.h"
    "/Users/JoseChavez/Desktop/cs184/proj2-meshedit-jchavezberkeley/CGL/src/renderer.h"
    )
endif()

