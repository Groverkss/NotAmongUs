#include "HUD.h"

HUD::HUD(Player *player, GLFWwindow *window) {
    this->player = player;
    this->window = window;
}

void HUD::draw(std::string hudText,
               int scale,
               GLfloat width,
               GLfloat height,
               int horizontal,
               int vertical) {
    /* Initialize glText */
    gltInit();

    std::string score = std::to_string(player->score);
    std::string tasks = std::to_string(player->task1 + player->task2);
    std::string time = std::to_string(player->time);
    std::string light = player->light ? "On" : "Off";

    if (hudText.empty()) {
        hudText = "Health " + score + "\n" +
            "Tasks Completed / Tasks Available: " + tasks + " / 2\n" +
            "Light: " + light + "\n" +
            "Time: " + time + "\n";
    }

    /* Creating text */
    GLTtext *text = gltCreateText();
    gltSetText(text, hudText.data());

    /* Begin text drawing (this for instance calls glUseProgram) */
    gltBeginDraw();

    /* Draw any amount of text between begin and end */
    gltColor(1.0f, 1.0f, 1.0f, 1.0f);
    gltDrawText2DAligned(text, height, width, scale, horizontal, vertical);

    /* Finish drawing text */
    gltEndDraw();

    /* Deleting text */
    gltDeleteText(text);

    /* Destroy glText */
    gltTerminate();
}

void HUD::endgame() {
    int viewportWidth, viewportHeight;
    glfwGetFramebufferSize(window, &viewportWidth, &viewportHeight);

    if (player->state == 1) {
        draw("You Lost!",
             5,
             (GLfloat) viewportWidth / 2,
             (GLfloat) viewportHeight / 2,
             GLT_CENTER,
             GLT_CENTER);
    } else {
        draw("You Won!",
             5,
             (GLfloat) viewportWidth / 2,
             (GLfloat) viewportHeight / 2,
             GLT_CENTER,
             GLT_CENTER);
    }
}