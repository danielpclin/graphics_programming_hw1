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


// Change loadobj format from tinyobj V1.X to V0.9.X 
// in order not to change OLD Lecture Shader too much Zzz...
// You can write your own loadObj by yourself.
//typedef struct _MeshData
//{
//	// if OBJ preserves vertex order, you can use element array buffer for memory efficiency
//	// If no data return empty vector
//	std::vector<float> positions;
//	std::vector<float> normals;
//	std::vector<float> texcoords;
//	std::vector<unsigned int> indices;
//	std::vector<unsigned char> num_vertices;
//	std::vector<int> material_ids; // per-face material ID
//} MeshData;

// load obj file
//std::vector<MeshData> loadObj(const char* const objFilePath)
//{
//	tinyobj::attrib_t attrib;
//	std::vector<tinyobj::shape_t> shapes;
//	std::vector<tinyobj::material_t> materials;
//	std::string warn, err;
//	bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, objFilePath);
//	printf("Load Models ! Shapes size %zd Material size %zd\n", shapes.size(), materials.size());
//	if (!warn.empty()) {
//		std::cout << warn << std::endl;
//	}
//	if (!err.empty()) {
//		std::cout << err << std::endl;
//	}
//	if (!ret) {
//		exit(1);
//	}
//
//	std::vector<MeshData> meshes;
//
//	for (int s = 0; s < shapes.size(); ++s) {
//		MeshData mesh;
//
//		int index_offset = 0;
//		for (int f = 0; f < shapes[s].mesh.num_face_vertices.size(); ++f) {
//			int fv = shapes[s].mesh.num_face_vertices[f];
//			for (int v = 0; v < fv; ++v) {
//				tinyobj::index_t idx = shapes[s].mesh.indices[index_offset + v];
//				if (idx.vertex_index != -1) {
//					mesh.positions.push_back(attrib.vertices[3 * idx.vertex_index + 0]);
//					mesh.positions.push_back(attrib.vertices[3 * idx.vertex_index + 1]);
//					mesh.positions.push_back(attrib.vertices[3 * idx.vertex_index + 2]);
//				}
//				if (idx.texcoord_index != -1) {
//					mesh.texcoords.push_back(attrib.texcoords[2 * idx.texcoord_index + 0]);
//					mesh.texcoords.push_back(attrib.texcoords[2 * idx.texcoord_index + 1]);
//				}
//				if (idx.normal_index != -1) {
//					mesh.normals.push_back(attrib.normals[3 * idx.normal_index + 0]);
//					mesh.normals.push_back(attrib.normals[3 * idx.normal_index + 1]);
//					mesh.normals.push_back(attrib.normals[3 * idx.normal_index + 2]);
//				}
//				mesh.indices.push_back(index_offset + v);
//			}
//			index_offset += fv;
//		}
//
//		meshes.push_back(mesh);
//	}
//
//	return meshes;
//}