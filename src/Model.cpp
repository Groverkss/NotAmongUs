#include "Model.h"

Model::Model(const std::vector<std::pair<float, float>> &points,
             const std::vector<float> &color) {
    this->points = points;
    this->color = color;

    createIndices();
    VAO = createVAO();
    shaders = createShaders();
    initTransforms();
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

void Model::initTransforms() {
    /* TODO: Write better transforms */
    modelTransform = glm::mat4(1.0f);
    modelTransform =
        glm::scale(modelTransform, glm::vec3(0.1f, 0.1f, 1.0f));
    modelTransform =
        glm::translate(modelTransform, glm::vec3(-0.5f, -0.5f, 0.0f));

    viewTransform = glm::mat4(1.0f);
}

void Model::debug() {
    auto model = glm::value_ptr(modelTransform);
    for (int i = 0; i < 16; i++) {
        std::cout << model[i] << "\n";
    }

    std::cout << "\n\n";

    auto view = glm::value_ptr(viewTransform);
    for (int i = 0; i < 16; i++) {
        std::cout << view[i] << "\n";
    }
}

void Model::draw() {
    shaders->setMat4("model", modelTransform);
    shaders->setMat4("view", viewTransform);
    shaders->use();
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
}

bool Model::checkCollision(Model *otherModel) {
    /* TODO: Implement collision */
    return false;
}