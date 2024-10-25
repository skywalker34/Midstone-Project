#include "ComputeShader.h"
#include <string.h>

ComputeShader::ComputeShader(const char* compFilename_) :
    computeProgram(0), compShaderID(0), compFilename(compFilename_) { }

ComputeShader::~ComputeShader() {}

bool ComputeShader::OnCreate() {
    bool status;
    status = CompileAttach();
    if (status == false) return false;
    status = Link();
    if (status == false) return false;

    SetUniformLocations();
    return true;
}

void ComputeShader::OnDestroy() {
    glDetachShader(computeProgram, compShaderID);
    glDeleteShader(compShaderID);
    glDeleteProgram(computeProgram);
}

bool ComputeShader::CompileAttach() {
    GLint status;
    const char* compText = nullptr;

    try {
        compText = ReadTextFile(compFilename);
        if (compText == nullptr) {
            return false;
        }
        compShaderID = glCreateShader(GL_COMPUTE_SHADER);
        if (compShaderID == 0) {
            std::string errorMsg("Can't create a new compute shader");
            throw errorMsg;
        }

        glShaderSource(compShaderID, 1, &compText, 0);
        glCompileShader(compShaderID);
        glGetShaderiv(compShaderID, GL_COMPILE_STATUS, &status);
        if (status == 0) {
            GLsizei errorLogSize = 0;
            GLsizei titleLength;
            std::string errorLog = compFilename;
            errorLog += "\nCOMP:\n";
            titleLength = errorLog.length();
            glGetShaderiv(compShaderID, GL_INFO_LOG_LENGTH, &errorLogSize);
            errorLog.resize(titleLength + errorLogSize);
            glGetShaderInfoLog(compShaderID, errorLogSize, &errorLogSize, &errorLog[titleLength]);
            throw errorLog;
        }

        computeProgram = glCreateProgram();
        glAttachShader(computeProgram, compShaderID);

        if (compText) delete[] compText;

    }
    catch (std::string error) {
        printf("%s\n", &error[0]);
        Debug::Error(error, __FILE__, __LINE__);
        return false;
    }
    return true;
}

bool ComputeShader::Link() {
    GLint status;
    try {
        glLinkProgram(computeProgram);
        glGetProgramiv(computeProgram, GL_LINK_STATUS, &status);
        if (status == 0) {
            GLsizei errorLogSize = 0;
            std::string errorLog;
            glGetProgramiv(computeProgram, GL_INFO_LOG_LENGTH, &errorLogSize);
            errorLog.resize(errorLogSize);
            glGetProgramInfoLog(computeProgram, errorLogSize, &errorLogSize, &errorLog[0]);
            throw errorLog;
        }

    }
    catch (std::string error) {
        Debug::Error(error, __FILE__, __LINE__);
        return false;
    }


   
    return true;
}








GLuint ComputeShader::GetUniformID(std::string name) {
    auto id = uniformMap.find(name);
#ifdef _DEBUG
    static bool writeBadUniformWarning = true;
    if (id == uniformMap.end()) {
        if (writeBadUniformWarning) {
            Debug::Warning(name + " :No such uniform name", __FILE__, __LINE__);
            writeBadUniformWarning = false;/// I already told you, don't repeat the message 
        }
        return -1;
    }
#endif 
    return id->second;
}

void ComputeShader::SetUniformLocations() {
    int count;
    GLsizei actualLen;
    GLint size;
    GLenum type;
    char* name;
    int maxUniformListLength;
    unsigned int loc;
    char buffer[512];

    std::string activeUniformList = "Uniforms:\n";
    glGetProgramiv(computeProgram, GL_ACTIVE_UNIFORMS, &count);
    sprintf_s(buffer, 512, "There are %d active Uniforms\n", count);

    /// get the length of the longest named uniform 
    glGetProgramiv(computeProgram, GL_ACTIVE_UNIFORM_MAX_LENGTH, &maxUniformListLength);

    /// Create a little buffer to hold the uniform's name - old C memory call just for fun 
    name = (char*)malloc(sizeof(char) * maxUniformListLength);


    for (int i = 0; i < count; ++i) {

        /// Get the name of the ith uniform

        glGetActiveUniform(computeProgram, i, maxUniformListLength, &actualLen, &size, &type, name);
        /// Get the (unsigned int) loc for this uniform
        loc = glGetUniformLocation(computeProgram, name);
        std::string uniformName = name;
        uniformMap[uniformName] = loc;

        sprintf_s(buffer, 512, "\"%s\" loc:%d\n", uniformName.c_str(), uniformMap[uniformName]);
        activeUniformList += buffer;
    }
    Debug::Info(activeUniformList, __FILE__, __LINE__);
    free(name);
}




char* ComputeShader::ReadTextFile(const char* filename) {
    char* buffer = nullptr;
    std::ifstream file(filename, std::ios::binary);
    if (file.is_open()) {
        file.seekg(0, std::ios::end);
        int bufferSize = (int)file.tellg();
        if (bufferSize == 0) {
            std::string errorMsg("Can't read shader file: ");
            std::string str2(filename);
            errorMsg += str2;
            throw errorMsg;
        }
        buffer = new char[(int)(bufferSize + 1)];
        file.seekg(0, std::ios::beg);
        file.read(buffer, bufferSize);
        buffer[bufferSize] = NULL;
        file.close();
    }
    else {
        std::string errorMsg("Can't open shader file: ");
        errorMsg += filename;
        Debug::Error(errorMsg, __FILE__, __LINE__);
    }
    return buffer;
}
