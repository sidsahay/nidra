// Include standard headers
#include <vector>
#include <map>
#include <iostream>

#define USE_ASSIMP

// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <GLFW/glfw3.h>

GLFWwindow *window;

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>

using namespace glm;

#include "../code/utilities/controls.hpp"
#include "../code/scene/animatedcharacter.hpp"

#include <assimp/Importer.hpp>      // C++ importer interface


int main() {
    if (!glfwInit()) {
        fprintf(stderr, "Failed to initialize GLFW\n");
        getchar();
        return -1;
    }

    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(1024, 768, "Tatti", NULL, NULL);
    if (window == NULL) {
        fprintf(stderr,
                "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n");
        getchar();
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // Initialize GLEW
    glewExperimental = static_cast<GLboolean>(true); // Needed for core profile
    if (glewInit() != GLEW_OK) {
        fprintf(stderr, "Failed to initialize GLEW\n");
        getchar();
        glfwTerminate();
        return -1;
    }

    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_FALSE);

    // Hide the mouse and enable unlimited mouvement
    //glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    glfwPollEvents();
    glfwSetCursorPos(window, 1024 / 2, 768 / 2);

    glClearColor(0.1f, 0.1f, 0.1f, 0.0f);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glEnable(GL_CULL_FACE);

    GLuint vertexArrayID;
    glGenVertexArrays(1, &vertexArrayID);
    glBindVertexArray(vertexArrayID);

    Assimp::Importer importer;

    AnimatedCharacterData characterData(importer.ReadFile("../models/combat.dae", 0), "../code/realtime/shaders/vshader.glsl",
                                        "../code/realtime/shaders/fshader.glsl", "../models/tex.bmp");
    if (!characterData.scene) {
        fprintf(stderr, importer.GetErrorString());
        getchar();
        return false;
    }

    using clip = std::pair<unsigned int, unsigned int>;

    characterData.animationClips.emplace_back(9, 69);
    characterData.animationClips.emplace_back(79, 119);
    characterData.animationClips.emplace_back(129, 159);

    //Stuff the data into the GPU
    AnimatedCharacterBuffers characterBuffers(characterData);
    characterBuffers.Build();

    //Attach the input handler and set keybindings
    AnimatedCharacterInputHandler characterInputHandler(window, characterData);
    characterInputHandler.kickKey = GLFW_KEY_J;
    characterInputHandler.punchKey = GLFW_KEY_K;

    AnimatedCharacter character(characterData, characterBuffers, characterInputHandler);
    glPointSize(5.0f);

    /*GLuint boneProgramID = LoadShaders("../code/shaders/vshader_bone.glsl", "../code/shaders/fshader_bone.glsl");

    glUseProgram(boneProgramID);
    GLint boneMVPID = glGetUniformLocation(boneProgramID, "MVP");

    //send bone position data
    GLuint bonePositionBuffer;
    glGenBuffers(1, &bonePositionBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, bonePositionBuffer);
    glBufferData(GL_ARRAY_BUFFER, characterData.bonePositions.size() * sizeof(glm::vec3),
                 &characterData.bonePositions[0], GL_DYNAMIC_DRAW);
*/
    do {

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Compute the MVP matrix from keyboard and mouse input
        computeMatricesFromInputs();
        glm::mat4 ProjectionMatrix = getProjectionMatrix();
        glm::mat4 ViewMatrix = getViewMatrix();
        glm::mat4 ModelMatrix = glm::mat4(1.0);
        glm::mat4 MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;

        glm::vec3 light = glm::vec3(4, 4, 4);

        character.UpdateMatrices(ProjectionMatrix, ViewMatrix, ModelMatrix, MVP);
        character.UpdateLight(light);

        character.Render();

//        Draw bones
//        glUseProgram(boneProgramID);
//        glUniformMatrix4fv(boneMVPID, 1, GL_FALSE, &MVP[0][0]);
//        glNamedBufferSubData(bonePositionBuffer, 0, character.bonePositions.size() * sizeof(glm::vec3), &character.bonePositions[0]);
//
//        glEnableVertexAttribArray(0);
//        glBindBuffer(GL_ARRAY_BUFFER, bonePositionBuffer);
//        glVertexAttribPointer(
//                0,                  // attribute
//                3,                  // size
//                GL_FLOAT,           // type
//                GL_FALSE,           // normalized?
//                0,                  // stride
//                (void *) 0            // array buffer offset
//        );
//
//        glDrawArrays(GL_POINTS, 0, character.bonePositions.size());
//        glDisableVertexAttribArray(0);


        glfwSwapBuffers(window);
        glfwPollEvents();

    } while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
             glfwWindowShouldClose(window) == 0);

    glDeleteVertexArrays(1, &vertexArrayID);

    glfwTerminate();

    return 0;
}