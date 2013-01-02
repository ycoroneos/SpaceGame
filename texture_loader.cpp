#include "texture_loader.h"

void initFreeImage()
{
    FreeImage_Initialise();
    DEBUG_PRINT(("FreeImage version: "));
    PUTS((FreeImage_GetVersion()));
}

void destroyFreeImage()
{
    FreeImage_DeInitialise();
}

GLuint loadtexture(const char* file)
{
    FREE_IMAGE_FORMAT format=FreeImage_GetFileType(file);
    FIBITMAP* bitmap=FreeImage_Load(format, file);
    if (!bitmap)
    {
        DEBUG_PRINT(("could not load texture: "));
        PUTS((file));
        DEBUG_PRINT(("\n"));
        return -1;
    }
    FIBITMAP* temp=bitmap;
    bitmap=FreeImage_ConvertTo32Bits(bitmap);
    FreeImage_Unload(temp);
    temp=NULL;
    int w=FreeImage_GetWidth(bitmap);
    int h=FreeImage_GetHeight(bitmap);
    GLubyte* data=new GLubyte[4*w*h];
    char* pixels=(char*)FreeImage_GetBits(bitmap);
    for(int j= 0; j<w*h; j++){
		data[j*4+0]= pixels[j*4+2];
		data[j*4+1]= pixels[j*4+1];
		data[j*4+2]= pixels[j*4+0];
		data[j*4+3]= pixels[j*4+3];
	}
	GLuint texID;
	glGenTextures(1, &texID);
	glBindTexture(GL_TEXTURE_2D, texID);
	glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA, w, h, 0, GL_RGBA,GL_UNSIGNED_BYTE,(GLvoid*)data );
	glGenerateMipmap(GL_TEXTURE_2D);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR); //change these for quality
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	if (glewIsSupported("GL_EXT_texture_filter_anisotropic"))
	{
	    DEBUG_PRINT(("anistrophy is supported, setting to max level\n"));
	    GLfloat max_lvl;
	    glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &max_lvl);
	    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, max_lvl);
	}
	glBindTexture(GL_TEXTURE_2D,0);
	FreeImage_Unload(bitmap);
    return texID;
}
