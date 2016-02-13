# Install script for directory: /Users/Gus/GitHub_/CeCe/cece

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

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Library")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE SHARED_LIBRARY FILES "/Users/Gus/GitHub_/CeCe/build/cece/libcece.dylib")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libcece.dylib" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libcece.dylib")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/strip" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libcece.dylib")
    endif()
  endif()
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Runtime")
  
            execute_process(COMMAND "/Users/Gus/GitHub_/CeCe/resources/scripts/macosx-libraries.sh" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libcece.dylib")
            
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE FILE FILES "/usr/local/Cellar/box2d/2.3.1/lib/libBox2D.2.3.0.dylib")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for each subdirectory.
  include("/Users/Gus/GitHub_/CeCe/build/cece/core/cmake_install.cmake")
  include("/Users/Gus/GitHub_/CeCe/build/cece/config/cmake_install.cmake")
  include("/Users/Gus/GitHub_/CeCe/build/cece/loader/cmake_install.cmake")
  include("/Users/Gus/GitHub_/CeCe/build/cece/init/cmake_install.cmake")
  include("/Users/Gus/GitHub_/CeCe/build/cece/module/cmake_install.cmake")
  include("/Users/Gus/GitHub_/CeCe/build/cece/object/cmake_install.cmake")
  include("/Users/Gus/GitHub_/CeCe/build/cece/program/cmake_install.cmake")
  include("/Users/Gus/GitHub_/CeCe/build/cece/plugin/cmake_install.cmake")
  include("/Users/Gus/GitHub_/CeCe/build/cece/render/cmake_install.cmake")
  include("/Users/Gus/GitHub_/CeCe/build/cece/simulator/cmake_install.cmake")

endif()

