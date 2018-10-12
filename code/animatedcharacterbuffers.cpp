#include <GL/glew.h>
#include "animatedcharacterbuffers.hpp"


AnimatedCharacterBuffers::AnimatedCharacterBuffers(const AnimatedCharacterData &character) : character(character) {}

AnimatedCharacterBuffers::~AnimatedCharacterBuffers() {
    if (isBuilt) {
        glDeleteBuffers(character.numMeshes, &vertexBuffers[0]);
        glDeleteBuffers(character.numMeshes, &uvBuffers[0]);
        glDeleteBuffers(character.numMeshes, &normalBuffers[0]);
        glDeleteBuffers(character.numMeshes, &elementBuffers[0]);
        glDeleteBuffers(character.numMeshes, &vboneBuffers[0]);

        glDeleteProgram(shaderProgramID);
        glDeleteTextures(1, &textureID);
    }
}

void AnimatedCharacterBuffers::Build() {
    shaderProgramID = LoadShaders(character.vshaderPath.c_str(), character.fshaderPath.c_str());
    textureID = loadBMP_custom(character.texturePath.c_str());

    textureSamplerID = glGetUniformLocation(shaderProgramID, "diffuseTextureSampler");

    glUseProgram(shaderProgramID);
    matrixID = glGetUniformLocation(shaderProgramID, "MVP");
    viewMatrixID = glGetUniformLocation(shaderProgramID, "V");
    modelMatrixID = glGetUniformLocation(shaderProgramID, "M");

    lightID = glGetUniformLocation(shaderProgramID, "LightPosition_worldspace");
    boneXformArrayID = glGetUniformLocation(shaderProgramID, "boneXformArray");

    vertexBuffers.reserve(character.numMeshes);

    glGenBuffers(character.numMeshes, &vertexBuffers[0]);
    for (auto i = 0; i < character.numMeshes; i++) {
        glBindBuffer(GL_ARRAY_BUFFER, vertexBuffers[i]);
        glBufferData(GL_ARRAY_BUFFER, character.vertices[i].size() * sizeof(glm::vec3), &character.vertices[i][0],
                     GL_STATIC_DRAW);
    }

    uvBuffers.reserve(character.numMeshes);

    glGenBuffers(character.numMeshes, &uvBuffers[0]);

    for (auto i = 0; i < character.numMeshes; i++) {
        glBindBuffer(GL_ARRAY_BUFFER, uvBuffers[i]);
        glBufferData(GL_ARRAY_BUFFER, character.uvs[i].size() * sizeof(glm::vec2), &character.uvs[i][0],
                     GL_STATIC_DRAW);
    }

    normalBuffers.reserve(character.numMeshes);

    glGenBuffers(character.numMeshes, &normalBuffers[0]);

    for (auto i = 0; i < character.numMeshes; i++) {
        glBindBuffer(GL_ARRAY_BUFFER, normalBuffers[i]);
        glBufferData(GL_ARRAY_BUFFER, character.normals[i].size() * sizeof(glm::vec3), &character.normals[i][0],
                     GL_STATIC_DRAW);
    }

    elementBuffers.reserve(character.numMeshes);

    glGenBuffers(character.numMeshes, &elementBuffers[0]);

    for (auto i = 0; i < character.numMeshes; i++) {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBuffers[i]);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, character.indices[i].size() * sizeof(unsigned short),
                     &character.indices[i][0], GL_STATIC_DRAW);
    }

    // Generate a buffer for vertexBoneData
    vboneBuffers.reserve(character.numMeshes);

    glGenBuffers(character.numMeshes, &vboneBuffers[0]);

    for (auto i = 0; i < character.numMeshes; i++) {
        glBindBuffer(GL_ARRAY_BUFFER, vboneBuffers[i]);
        glBufferData(GL_ARRAY_BUFFER, character.vboneDatas[i].size() * sizeof(VertexBoneData),
                     &character.vboneDatas[i][0], GL_STATIC_DRAW);
    }

    isBuilt = true;
}

void AnimatedCharacterBuffers::Render() {
    glUseProgram(shaderProgramID);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glUniform1i(textureSamplerID, 0);

    for (auto i = 0; i < character.numMeshes; i++) {
        //vertices are attribute 0
        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, vertexBuffers[i]);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void *) 0);

        //UVs are attribute 1
        glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, uvBuffers[i]);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void *) 0);

        //normals are attribute 2
        glEnableVertexAttribArray(2);
        glBindBuffer(GL_ARRAY_BUFFER, normalBuffers[i]);
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (void *) 0);

        //vbone ids are attribute 3
        glEnableVertexAttribArray(3);
        glBindBuffer(GL_ARRAY_BUFFER, vboneBuffers[i]);
        glVertexAttribIPointer(3, 4, GL_INT, sizeof(VertexBoneData), (void *) 0);

        //vbone influences are attribute 4
        glEnableVertexAttribArray(4);
        glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(VertexBoneData), (void *) 16);

        //indices
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBuffers[i]);

        //draw stuff
        glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(character.indices[i].size()), GL_UNSIGNED_SHORT, (void *) 0);

        //disarm attributes
        glDisableVertexAttribArray(0);
        glDisableVertexAttribArray(1);
        glDisableVertexAttribArray(2);
        glDisableVertexAttribArray(3);
        glDisableVertexAttribArray(4);
    }
}

void AnimatedCharacterBuffers::UpdateLight(const vec3 &light) {
    glUniform3f(lightID, light.x, light.y, light.z);
}

