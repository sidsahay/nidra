#include "animatedcharacterinputhandler.hpp"

AnimatedCharacterInputHandler::AnimatedCharacterInputHandler(GLFWwindow *window, AnimatedCharacterData &character)
        : window(window), character(character) {}

void AnimatedCharacterInputHandler::ProcessInput() {
    if (!isBouncing) {
        if (glfwGetKey(window, kickKey) == GLFW_PRESS) {
            character.nextClip = 1;
            isBouncing = true;
            debounceTimer = 20;
        } else if (glfwGetKey(window, punchKey) == GLFW_PRESS) {
            character.nextClip = 2;
            isBouncing = true;
            debounceTimer = 20;
        }
    } else {
        character.nextClip = 0;
    }

    if (debounceTimer == 0) {
        isBouncing = false;
    } else {
        debounceTimer--;
    }

}

