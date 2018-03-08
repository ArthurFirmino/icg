cmake_minimum_required(VERSION 2.8)
project(icg)

#--- Load the common ICG configuration
include(common/config.cmake)
include(common/Eigen.cmake)
include(common/OpenGP.cmake)

#--- OpenGL configuration
include(common/OpenGL.cmake)
include(common/GLEW.cmake)
include(common/GLFW.cmake)

#--- Subprojects
#add_subdirectory(raytrace)
#add_subdirectory(rasterize)
#add_subdirectory(trianglemesh)
#add_subdirectory(transform)
#add_subdirectory(antialiasing)
#add_subdirectory(framebuffer2d)
add_subdirectory(mouse)
add_subdirectory(mouse_framebuffer)

#add_subdirectory(assignment2)
