#include "GPGPU.h"
#include <iostream>
#include <fstream>

GPGPU::GPGPU(int w, int h) : _initialized(0), _width(w), _height(h)
{
    // Create a texture to store the framebuffer
    glGenTextures(1, &_textureId);
    glBindTexture(GL_TEXTURE_2D, _textureId);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, _width, _height, 0, GL_RGB, GL_FLOAT, 0);

    _programId = glCreateProgram();

	// Load fragment shader which will be used as computational kernel
	std::string edgeFragSource2;
	loadShader("fragment.glsl", edgeFragSource2);

    // Create the fragment program
    _fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	const char* source = edgeFragSource2.c_str();
    glShaderSource(_fragmentShader, 1, &source, NULL);
    glCompileShader(_fragmentShader);
    glAttachShader(_programId, _fragmentShader);

    // Link the shader into a complete GLSL program.
    glLinkProgram(_programId);

	// Check program
	{
		int infologLength = 0;
		glGetProgramiv(_programId, GL_INFO_LOG_LENGTH, &infologLength);
		if (infologLength > 0) {
			char *infoLog = (char *)malloc(infologLength);
			glGetProgramInfoLog(_programId, infologLength, NULL, infoLog);
			printf("%s\n",infoLog);
			free(infoLog);
		}
	}

    // Get location of the uniform variables
    _texUnitLoc = glGetUniformLocation(_programId, "texUnit");
	_tLoc = glGetUniformLocation(_programId, "t");
	_centerLoc = glGetUniformLocation(_programId, "center");
	_frequencyLoc = glGetUniformLocation(_programId, "frequency");
	_amplitudeLoc = glGetUniformLocation(_programId, "amplitude");
	_lightDirLoc = glGetUniformLocation(_programId, "lightDir");
}

void GPGPU::update(float t)
{
    // Backup the viewport dimensions
    int vp[4];
    glGetIntegerv(GL_VIEWPORT, vp);

    glViewport(0, 0, _width, _height);
        
    // Copy the results to the texture
	glBindTexture(GL_TEXTURE_2D, _textureId);

	glUseProgram(_programId);
            
    // Set the uniform variables
    glUniform1i(_texUnitLoc, 0);
	float center[] = {0.3, 0.3, 0.8, 0.7, 0.4, 0.9};
	float frequency[] = {100.0, 80.0, 50.0};
	float amplitude[] = {0.33333, 0.33333, 0.33333};
	float lightDir[] = {0.0, 0.0, 1.0};
	glUniform2fv(_centerLoc, 3, center);
	glUniform1fv(_frequencyLoc, 3, frequency);
	glUniform1fv(_amplitudeLoc, 3, amplitude);
	glUniform3fv(_lightDirLoc, 1, lightDir);
	glUniform1f(_tLoc, t);
            
	// By drawing a quad, the fragment shader will be called for each pixel.
    glBegin(GL_QUADS);
    {            
        glTexCoord2f(0, 0); glVertex3f(-1, -1, -0.5f);
        glTexCoord2f(1, 0); glVertex3f( 1, -1, -0.5f);
        glTexCoord2f(1, 1); glVertex3f( 1,  1, -0.5f);
        glTexCoord2f(0, 1); glVertex3f(-1,  1, -0.5f);
    }
    glEnd();

    glUseProgram(0);
        
    // Copy the rendered image to the texture so that we can start from the current state in the next iteration
    glBindTexture(GL_TEXTURE_2D, _textureId);
    glCopyTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 0, 0, _width, _height);
        
    // Restore the stored viewport dimensions
	glViewport(vp[0], vp[1], vp[2], vp[3]);
}

void GPGPU::display()
{
    // Bind the texture
    glBindTexture(GL_TEXTURE_2D, _textureId);
    glEnable(GL_TEXTURE_2D);

    // Render the result image.
    glBegin(GL_QUADS);
    {
        glTexCoord2f(0, 0); glVertex3f(-1, -1, -0.5f);
        glTexCoord2f(1, 0); glVertex3f( 1, -1, -0.5f);
        glTexCoord2f(1, 1); glVertex3f( 1,  1, -0.5f);
        glTexCoord2f(0, 1); glVertex3f(-1,  1, -0.5f);
    }
    glEnd();

    glDisable(GL_TEXTURE_2D);
}

int GPGPU::loadShader(char* filename, std::string& text)
{
	std::ifstream ifs;
	ifs.open(filename, std::ios::in);

	std::string line;
	while (ifs.good()) {
        getline(ifs, line);

		text += line + "\n";
    }

	return 0;
}