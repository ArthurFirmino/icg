#pragma once

///--- Standard library IO
#include <iostream>
#include <cassert>

///--- On some OSs the exit flags are not defined
#ifndef EXIT_SUCCESS
    #define EXIT_SUCCESS 0
#endif
#ifndef EXIT_FAILURE
    #define EXIT_FAILURE 1
#endif

///--- We use Eigen for linear algebra
#ifdef WITH_EIGEN
    #include <Eigen/Dense>
    typedef Eigen::Vector2f vec2;
    typedef Eigen::Vector3f vec3;
    typedef Eigen::Vector4f vec4;
    typedef Eigen::Matrix4f mat4;
    typedef Eigen::Matrix3f mat3;
#endif

///--- Utilities to simplify glfw setup
#if defined(WITH_OPENGP) && defined(WITH_GLFW)
    #include <OpenGP/GL/glfw_helpers.h>
#endif

///--- Shader compiling utilities
#if defined(WITH_OPENGL) && defined(WITH_OPENGP)
    #include "OpenGP/GL/shader_helpers.h"
#endif

///--- Shader compiling utilities
#if defined(WITH_OPENGL) && defined(WITH_OPENGP)
    #include "OpenGP/GL/shader_helpers.h"
#endif

///--- For mesh I/O we use OpenGP
#ifdef WITH_OPENGP
    #include <OpenGP/SurfaceMesh/SurfaceMesh.h>
#endif

///--- GL Error checking
#ifdef WITH_OPENGL
    #include "OpenGP/GL/check_error_gl.h"
#endif

///--- glfw
#if defined(WITH_GLFW)
    #include <GLFW/glfw3.h>
#endif

#include "picoPNG.h"

///--- These namespaces assumed by default
using namespace std;
