#ifndef NIDRA_CODE_ANIMATEDCHARACTERDATA_HPP
#define NIDRA_CODE_ANIMATEDCHARACTERDATA_HPP

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <iostream>
#include <map>
#include <assimp/scene.h>
#include <vector>

using namespace glm;

glm::mat4 aiMatrix4x4ToGlm(const aiMatrix4x4 &from);

struct VertexBoneData {
    int boneIds[4] = {0};
    float boneWeights[4] = {0.f, 0.f, 0.f, 0.f};
};

struct BoneInfo {
    unsigned int id;
    aiNodeAnim *nodeAnim;
    glm::mat4 offsetMatrix;
};

struct AnimatedCharacterData {
    explicit AnimatedCharacterData(const aiScene *scene, const char *vshaderPath, const char *fshaderPath,
                                   const char *texturePath);

    ~AnimatedCharacterData();

    const aiScene *scene;

    unsigned int numMeshes = 0;
    unsigned int boneNumber = 0;

    std::vector<std::vector<unsigned short>> indices;
    std::vector<std::vector<glm::vec3>> vertices;
    std::vector<std::vector<glm::vec2>> uvs;
    std::vector<std::vector<glm::vec3>> normals;
    std::vector<std::vector<VertexBoneData>> vboneDatas;

    std::map<std::string, BoneInfo> boneNameToInfo;
    std::map<std::string, aiNodeAnim *> nodeNameToAnim;
    std::vector<glm::mat4> boneTransforms;
    std::vector<glm::vec3> bonePositions;

    std::vector<std::pair<unsigned int, unsigned int>> animationClips;

    std::string vshaderPath;
    std::string fshaderPath;
    std::string texturePath;

    unsigned int currentClip = 0;
    unsigned int nextClip = 0;

    unsigned int currentAnimationIndex = 0;

    void UpdateBones();

    void _UpdateBonesRec(aiNode *node, glm::mat4 parentTransform);
};

#endif //NIDRA_CODE_ANIMATEDCHARACTERDATA_HPP
