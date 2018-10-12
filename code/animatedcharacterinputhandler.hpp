#ifndef NIDRA_CODE_ANIMATEDCHARACTERINPUTHANDLER_HPP
#define NIDRA_CODE_ANIMATEDCHARACTERINPUTHANDLER_HPP

#include "animatedcharacterdata.hpp"

#include <GLFW/glfw3.h>

struct AnimatedCharacterInputHandler
{
    static const int debounceTimerStartingValue = 10;

    explicit AnimatedCharacterInputHandler(GLFWwindow* window, AnimatedCharacterData& character);

    bool isBouncing = false;
    int debounceTimer = debounceTimerStartingValue;

    int kickKey = 0;
    int punchKey = 0;

    AnimatedCharacterData& character;

    GLFWwindow* window;

    void ProcessInput();
};

#endif //NIDRA_CODE_ANIMATEDCHARACTERINPUTHANDLER_HPP
