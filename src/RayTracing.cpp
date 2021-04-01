#include "RayTracing.h"

RayTracing::RayTracing(Maze *maze) {
    this->maze = maze;
    shaders = createShaders();
}

glm::vec3 RayTracing::getIntersection(glm::vec2 rayA,
                                      glm::vec2 rayB,
                                      glm::vec2 segA,
                                      glm::vec2 segB) {
    /* RAY in paramteric: Point + Delta*T1 */
    float r_px = rayA.x;
    float r_py = rayA.y;
    float r_dx = rayB.x - rayA.x;
    float r_dy = rayB.y - rayA.y;

    /* SEGMENT in paramteric: Point + Delta*T2 */
    float s_px = segA.x;
    float s_py = segA.y;
    float s_dx = segB.x;
    float s_dy = segB.y;

    /* Check if paralllel */
    float r_mag = sqrt(r_dx * r_dx + r_dy * r_dy);
    float s_mag = sqrt(s_dx * s_dx + s_dy * s_dy);
    if (r_dx / r_mag == s_dx / s_mag and r_dy / r_mag == s_dy / s_mag) {
        /* Unit Vectors are same */
        return glm::vec3(0.0f, 0.0f, -1.0f);
    }

    /* SOLVE FOR T1 & T2 */
    /* r_px+r_dx*T1 = s_px+s_dx*T2 && r_py+r_dy*T1 = s_py+s_dy*T2 */
    /* ==> T1 = (s_px+s_dx*T2-r_px)/r_dx = (s_py+s_dy*T2-r_py)/r_dy */
    /* ==> s_px*r_dy + s_dx*T2*r_dy - r_px*r_dy = s_py*r_dx + s_dy*T2*r_dx - r_py*r_dx */
    /* ==> T2 = (r_dx*(s_py-r_py) + r_dy*(r_px-s_px))/(s_dx*r_dy - s_dy*r_dx) */

    float T2 = (r_dx * (s_py - r_py) + r_dy * (r_px - s_px))
        / (s_dx * r_dy - s_dy * r_dx);
    float T1 = (s_px + s_dx * T2 - r_px) / r_dx;

    // Must be within parametic whatevers for RAY/SEGMENT
    if (T1 < 0) {
        return glm::vec3(0.0f, 0.0f, -1.0f);
    }
    if (T2 < 0 || T2 > 1) {
        return glm::vec3(0.0f, 0.0f, -1.0f);
    }

    return glm::vec3(r_px + r_dx * T1, r_py + r_dy * T1, T1);
}

std::vector<glm::vec3> RayTracing::buildSightPolygon(float sightY,
                                                     float sightX) {
    /* Get all angles */
    std::vector<float> uniqueAngles;
    std::vector<float> vertexAngle;
    for (auto point: maze->invHashVertex) {
        float x = point.second.second;
        float y = point.second.first;

        auto angle = glm::atan(y - sightY, x - sightX);
        vertexAngle.push_back(angle);

//        uniqueAngles.push_back(angle - 0.00001f);
        uniqueAngles.push_back(angle);
//        uniqueAngles.push_back(angle + 0.00001f);
    }

    /* Fire rays in all directions */
    std::vector<glm::vec3> intersects;
    for (auto angle: uniqueAngles) {
        /* Calculate dx & dy from angle */
        float dx = cosf(angle);
        float dy = sinf(angle);

        /* Start ray */
        glm::vec2 rayA = glm::vec2(sightX, sightY);
        glm::vec2 rayB = glm::vec2(sightX + dx, sightY + dy);

        /* Get closest intersection */
        glm::vec3 closestIntersect = glm::vec3(0.0f, 0.0f, -1.0f);
        for (int i = 0; i < maze->indices.size(); i += 2) {
            auto vertex1 = maze->invHashVertex[maze->indices[(i << 1)]];
            auto vertex2 = maze->invHashVertex[maze->indices[(i << 1) | 1]];

            glm::vec2 segA = glm::vec2(vertex1.second, vertex1.first);
            glm::vec2 segB = glm::vec2(vertex2.second, vertex2.first);

            auto intersect = getIntersection(rayA, rayB, segA, segB);
            if (intersect.z == -1.0f) {
                continue;
            }

            if (closestIntersect.z == -1.0f
                or intersect.z < closestIntersect.z) {
                closestIntersect = intersect;
            }
        }

        /* Intersection angle */
        if (closestIntersect.z == -1.0f) {
            continue;
        }
        closestIntersect.z = angle;

        /* Add to list of intersects */
        intersects.push_back(closestIntersect);
    }

    std::sort(intersects.begin(),
              intersects.end(),
              [](const glm::vec3 &a, const glm::vec3 &b) -> bool {
                  return a.z < b.z;
              });
    return intersects;
}

void RayTracing::drawStencil(float sightY, float sightX) {
    shaders->use();
    shaders->setMat4("view", viewTransform);
    shaders->setMat4("projection", projectionTransform);

    /* Enable stencil writing */
    glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
    glStencilFunc(GL_ALWAYS, 0, 1);
    glStencilOp(GL_KEEP, GL_KEEP, GL_INVERT);
    glStencilMask(1);

    /* Build sight polygon */
    auto intersections = buildSightPolygon(sightY, sightX);

    std::vector<float> vertices;
    vertices.push_back(sightX);
    vertices.push_back(sightY);

    for (auto it: intersections) {
        vertices.push_back(it.x);
        vertices.push_back(it.y);
    }

    /* Create triangle fan from sight polygon */
    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER,
                 vertices.size() * sizeof(float),
                 vertices.data(),
                 GL_DYNAMIC_DRAW);

    glVertexAttribPointer(0,
                          2,
                          GL_FLOAT,
                          GL_FALSE,
                          2 * sizeof(float),
                          nullptr);
    glEnableVertexAttribArray(0);

    glDrawArrays(GL_TRIANGLE_FAN, 0, vertices.size());

    glBindVertexArray(0);

    /* Disable stencil */
    glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
    glStencilFunc(GL_EQUAL, 1, 1);
    glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
}

Shader *RayTracing::createShaders() {
    auto newShader = new Shader(vertexShader, fragmentShader);
    return newShader;
}

void RayTracing::setCameraAndProjection(glm::mat4 camera,
                                        glm::mat4 projection) {
    viewTransform = camera;
    projectionTransform = projection;
}