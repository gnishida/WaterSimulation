#pragma once

#define GLEW_STATIC 1
#include <GL/glew.h>
#include <GL/glut.h>
#include <string>

class GPGPU
{
public:
	GPGPU(int w, int h);

	void update(float t);
	void display();
	int loadShader(char* filename, std::string& text);

private:
    int _width;				// width of the screen
	int _height;			// height of the screen
	int _initialized;		// if the cells are initialized (=1) or not (=0)

	GLuint _tLoc;
    GLuint _texUnitLoc;
	GLuint _centerLoc;
	GLuint _frequencyLoc;
	GLuint _amplitudeLoc;
	GLuint _islandLoc;
	GLuint _lightDirLoc;
    
    GLuint _textureId;		// The texture ID used to store data array
    GLuint _programId;		// the program ID

    GLuint _fragmentShader;
};

