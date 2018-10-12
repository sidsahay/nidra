#include "animatedcharacterdata.hpp"

glm::mat4 aiMatrix4x4ToGlm(const aiMatrix4x4 &from) {
    glm::mat4 to;
    //the a,b,c,d in assimp is the row ; the 1,2,3,4 is the column
    to[0][0] = from.a1;
    to[1][0] = from.a2;
    to[2][0] = from.a3;
    to[3][0] = from.a4;
    to[0][1] = from.b1;
    to[1][1] = from.b2;
    to[2][1] = from.b3;
    to[3][1] = from.b4;
    to[0][2] = from.c1;
    to[1][2] = from.c2;
    to[2][2] = from.c3;
    to[3][2] = from.c4;
    to[0][3] = from.d1;
    to[1][3] = from.d2;
    to[2][3] = from.d3;
    to[3][3] = from.d4;
    return to;
}

AnimatedCharacterData::AnimatedCharacterData(const aiScene *scene, const char *vshaderPath, const char *fshaderPath,
                                             const char *texturePath) : scene(scene) {
    this->vshaderPath = std::string(vshaderPath);
    this->fshaderPath = std::string(fshaderPath);
    this->texturePath = std::string(texturePath);

    //Fill in the animation nodes. Only uses the first animation for now.
    //Caveat: ALL BONES MUST BE ANIMATED. Yeah.

    for (auto i = 0; i < scene->mAnimations[0]->mNumChannels; i++) {
        nodeNameToAnim[std::string(
                scene->mAnimations[0]->mChannels[i]->mNodeName.data)] = scene->mAnimations[0]->mChannels[i];
    }

    numMeshes = scene->mNumMeshes;

    for (auto meshIdx = 0; meshIdx < numMeshes; meshIdx++) {
        std::vector<unsigned short> _indices;
        std::vector<glm::vec3> _vertices;
        std::vector<glm::vec2> _uvs;
        std::vector<glm::vec3> _normals;
        std::vector<VertexBoneData> _vboneData;

        indices.push_back(_indices);
        vertices.push_back(_vertices);
        uvs.push_back(_uvs);
        normals.push_back(_normals);
        vboneDatas.push_back(_vboneData);

        const auto *mesh = scene->mMeshes[meshIdx];

        //Get vertices from mesh data

        vertices[meshIdx].clear();
        vboneDatas[meshIdx].clear();

        for (auto i = 0; i < mesh->mNumVertices; i++) {
            aiVector3D pos = mesh->mVertices[i];
            vertices[meshIdx].push_back(glm::vec3(pos.x, pos.y, pos.z));
            VertexBoneData v{-1, -1, -1, -1, 0, 0, 0, 0};
            vboneDatas[meshIdx].push_back(v);
        }

        //Get bones from mesh data
        for (auto i = 0; i < mesh->mNumBones; i++) {
            auto name = std::string(mesh->mBones[i]->mName.data);
            const auto &foundBoneName = boneNameToInfo.find(name);

            if (foundBoneName == boneNameToInfo.end()) {
                boneNameToInfo[name] = {boneNumber, nodeNameToAnim[name],
                                        aiMatrix4x4ToGlm(mesh->mBones[i]->mOffsetMatrix)};
                boneNumber++;
                boneTransforms.emplace_back(glm::mat4(1.f));
                bonePositions.emplace_back(glm::vec3(0.f));
            }
        }

        //Get vbone data

        for (auto i = 0; i < mesh->mNumBones; i++) {
            auto boneName = std::string(mesh->mBones[i]->mName.data);
            for (auto j = 0; j < mesh->mBones[i]->mNumWeights; j++) {
                auto w = mesh->mBones[i]->mWeights[j];

                for (int h = 0; h < 4; h++) {
                    if (vboneDatas[meshIdx][w.mVertexId].boneWeights[h] == 0.f) {
                        vboneDatas[meshIdx][w.mVertexId].boneIds[h] = boneNameToInfo[boneName].id;
                        vboneDatas[meshIdx][w.mVertexId].boneWeights[h] = w.mWeight;
                        break;
                    }
                }
            }
        }

        // Get UVs

        uvs[meshIdx].reserve(numMeshes);

        for (auto i = 0; i < mesh->mNumVertices; i++) {
            aiVector3D UVW = mesh->mTextureCoords[0][i];
            uvs[meshIdx].push_back(glm::vec2(UVW.x, UVW.y));
        }

        // Get normals

        normals[meshIdx].reserve(numMeshes);

        for (auto i = 0; i < mesh->mNumVertices; i++) {
            aiVector3D n = mesh->mNormals[i];
            normals[meshIdx].push_back(glm::vec3(n.x, n.y, n.z));
        }

        // Get indices

        indices[meshIdx].reserve(3 * numMeshes);

        for (auto i = 0; i < mesh->mNumFaces; i++) {
            // Triangles only
            indices[meshIdx].push_back(static_cast<unsigned short &&>(mesh->mFaces[i].mIndices[0]));
            indices[meshIdx].push_back(static_cast<unsigned short &&>(mesh->mFaces[i].mIndices[1]));
            indices[meshIdx].push_back(static_cast<unsigned short &&>(mesh->mFaces[i].mIndices[2]));
        }

    }
}

void AnimatedCharacterData::UpdateBones() {
    if (currentClip == 0) {
        if (nextClip != 0) {
            currentClip = nextClip;
            nextClip = 0;
            currentAnimationIndex = animationClips[currentClip].first;
        } else {
            if (currentAnimationIndex < animationClips[currentClip].second) {
                currentAnimationIndex++;
            } else {
                currentAnimationIndex = animationClips[currentClip].first;
            }
        }
    } else {
        if (currentAnimationIndex < animationClips[currentClip].second) {
            currentAnimationIndex++;
        } else {
            currentClip = 0;
            currentAnimationIndex = animationClips[currentClip].first;
        }
    }

    _UpdateBonesRec(scene->mRootNode, glm::mat4(1.f));
}

void AnimatedCharacterData::_UpdateBonesRec(aiNode *node, glm::mat4 parentTransform) {
    std::string name(node->mName.data);
    const auto &found = nodeNameToAnim.find(name);

    glm::mat4 nodeTransform = aiMatrix4x4ToGlm(node->mTransformation);

    if (found != nodeNameToAnim.end()) {
        const auto *nodeAnim = found->second;

        auto boneTranslateVec = nodeAnim->mPositionKeys[currentAnimationIndex].mValue;
        auto boneTranslateGlm = glm::translate(glm::mat4(1.f),
                                               glm::vec3(boneTranslateVec.x, boneTranslateVec.y, boneTranslateVec.z));

        auto boneQuat = nodeAnim->mRotationKeys[currentAnimationIndex].mValue;
        auto boneRotationGlm = glm::mat4_cast(glm::quat{boneQuat.w, boneQuat.x, boneQuat.y, boneQuat.z});

        nodeTransform = boneTranslateGlm * boneRotationGlm;
    }

    glm::mat4 overallTransform = parentTransform * nodeTransform;

    auto boneFound = boneNameToInfo.find(name);

    if (boneFound != boneNameToInfo.end()) {
        unsigned int id = boneFound->second.id;
        boneTransforms[id] = overallTransform * boneFound->second.offsetMatrix;
        bonePositions[id] = glm::vec3(overallTransform * glm::vec4(0., 0., 0., 1.));
    }

    for (auto i = 0; i < node->mNumChildren; i++) {
        _UpdateBonesRec(node->mChildren[i], overallTransform);
    }
}

AnimatedCharacterData::~AnimatedCharacterData() {}
