#include "Model.h"

Model::Model(std::pair<float, float> startPoint,
             const std::vector<float> &color) {
    this->currPoint = startPoint;
    this->color = color;

    points = {
        {startPoint.first, startPoint.second},
        {startPoint.first, startPoint.second + 1},
        {startPoint.first + 1, startPoint.second + 1},
        {startPoint.first + 1, startPoint.second},
    };

    createIndices();
    VAO = createVAO();
    shaders = createShaders();

    /* TODO: Fix temporary value */
    modelTransform = glm::mat4(1.0f);
    viewTransform = glm::lookAt(
        glm::vec3(15, 15, 1),
        glm::vec3(15, 15, 0),
        glm::vec3(0, 1, 0)
    );

    /* TODO: Remove constants */
    projectionTransform = glm::ortho(-30.0f, 30.0f, -30.0f, 30.0f);
}

void Model::createIndices() {
    for (auto it: points) {
        vertices.push_back(it.first);
        vertices.push_back(it.second);
    }

    indices.insert(indices.end(), {0, 1, 2});
    indices.insert(indices.end(), {2, 3, 0});
}

unsigned int Model::createVAO() {
    /* Create VAO */
    unsigned int newVAO;
    glGenVertexArrays(1, &newVAO);
    glBindVertexArray(newVAO);

    /* Set vertex buffer */
    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER,
                 vertices.size() * sizeof(float),
                 vertices.data(),
                 GL_STATIC_DRAW);

    /* Set element buffer */
    unsigned int EBO;
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                 indices.size() * sizeof(unsigned int),
                 indices.data(),
                 GL_STATIC_DRAW);

    /* Set vertex pointers */
    glVertexAttribPointer(0,
                          2,
                          GL_FLOAT,
                          GL_FALSE,
                          2 * sizeof(float),
                          (void *) 0);
    glEnableVertexAttribArray(0);

    return newVAO;
}

Shader *Model::createShaders() {
    auto newShader = new Shader(vertexShader, fragmentShader);
    return newShader;
}

void Model::debug() {
    std::cout << "MODEL MATRIX\n\n";

    auto model = glm::value_ptr(modelTransform);
    for (int i = 0; i < 16; i++) {
        std::cout << model[i] << "\n";
    }

    std::cout << "VIEW MATRIX\n\n";

    auto view = glm::value_ptr(viewTransform);
    for (int i = 0; i < 16; i++) {
        std::cout << view[i] << "\n";
    }

    std::cout << "PROJECTION MATRIX\n\n";

    auto project = glm::value_ptr(projectionTransform);
    for (int i = 0; i < 16; i++) {
        std::cout << project[i] << "\n";
    }

    std::cout << "VERTICES \n\n";

    for (auto it: vertices) {
        std::cout << it << "\n";
    }
}

void Model::draw() {
    shaders->setMat4("projection", projectionTransform);
    shaders->setMat4("model", modelTransform);
    shaders->setMat4("view", viewTransform);
    shaders->use();
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

bool Model::checkCollision(Model *otherModel) {
    /* TODO: Implement collision */
    return false;
}