#pragma once

// include OpenGL
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include "GL/glew.h"
#include <GLFW/glfw3.h>

// include tinyobjloader for Wavefront OBJ format import
#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"

// include stb image for texture image import
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

// include glm for vector/matrix math
#define GLM_FORCE_SWIZZLE
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

// include Shader.h (gl materialShader helper)
#include "Shader.h"
#include "Model.h"
#include "Texture.h"
#include "Scene.h"
#include "Camera.h"
#include "Animator.h"

// include standard libraries
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <string>
#include <algorithm>

// forward declaration of functions
void printGLContextInfo();
void printGLError();

// print OpenGL context related information
void printGLContextInfo()
{
	printf("GL_VENDOR: %s\n", glGetString (GL_VENDOR));
	printf("GL_RENDERER: %s\n", glGetString (GL_RENDERER));
	printf("GL_VERSION: %s\n", glGetString (GL_VERSION));
	printf("GL_SHADING_LANGUAGE_VERSION: %s\n", glGetString (GL_SHADING_LANGUAGE_VERSION));
}

// get the OpenGL error code and print its text representation
// note that the error code is reset to GL_NO_ERROR after the glGetError() call
void printGLError()
{
    GLenum code = glGetError();
    switch(code)
    {
    case GL_NO_ERROR:
        std::cout << "GL_NO_ERROR" << std::endl;
        break;
    case GL_INVALID_ENUM:
        std::cout << "GL_INVALID_ENUM" << std::endl;
        break;
    case GL_INVALID_VALUE:
        std::cout << "GL_INVALID_VALUE" << std::endl;
        break;
    case GL_INVALID_OPERATION:
        std::cout << "GL_INVALID_OPERATION" << std::endl;
        break;
    case GL_INVALID_FRAMEBUFFER_OPERATION:
        std::cout << "GL_INVALID_FRAMEBUFFER_OPERATION" << std::endl;
        break;
    case GL_OUT_OF_MEMORY:
        std::cout << "GL_OUT_OF_MEMORY" << std::endl;
        break;
    case GL_STACK_UNDERFLOW:
        std::cout << "GL_STACK_UNDERFLOW" << std::endl;
        break;
    case GL_STACK_OVERFLOW:
        std::cout << "GL_STACK_OVERFLOW" << std::endl;
        break;
    default:
        std::cout << "GL_ERROR (" << std::hex << code << std::dec << ")" << std::endl;
    }
}
