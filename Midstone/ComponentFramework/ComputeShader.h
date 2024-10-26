#ifndef COMPUTESHADER_H
#define COMPUTESHADER_H

#include "Debug.h"
#include <fstream>
#include <string.h>
#include "glew.h"
#include <unordered_map>

class ComputeShader {
public:
    ComputeShader(const char* compFilename_);
    ~ComputeShader();

    bool OnCreate();
    void OnDestroy();
    GLuint GetUniformID(std::string name);
    inline GLuint GetProgram() const { return computeProgram; }

private:
    bool CompileAttach();
    bool Link();
    char* ReadTextFile(const char* filename);
    void SetUniformLocations();

    GLuint computeProgram;
    GLuint compShaderID;
    const char* compFilename;
    std::unordered_map<std::string, GLuint > uniformMap;
};

#endif // COMPUTESHADER_H
