#include "Model.h"

Model::Model(const std::pair<float, float> &startPoint,
             const std::vector<float> &color,
             Maze *mazeIn,
             float objectWidth) {
    this->currPoint = startPoint;
    this->color = color;
    this->maze = mazeIn;
    this->objectWidth = objectWidth;

    auto margin = (1.0f - objectWidth) / 2;

    this->currPoint.first += margin;
    this->currPoint.second += margin;

    points = {
        {currPoint.first, currPoint.second},
        {currPoint.first, currPoint.second + objectWidth},
        {currPoint.first + objectWidth, currPoint.second + objectWidth},
        {currPoint.first + objectWidth, currPoint.second},
    };

    createIndices();
    VAO = createVAO();
    shaders = createShaders();

    modelTransform = glm::mat4(1.0f);
    viewTransform = glm::mat4(1.0f);
    projectionTransform = glm::mat4(1.0f);

    show = true;

    /* Init RNG */
    std::random_device dev;
    std::mt19937 rngInit(dev());
    rng = rngInit;
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
                          nullptr);
    glEnableVertexAttribArray(0);

    return newVAO;
}

Shader *Model::createShaders() {
    auto newShader = new Shader(vertexShader, fragmentShader);
    return newShader;
}

void Model::draw() {
    if (!show) {
        return;
    }

    shaders->use();
    shaders->setMat4("model", modelTransform);
    shaders->setMat4("view", viewTransform);
    shaders->setMat4("projection", projectionTransform);

    shaders->setVec3("color", glm::make_vec3(color.data()));
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, nullptr);
    glBindVertexArray(0);
}

void Model::setCameraAndProjection(glm::mat4 camera, glm::mat4 projection) {
    viewTransform = camera;
    projectionTransform = projection;
}

bool Model::checkCollisionWithMaze() {
    if (!show) {
        return false;
    }

    auto x = currPoint.first;
    auto y = currPoint.second;
    auto w = objectWidth;

    for (int i = 0; i < maze->indices.size(); i += 2) {
        auto index1 = maze->indices[i];
        auto index2 = maze->indices[i + 1];

        std::pair<float, float>
            vertex1 = {maze->vertices[index1 << 1],
                       maze->vertices[(index1 << 1) | 1]};
        std::pair<float, float>
            vertex2 = {maze->vertices[index2 << 1],
                       maze->vertices[(index2 << 1) | 1]};

        if (vertex1.first == vertex2.first) {
            if (x + w >= vertex1.first
                and vertex1.first >= x
                and std::max(vertex1.second, vertex2.second) >= y
                and std::min(vertex1.second, vertex2.second) <= y + w) {
                return true;
            }
        } else {
            if (y + w >= vertex1.second
                and vertex1.second >= y
                and std::max(vertex1.first, vertex2.first) >= x
                and std::min(vertex1.first, vertex2.first) <= x + w) {
                return true;
            }
        }
    }

    return false;
}

int Model::getRandom(int low, int high) {
    std::uniform_int_distribution<std::mt19937::result_type> randGen(low, high);
    return randGen(rng);
}

bool Model::checkCollisionWithModel(Model *other) {
    if (!show or !other->show) {
        return false;
    }

    auto x = currPoint.first;
    auto y = currPoint.second;
    auto w = objectWidth;

    for (int i = 0; i < other->points.size(); i++) {
        auto vertex1 = other->points[i];
        auto vertex2 = other->points[(i + 1) % (other->points.size())];

        if (vertex1.first == vertex2.first) {
            if (x + w >= vertex1.first
                and vertex1.first >= x
                and std::max(vertex1.second, vertex2.second) >= y
                and std::min(vertex1.second, vertex2.second) <= y + w) {
                return true;
            }
        } else {
            if (y + w >= vertex1.second
                and vertex1.second >= y
                and std::max(vertex1.first, vertex2.first) >= x
                and std::min(vertex1.first, vertex2.first) <= x + w) {
                return true;
            }
        }
    }

    return false;
}