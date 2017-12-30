# Locate the GLFW library (version 3.0)
# This module defines the following variables:
# GLFW_LIBRARIES, the name of the library;
# GLFW_INCLUDE_DIRS, where to find GLFW include files.
# GLFW_FOUND, true if library path was resolved
#
# Usage example to compile an "executable" target to the glfw library:
#
# FIND_PACKAGE (GLFW REQUIRED)
# INCLUDE_DIRECTORIES (${GLFW_INCLUDE_DIRS})
# ADD_EXECUTABLE (executable ${YOUR_EXECUTABLE_SRCS})
# TARGET_LINK_LIBRARIES (executable ${GLFW_LIBRARIES})
#
# TODO:
# Lookup for windows
# Allow the user to select to link to a shared library or to a static library.
#
# SEE:
# - https://raw.github.com/progschj/OpenGL-Examples/master/cmake_modules/FindGLFW.cmake
# 

IF(WIN32)
    IF(MSVC)
        IF(MSVC_VERSION EQUAL 1910)
            SET(GLFW_WINLIB "lib-vc2015")
        ELSE(MSVC_VERSION EQUAL 1910)
            SET(GLFW_WINLIB "lib-vc2012")
        ENDIF(MSVC_VERSION EQUAL 1910)
    ELSEIF(MINGW)
        SET(GLFW_WINLIB "lib-mingw-w64")
    ELSE(MSVC)
        message(ERROR "Unsupported Compiler")
    ENDIF(MSVC)
ENDIF(WIN32)

FIND_PATH( GLFW3_INCLUDE_DIRS GLFW/glfw3.h
    $ENV{GLFWDIR}/include
    /usr/local/include
    /usr/local/X11R6/include
    /usr/X11R6/include
    /usr/X11/include
    /usr/include/X11
    /usr/include
    /opt/X11/include
    /opt/include
    ${CMAKE_SOURCE_DIR}/external/glfw/include)

FIND_LIBRARY( GLFW3_LIBRARIES NAMES glfw3 glfw PATHS
    $ENV{GLFWDIR}/lib
    $ENV{GLFWDIR}/support/msvc80/Debug
    $ENV{GLFWDIR}/support/msvc80/Release
    /usr/local/lib
    /usr/local/X11R6/lib
    /usr/X11R6/lib
    /usr/X11/lib
    /usr/lib/X11
    /usr/lib
    /usr/lib/x86_64-linux-gnu/
    /opt/X11/lib
    /opt/lib
    ${CMAKE_SOURCE_DIR}/external/glfw/${GLFW_WINLIB})

SET(GLFW3_FOUND "NO")
IF(GLFW3_LIBRARIES AND GLFW3_INCLUDE_DIRS)
    SET(GLFW3_FOUND "YES")
    message(STATUS "Found GLFW3: ${GLFW3_LIBRARIES}")
ENDIF(GLFW3_LIBRARIES AND GLFW3_INCLUDE_DIRS)
