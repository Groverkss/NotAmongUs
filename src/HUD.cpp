#include "HUD.h"

HUD::HUD(Player *player, GLFWwindow *window) {
    this->player = player;
    this->window = window;
}

void HUD::draw(std::string hudText, int scale, GLfloat width, GLfloat height) {
    /* Initialize glText */
    gltInit();

    std::string score = std::to_string(player->score);
    std::string tasks = std::to_string(player->task1 + player->task2);

    if (hudText.empty()) {
        hudText = "Health " + score + "\n" +
            "Tasks Completed / Tasks Available: " + tasks + " / 2\n" +
            "Light: On\n" +
            "Time: 0";
    }

    /* Creating text */
    GLTtext *text = gltCreateText();
    gltSetText(text, hudText.data());

    /* Begin text drawing (this for instance calls glUseProgram) */
    gltBeginDraw();

    /* Draw any amount of text between begin and end */
    gltColor(1.0f, 1.0f, 1.0f, 1.0f);
    gltDrawText2DAligned(text, height, width, scale, GLT_CENTER, GLT_CENTER);

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
             (GLfloat) viewportHeight / 2);
    } else {
        draw("You Won!",
             5,
             (GLfloat) viewportWidth / 2,
             (GLfloat) viewportHeight / 2);
    }
}