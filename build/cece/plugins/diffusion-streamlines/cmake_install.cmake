# Install script for directory: /Users/Gus/GitHub_/CeCe/cece/plugins/diffusion-streamlines

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
    set(CMAKE_INSTALL_CONFIG_NAME "")
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

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Plugins")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/plugins" TYPE SHARED_LIBRARY FILES "/Users/Gus/GitHub_/CeCe/build/cece/plugins/diffusion-streamlines/libplugin-diffusion-streamlines.dylib")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/plugins/libplugin-diffusion-streamlines.dylib" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/plugins/libplugin-diffusion-streamlines.dylib")
    execute_process(COMMAND /usr/bin/install_name_tool
      -delete_rpath "/Users/Gus/GitHub_/CeCe/build/cece/plugins/diffusion"
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/plugins/libplugin-diffusion-streamlines.dylib")
    execute_process(COMMAND /usr/bin/install_name_tool
      -delete_rpath "/Users/Gus/GitHub_/CeCe/build/cece/plugins/streamlines"
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/plugins/libplugin-diffusion-streamlines.dylib")
    execute_process(COMMAND /usr/bin/install_name_tool
      -delete_rpath "/Users/Gus/GitHub_/CeCe/build/cece"
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/plugins/libplugin-diffusion-streamlines.dylib")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/strip" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/plugins/libplugin-diffusion-streamlines.dylib")
    endif()
  endif()
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Runtime")
  
                    execute_process(COMMAND "/Users/Gus/GitHub_/CeCe/resources/scripts/macosx-libraries.sh" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/plugins/libplugin-diffusion-streamlines.dylib")
                    
endif()

