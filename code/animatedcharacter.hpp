#ifndef NIDRA_ANIMATEDCHARACTER_HPP
#define NIDRA_ANIMATEDCHARACTER_HPP

#include "animatedcharacterbuffers.hpp"
#include "animatedcharacterinputhandler.hpp"

struct AnimatedCharacter
{
    explicit AnimatedCharacter(AnimatedCharacterData& data, AnimatedCharacterBuffers& buffers, AnimatedCharacterInputHandler& handler);

    AnimatedCharacterData& data;
    AnimatedCharacterBuffers& buffers;
    AnimatedCharacterInputHandler& handler;

    void UpdateMatrices(const glm::mat4& P, const glm::mat4& V, const glm::mat4& M, const glm::mat4& MVP);
    void UpdateBones();
    void UpdateLight(const glm::vec3& light);
    void ProcessInput();
    void RenderBuffers();
    void Render();
};

#endif //NIDRA_CODE_ANIMATEDCHARACTER_HPP
