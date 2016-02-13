# Install script for directory: /Users/Gus/GitHub_/CeCe

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

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Examples")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/." TYPE DIRECTORY FILES "/Users/Gus/GitHub_/CeCe/examples" FILES_MATCHING REGEX "/[^/]*$")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/." TYPE FILE FILES "/Users/Gus/GitHub_/CeCe/Copyright.txt")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for each subdirectory.
  include("/Users/Gus/GitHub_/CeCe/build/cece/cmake_install.cmake")
  include("/Users/Gus/GitHub_/CeCe/build/cece/plugins/agglutination/cmake_install.cmake")
  include("/Users/Gus/GitHub_/CeCe/build/cece/plugins/background/cmake_install.cmake")
  include("/Users/Gus/GitHub_/CeCe/build/cece/plugins/cell/cmake_install.cmake")
  include("/Users/Gus/GitHub_/CeCe/build/cece/plugins/chemostat/cmake_install.cmake")
  include("/Users/Gus/GitHub_/CeCe/build/cece/plugins/diffusion/cmake_install.cmake")
  include("/Users/Gus/GitHub_/CeCe/build/cece/plugins/diffusion-streamlines/cmake_install.cmake")
  include("/Users/Gus/GitHub_/CeCe/build/cece/plugins/object-generator/cmake_install.cmake")
  include("/Users/Gus/GitHub_/CeCe/build/cece/plugins/obstacle/cmake_install.cmake")
  include("/Users/Gus/GitHub_/CeCe/build/cece/plugins/obstacles-svg/cmake_install.cmake")
  include("/Users/Gus/GitHub_/CeCe/build/cece/plugins/picture/cmake_install.cmake")
  include("/Users/Gus/GitHub_/CeCe/build/cece/plugins/stochastic-reactions/cmake_install.cmake")
  include("/Users/Gus/GitHub_/CeCe/build/cece/plugins/streamlines/cmake_install.cmake")
  include("/Users/Gus/GitHub_/CeCe/build/cece/plugins/xml/cmake_install.cmake")
  include("/Users/Gus/GitHub_/CeCe/build/app/cli/cmake_install.cmake")

endif()

if(CMAKE_INSTALL_COMPONENT)
  set(CMAKE_INSTALL_MANIFEST "install_manifest_${CMAKE_INSTALL_COMPONENT}.txt")
else()
  set(CMAKE_INSTALL_MANIFEST "install_manifest.txt")
endif()

string(REPLACE ";" "\n" CMAKE_INSTALL_MANIFEST_CONTENT
       "${CMAKE_INSTALL_MANIFEST_FILES}")
file(WRITE "/Users/Gus/GitHub_/CeCe/build/${CMAKE_INSTALL_MANIFEST}"
     "${CMAKE_INSTALL_MANIFEST_CONTENT}")
