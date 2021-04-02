#include "RayTracing.h"

RayTracing::RayTracing(Maze *maze) {
    this->maze = maze;
    shaders = createShaders();
}

glm::dvec3 RayTracing::getIntersection(glm::dvec2 rayA,
                                       glm::dvec2 rayB,
                                       glm::dvec2 segA,
                                       glm::dvec2 segB) {
    /* RAY in paramteric: Point + Delta*T1 */
    double r_px = rayA.x;
    double r_py = rayA.y;
    double r_dx = rayB.x - rayA.x;
    double r_dy = rayB.y - rayA.y;

    /* SEGMENT in paramteric: Point + Delta*T2 */
    double s_px = segA.x;
    double s_py = segA.y;
    double s_dx = segB.x - segA.x;
    double s_dy = segB.y - segA.y;

    /* Check if paralllel */
    double r_mag = sqrt(r_dx * r_dx + r_dy * r_dy);
    double s_mag = sqrt(s_dx * s_dx + s_dy * s_dy);
    if (r_dx / r_mag == s_dx / s_mag and r_dy / r_mag == s_dy / s_mag) {
        /* Unit Vectors are same */
        return glm::dvec3(0.0f, 0.0f, -1.0f);
    }

    /* SOLVE FOR T1 & T2 */
    /* r_px+r_dx*T1 = s_px+s_dx*T2 && r_py+r_dy*T1 = s_py+s_dy*T2 */
    /* ==> T1 = (s_px+s_dx*T2-r_px)/r_dx = (s_py+s_dy*T2-r_py)/r_dy */
    /* ==> s_px*r_dy + s_dx*T2*r_dy - r_px*r_dy = s_py*r_dx + s_dy*T2*r_dx - r_py*r_dx */
    /* ==> T2 = (r_dx*(s_py-r_py) + r_dy*(r_px-s_px))/(s_dx*r_dy - s_dy*r_dx) */

    double T2 = (r_dx * (s_py - r_py) + r_dy * (r_px - s_px))
        / (s_dx * r_dy - s_dy * r_dx);
    double T1 = (s_px + s_dx * T2 - r_px) / r_dx;

    // Must be within parametic whatevers for RAY/SEGMENT
    if (T1 < 0) {
        return glm::dvec3(0.0f, 0.0f, -1.0f);
    }
    if (T2 < 0.0 || T2 > 1.0f) {
        return glm::dvec3(0.0f, 0.0f, -1.0f);
    }

    return glm::dvec3(r_px + r_dx * (T1),
                      r_py + r_dy * (T1),
                      T1);
}

std::vector<glm::dvec3> RayTracing::buildSightPolygon(double sightY,
                                                      double sightX) {
    /* Get all angles */
    std::vector<double> uniqueAngles;
    for (auto point: maze->invHashVertex) {
        double x = point.second.second;
        double y = point.second.first;

        auto angle = glm::atan(y - sightY, x - sightX);

        uniqueAngles.push_back(angle - 0.00001f);
        uniqueAngles.push_back(angle);
        uniqueAngles.push_back(angle + 0.00001f);
    }

    /* Fire rays in all directions */
    std::vector<glm::dvec3> intersects;
    for (auto angle: uniqueAngles) {
        /* Calculate dx & dy from angle */
        double dx = cos(angle);
        double dy = sin(angle);

        /* Start ray */
        auto rayA = glm::dvec2(sightX, sightY);
        auto rayB = glm::dvec2(sightX + dx, sightY + dy);

        /* Get closest intersection */
        auto closestIntersect = glm::dvec3(0.0f, 0.0f, -1.0f);
        for (int i = 0; i < maze->indices.size(); i += 2) {
            auto vertex1 = maze->invHashVertex[maze->indices[i]];
            auto vertex2 = maze->invHashVertex[maze->indices[i | 1]];

            auto segA = glm::dvec2(vertex1.second, vertex1.first);
            auto segB = glm::dvec2(vertex2.second, vertex2.first);

            auto intersect = getIntersection(rayA, rayB, segA, segB);
            if (intersect.z < 0.0f) {
                continue;
            }

            if (closestIntersect.z < 0.0f or intersect.z < closestIntersect.z) {
                closestIntersect = intersect;
            }
        }

        /* Intersection angle */
        if (closestIntersect.z < 0.0f) {
            continue;
        }
        closestIntersect.z = angle;

        /* Add to list of intersects */
        intersects.push_back(closestIntersect);
    }

    std::sort(intersects.begin(),
              intersects.end(),
              [](const glm::dvec3 &a, const glm::dvec3 &b) -> bool {
                  return a.z < b.z;
              });
    return intersects;
}

void RayTracing::drawStencil(float sightY, float sightX) {
    shaders->use();
    shaders->setMat4("view", viewTransform);
    shaders->setMat4("projection", projectionTransform);

    /* Enable stencil writing */
//    glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
//    glStencilFunc(GL_ALWAYS, 0, 1);
//    glStencilOp(GL_KEEP, GL_KEEP, GL_INVERT);
//    glStencilMask(1);

    /* Build sight polygon */
    auto intersections = buildSightPolygon(sightY, sightX);

    std::vector<float> vertices;

    for (int i = 0; i < intersections.size(); i++) {
        auto curr1 = intersections[(i + 1) % (intersections.size())];
        auto curr2 = intersections[i % intersections.size()];
        vertices.push_back(sightX);
        vertices.push_back(sightY);
        vertices.push_back(curr1.x);
        vertices.push_back(curr1.y);
        vertices.push_back(curr2.x);
        vertices.push_back(curr2.y);
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

    glDrawArrays(GL_TRIANGLES, 0, vertices.size());

    glBindVertexArray(0);

    /* Disable stencil */
//    glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
//    glStencilFunc(GL_EQUAL, 1, 1);
//    glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
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