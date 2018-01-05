cmake_minimum_required(VERSION 2.8)
project(icg)

#--- Load the common ICG configuration
include(common/config.cmake)
include(common/Eigen.cmake)
include(common/OpenGP.cmake)

if(NOT WIN32)
    include(common/PNG.cmake)
endif()

#--- OpenGL configuration
include(common/OpenGL.cmake)
include(common/GLEW.cmake)
include(common/GLFW.cmake)

#--- Subprojects
add_subdirectory(raytrace)
