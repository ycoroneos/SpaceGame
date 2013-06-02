#include "glf.h"
using namespace std;
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

bool glf::removemesh(Mesh* mesh)
{
    glDeleteBuffers(1, &mesh->ibo);
    glDeleteBuffers(1, &mesh->vbo);
    glDeleteVertexArrays(1, &mesh->vao);
    delete[] mesh->name;
    delete mesh->pose;
    for (unsigned int i=0; i<mesh->children.size(); ++i)
    {
        removemesh(&mesh->children[i]);
    }
    return true;
}

bool glf::loadmodel(ModelObject* object, const char* name,const char* filename)
{
	Assimp::Importer importer;
	const aiScene* scene=importer.ReadFile(filename, aiProcess_CalcTangentSpace | aiProcess_Triangulate | aiProcess_JoinIdenticalVertices | aiProcess_SortByPType);
	if (!scene)
	{
        DEBUG_PRINT(("error loading scene\n"));
        return false;
	}
	aiNode* root=scene->mRootNode;
	aiMesh** meshes=scene->mMeshes;
	aiMaterial** materials=scene->mMaterials;
	processnodes(root, object, meshes, materials);
	PUTS((object->name));
	return true;
}

glm::mat4x4 glf::assimpMatToGlm(aiMatrix4x4 input)
{
    float vals[16];
    for (unsigned int i=0; i<16; ++i)
    {
        vals[i]=*input[i];
    }
    return glm::make_mat4x4(vals);
}

void glf::processnodes(aiNode* curNode, ModelObject* object, aiMesh** meshes, aiMaterial** materials)
{
    vector<Mesh>* children;
    if (curNode->mParent==NULL)
    {
        DEBUG_PRINT(("on root node\n"));
        children=&object->submeshes;
        object->root_pose=new glm::mat4x4(assimpMatToGlm(curNode->mTransformation));
    }
    else
    {
        Mesh* target=findmeshbfs(object, curNode->mParent->mName.C_Str());
        children=&target->children;
        DEBUG_PRINT(("found child\n"));
    }
    for (unsigned int i=0; i<curNode->mNumMeshes; ++i)
    {
        aiMesh* cur_mesh=meshes[curNode->mMeshes[i]];

        //init mesh
        Mesh tmesh;
        initmesh(&tmesh);
        DEBUG_PRINT(("initialized mesh\n"));

        //copy name
        tmesh.name=new char[cur_mesh->mName.length];
        strncpy(tmesh.name, cur_mesh->mName.C_Str(), cur_mesh->mName.length);
        DEBUG_PRINT(("copied name over: "));
        PUTS((tmesh.name));

        //copy pose
        tmesh.pose=new glm::mat4x4(assimpMatToGlm(curNode->mTransformation));
        DEBUG_PRINT(("copied pose\n"));

        //bind vao
        glBindVertexArray(tmesh.vao);
        DEBUG_PRINT(("bound vao\n"));

        //bind vbo
        glBindBuffer(GL_ARRAY_BUFFER, tmesh.vbo);
        DEBUG_PRINT(("bound vbo\n"));

        //fill vbo
        Vertex_t verts[cur_mesh->mNumVertices];
        for (unsigned int i=0; i<cur_mesh->mNumVertices; ++i)
        {
            verts[i].position=glm::vec4(cur_mesh->mVertices[i].x, cur_mesh->mVertices[i].y, cur_mesh->mVertices[i].z, 0);
            verts[i].normal=glm::vec3(cur_mesh->mNormals[i].x, cur_mesh->mNormals[i].y, cur_mesh->mNormals[i].z);
            verts[i].texture=glm::vec2(cur_mesh->mTextureCoords[0][i].x, cur_mesh->mTextureCoords[0][i].y);
            DEBUG_PRINT(("%f",verts[i].position[0]));
        }
        DEBUG_PRINT(("about to fill vbo\n"));
        glBufferData(GL_ARRAY_BUFFER, cur_mesh->mNumVertices*sizeof(Vertex_t), verts, GL_STATIC_DRAW);
        DEBUG_PRINT(("filled vbo\n"));
        glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex_t), (const GLvoid *)offsetof(Vertex_t, position));
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex_t), (const GLvoid *)offsetof(Vertex_t, normal));
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex_t), (const GLvoid *)offsetof(Vertex_t, texture));
        delete[] verts;
        DEBUG_PRINT(("freed memory\n"));

        //bind ibo
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, tmesh.ibo);
        DEBUG_PRINT(("bound ibo\n"));

        //fill ibo
        unsigned int indices[cur_mesh->mNumFaces*3];
        for (unsigned int i=0; i<cur_mesh->mNumFaces; ++i)
        {
            aiFace cface=cur_mesh->mFaces[i];
            if (cface.mNumIndices!=3)
            {
                DEBUG_PRINT(("THIS FACE IS NOT A TRIANGLE"));
                return;
            }
            else
            {
                for(unsigned int j=0; j<3; ++j)
                {
                    indices[(3*i)+j]=cface.mIndices[j];
                }
            }
        }
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int)*(cur_mesh->mNumFaces*3), indices, GL_STATIC_DRAW);
        tmesh.numIndices=cur_mesh->mNumFaces*3;
        delete[] indices;
        DEBUG_PRINT(("filled ibo"));

        //unbind vao to reset state
        glBindVertexArray(0);

        //load texture
        aiMaterial* material=materials[cur_mesh->mMaterialIndex];
        aiString texPath;
        material->GetTexture(aiTextureType_DIFFUSE, 0, &texPath);
        DEBUG_PRINT((texPath.C_Str()));
        tmesh.texID=loadtexture(texPath.C_Str());
        DEBUG_PRINT(("loaded texture\n"));

        //push child onto stack
        children->push_back(tmesh);
        DEBUG_PRINT(("pushed mesh onto stack\n"));
    }
    //repreat for all child nodes
    for (unsigned int i=0; i<curNode->mNumChildren; ++i)
    {
        processnodes(curNode->mChildren[i], object, meshes, materials);
    }
}

Mesh* glf::findmeshbfs(ModelObject* object, const char* name)
{
    deque<Mesh*> q;
    for (unsigned int i=0; i<object->submeshes.size(); ++i)
    {
        q.push_back(&object->submeshes[i]);
    }
    while (q.size()!=0)
    {
        Mesh* temp=q.front();
        q.pop_front();
        if (strncmp(name, temp->name, strlen(name))==0)
        {
            return temp;
        }
        else
        {
            for (unsigned int i=0; i<temp->children.size(); ++i)
            {
                q.push_back(&temp->children[i]);
            }
        }
    }
    return NULL;
}
