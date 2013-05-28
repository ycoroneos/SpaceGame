#ifndef GLF_H_INCLUDED
#define GLF_H_INCLUDED

#include "macros.h"
#include <GL/glew.h>
#include <GL/glfw.h>
#include <glm/glm.hpp>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "texture_loader.h"
#include <vector>
//#include <assimp/Importer.hpp>
//#include <assimp/scene.h>
//#include <assimp/postprocess.h>

struct Vertex_t
{
	glm::vec4 position;
	glm::vec3 normal;
	glm::vec2 texture;
};

struct Mesh
{
	GLuint vao, vbo, ibo, texID;
	glm::mat4x4* pose;               //should theoritically be in relation to parent but physics engine may change that
	char* name;
	std::vector<Mesh> children;
};

struct ModelObject
{
	char* name;
	glm::mat4x4* root_pose;
	std::vector<Mesh> submeshes;
};


void compilestatus(GLuint shader, GLenum type);
GLchar* readfile(const char* filename);
Vertex_t* loadobj(const char* filename, std::vector<Mesh>* submeshes);
namespace glf
{
	GLuint loadshader(const char* vprog, const char* fprog);
	bool initmesh(Mesh* mesh);
	bool loadmodel(ModelObject* object, const char* name, const char* texture, const char* geometry);
	void addsubmesh(ModelObject* object, Mesh* submesh, const char* submesh_parent);
}

#endif // GLF_H_INCLUDED
