#ifndef NOTAMONGUS_SRC_HUD_H_
#define NOTAMONGUS_SRC_HUD_H_

#include "Player.h"
#include "WindowHandler.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define GLT_IMPLEMENTATION
#include "gltext.h"

#include <string>

class HUD {
private:
    Player *player;
    GLFWwindow *window;
public:
    HUD(Player *player, GLFWwindow *window);

    void draw(std::string hudText = "",
              int scale = 1,
              GLfloat width = 0,
              GLfloat height = 0,
              int horizontal = GLT_LEFT,
              int vertical = GLT_TOP);

    void endgame();
};

#endif //NOTAMONGUS_SRC_HUD_H_