/*
    nanogui/opengl.h -- Pulls in OpenGL, GLAD (if needed), GLFW, and
    NanoVG header files

    NanoGUI was developed by Wenzel Jakob <wenzel.jakob@epfl.ch>.
    The widget drawing code is based on the NanoVG demo application
    by Mikko Mononen.

    All rights reserved. Use of this source code is governed by a
    BSD-style license that can be found in the LICENSE.txt file.
*/
/** \file */

#pragma once

#include <nanogui/vector.h>

#if !defined(DOXYGEN_SHOULD_SKIP_THIS)
#  if defined(NANOGUI_USE_OPENGL)
#    if defined(NANOGUI_GLAD)
#      if defined(NANOGUI_SHARED) && !defined(GLAD_GLAPI_EXPORT)
#        define GLAD_GLAPI_EXPORT
#      endif
#      include <glad/glad.h>
#    else
#      if defined(__APPLE__)
#        define GLFW_INCLUDE_GLCOREARB
#      else
#        define GL_GLEXT_PROTOTYPES
#      endif
#    endif
#  elif defined(NANOGUI_USE_GLES) && NANOGUI_GLES_VERSION == 2
#    define GLFW_INCLUDE_ES2
#  elif defined(NANOGUI_USE_GLES) && NANOGUI_GLES_VERSION == 3
#    define GLFW_INCLUDE_ES3
#  elif defined(NANOGUI_USE_METAL)
#  else
#    error You must select a backend (OpenGL/GLES2/GLES3/Metal)
#  endif
#endif // DOXYGEN_SHOULD_SKIP_THIS

#ifndef NANOGUI_NO_GLFW
#include <GLFW/glfw3.h>

#define NANOGUI_RELEASE GLFW_RELEASE
#define NANOGUI_PRESS GLFW_PRESS
#define NANOGUI_REPEAT GLFW_REPEAT

#define NANOGUI_KEY_A GLFW_KEY_A
#define NANOGUI_KEY_C GLFW_KEY_C
#define NANOGUI_KEY_R GLFW_KEY_R
#define NANOGUI_KEY_V GLFW_KEY_V
#define NANOGUI_KEY_X GLFW_KEY_X

#define NANOGUI_KEY_ENTER GLFW_KEY_ENTER
#define NANOGUI_KEY_BACKSPACE GLFW_KEY_BACKSPACE
#define NANOGUI_KEY_DELETE GLFW_KEY_DELETE
#define NANOGUI_KEY_RIGHT GLFW_KEY_RIGHT
#define NANOGUI_KEY_LEFT GLFW_KEY_LEFT
#define NANOGUI_KEY_HOME GLFW_KEY_HOME
#define NANOGUI_KEY_END GLFW_KEY_END

#define NANOGUI_MOD_SHIFT GLFW_MOD_SHIFT
#define NANOGUI_MOD_SUPER GLFW_MOD_SUPER
#define NANOGUI_MOD_CONTROL GLFW_MOD_CONTROL

#define NANOGUI_MOUSE_BUTTON_1 GLFW_MOUSE_BUTTON_1
#define NANOGUI_MOUSE_BUTTON_2 GLFW_MOUSE_BUTTON_2

#else
#if __APPLE__
#include <OpenGL/gl3.h>
#include <OpenGL/gl3ext.h>
#else
#include <GL/gl.h>
#include <GL/glext.h>
#endif

#define NANOGUI_RELEASE 0
#define NANOGUI_PRESS 1
#define NANOGUI_REPEAT 2

#define NANOGUI_KEY_A 65
#define NANOGUI_KEY_C 67
#define NANOGUI_KEY_R 82
#define NANOGUI_KEY_V 86
#define NANOGUI_KEY_X 88

#define NANOGUI_KEY_ENTER 257
#define NANOGUI_KEY_BACKSPACE 259
#define NANOGUI_KEY_DELETE 261
#define NANOGUI_KEY_RIGHT 262
#define NANOGUI_KEY_LEFT 263
#define NANOGUI_KEY_HOME 268
#define NANOGUI_KEY_END 269

#define NANOGUI_MOD_SHIFT 0x0001
#define NANOGUI_MOD_CONTROL 0x0002
#define NANOGUI_MOD_SUPER 0x0008

#define NANOGUI_MOUSE_BUTTON_1 0
#define NANOGUI_MOUSE_BUTTON_2 1

#endif

#if defined(NANOGUI_USE_GLES)
#  if NANOGUI_GLES_VERSION == 2
#    include <GLES2/gl2ext.h>
#  elif NANOGUI_GLES_VERSION == 3
#    include <GLES3/gl2ext.h>
#  endif
#endif

#include <nanovg.h>

// Special treatment of linux Nvidia opengl headers
#if !defined(_WIN32) && !defined(__APPLE__) && defined(NANOGUI_USE_OPENGL)
  #if !defined(GL_UNIFORM_BUFFER)
    #warning NanoGUI suspects you have the NVIDIA OpenGL headers installed.  \
             Compilation will likely fail. If it does, you have two choices: \
             (1) Re-install the mesa-lib_gl header files.                    \
             (2) Compile with NANOGUI_USE_GLAD.
  #endif
#endif

NAMESPACE_BEGIN(nanogui)

/// Allows for conversion between nanogui::Color and the NanoVG NVGcolor class.
inline Color::operator const NVGcolor &() const {
    return reinterpret_cast<const NVGcolor &>(*(this->v));
}

/**
 * \brief Determine whether an icon ID is a texture loaded via ``nvg_image_icon``.
 *
 * \rst
 * The implementation defines all ``value < 1024`` as image icons, and
 * everything ``>= 1024`` as an Entypo icon (see :ref:`file_nanogui_entypo.h`).
 * The value ``1024`` exists to provide a generous buffer on how many images
 * may have been loaded by NanoVG.
 * \endrst
 *
 * \param value
 *     The integral value of the icon.
 *
 * \return
 *     Whether or not this is an image icon.
 */
inline bool nvg_is_image_icon(int value) { return value < 1024; }

/**
 * \brief Determine whether an icon ID is a font-based icon (e.g. from ``entypo.ttf``).
 *
 * \rst
 * See :func:`nanogui::nvg_is_image_icon` for details.
 * \endrst
 *
 * \param value
 *     The integral value of the icon.
 *
 * \return
 *     Whether or not this is a font icon (from ``entypo.ttf``).
 */
inline bool nvg_is_font_icon(int value) { return value >= 1024; }


/// Check for OpenGL errors and warn if one is found (returns 'true' in that case')
extern NANOGUI_EXPORT bool nanogui_check_glerror(const char *cmd);

NAMESPACE_END(nanogui)
