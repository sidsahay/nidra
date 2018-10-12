#ifndef NIDRA_CODE_ANIMATEDCHARACTERBUFFERS_HPP
#define NIDRA_CODE_ANIMATEDCHARACTERBUFFERS_HPP

#include "animatedcharacterdata.hpp"

#include "shader.hpp"
#include "texture.hpp"

struct AnimatedCharacterBuffers
{
    explicit AnimatedCharacterBuffers(const AnimatedCharacterData& character);

    ~AnimatedCharacterBuffers();

    const AnimatedCharacterData& character;

    std::vector<GLuint> vertexBuffers;
    std::vector<GLuint> uvBuffers;
    std::vector<GLuint> normalBuffers;
    std::vector<GLuint> elementBuffers;
    std::vector<GLuint> vboneBuffers;

    GLint matrixID;
    GLint viewMatrixID;
    GLint modelMatrixID;
    GLint lightID;
    GLint boneXformArrayID;

    GLuint shaderProgramID;
    GLuint textureID;

    GLint textureSamplerID;

    bool isBuilt = false;

    void Build();

    void Render();

    void UpdateLight(const vec3 &light);
};

#endif //NIDRA_CODE_ANIMATEDCHARACTERBUFFERS_HPP
