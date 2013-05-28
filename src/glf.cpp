#include "glf.h"
//GL_COMPILE_STATUS
//GL_LINK_STATUS

GLchar* readfile(const char* name)
{
	FILE* file=fopen(name, "r");
	fseek(file, 0, SEEK_END);
	int length = ftell(file);
	rewind(file);
	GLchar* source = (GLchar*) malloc (sizeof(GLchar)*(length));
	fread(source,1,(length),file);
	fclose(file);
	free(file);
	return source;
}
void compilestatus(GLuint shader, GLenum type)
{
	GLint compile_status;
	glGetShaderiv(shader, type, &compile_status);
	if (compile_status==GL_FALSE)
	{
		glDeleteShader(shader);
		char infolog[1024];
		glGetShaderInfoLog(shader, 1024, NULL, infolog);
		puts(infolog);
		printf("\n");
	}
}

Vertex_t* loadobj(const char* filename, std::vector<Mesh>* submeshes)
{/*
	Assimp::Importer importer;
	const aiScene* scene=importer.ReadFile(filename, aiProcessPreset_TargetRealtime_Quality);
	aiNode* root=scene->mRootNode;
	(*meshes)=new Mesh[root->mNumChildren];
	for (int i=0; i<root->mNumChildren; ++i)
	{
		aiNode* cur_node=root->mChildren[i];
		(*meshes[i]).name=new char[cur_node->mName.length];
		strncpy((*meshes[i]).name, cur_node->mName.C_Str(), cur_node->mName.length);
		PUTS((cur_node->mName.C_Str()));
		DEBUG_PRINT(("\n"));
		glf::initmesh(meshes[i]);
	}
	*/
}
/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////

GLuint glf::loadshader(const char* vprog, const char* fprog)
{
	GLuint vshader, fshader, program;
	vshader=glCreateShader(GL_VERTEX_SHADER);
	fshader=glCreateShader(GL_FRAGMENT_SHADER);
	GLchar* vsource=readfile(vprog);
	if (vsource==NULL)
	{
		glDeleteShader(vshader);
		glDeleteShader(fshader);
		DEBUG_PRINT(("error loading vertex shader\n"));
		return GL_FALSE;
	}
	GLchar* fsource=readfile(fprog);
	if (fsource==NULL)
	{
		glDeleteShader(vshader);
		glDeleteShader(fshader);
		DEBUG_PRINT(("error loading vertex shader\n"));
		return GL_FALSE;
	}
	glCompileShader(vshader);
	glCompileShader(fshader);
	compilestatus(vshader, GL_COMPILE_STATUS);
	compilestatus(fshader, GL_COMPILE_STATUS);
	program=glCreateProgram();
	glAttachShader(program, vshader);
	glAttachShader(program, fshader);
	glLinkProgram(program);
	glDeleteShader(vshader);
	glDeleteShader(fshader);
	compilestatus(program, GL_LINK_STATUS);
	return program;
}
bool glf::initmesh(Mesh* mesh)
{
	glGenVertexArrays(1, &mesh->vao);
	if (mesh->vao<0)
	{
		return false;
	}
	glGenBuffers(1, &mesh->vbo);
	if (mesh->vbo<0)
	{
		return false;
	}
	glGenBuffers(1, &mesh->ibo);
	if (mesh->ibo<0)
	{
		return false;
	}
	return true;
}
bool glf::loadmodel(ModelObject* object, const char* name, const char* texture, const char* geometry)
{
	//object->texID=loadtexture(texture);
	//object->name=new char[sizeof(name)];
	//strncpy(object->name, name, sizeof(name));
	loadobj(geometry, &object->submeshes);
	return true;
}
