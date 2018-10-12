#include "animatedcharacter.hpp"

void AnimatedCharacter::UpdateMatrices(const glm::mat4 &P, const glm::mat4 &V, const glm::mat4 &M, const glm::mat4 &MVP)
{
    glUniformMatrix4fv(buffers.matrixID, 1, GL_FALSE, &MVP[0][0]);
    glUniformMatrix4fv(buffers.modelMatrixID, 1, GL_FALSE, &M[0][0]);
    glUniformMatrix4fv(buffers.viewMatrixID, 1, GL_FALSE, &V[0][0]);
}

void AnimatedCharacter::UpdateBones()
{
    data.UpdateBones();
    glUniformMatrix4fv(buffers.boneXformArrayID, static_cast<GLsizei>(data.boneTransforms.size()), GL_FALSE, &data.boneTransforms[0][0][0]);
}

void AnimatedCharacter::UpdateLight(const glm::vec3 &light)
{
    buffers.UpdateLight(light);
}

void AnimatedCharacter::ProcessInput()
{
    handler.ProcessInput();
}

void AnimatedCharacter::RenderBuffers()
{
    buffers.Render();
}

void AnimatedCharacter::Render()
{
    ProcessInput();
    UpdateBones();
    RenderBuffers();
}

AnimatedCharacter::AnimatedCharacter(AnimatedCharacterData &data, AnimatedCharacterBuffers &buffers,
                                     AnimatedCharacterInputHandler &handler) : data(data), buffers(buffers),
                                                                               handler(handler){}
