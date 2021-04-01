#ifndef NOTAMONGUS_SRC_RAYTRACING_H_
#define NOTAMONGUS_SRC_RAYTRACING_H_

#include "Shaders.h"
#include "Maze.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <cmath>
#include <algorithm>

class RayTracing {
private:
    glm::vec3 getIntersection(glm::vec2 rayA,
                              glm::vec2 rayB,
                              glm::vec2 segA,
                              glm::vec2 segB);

    char *vertexShader = "resources/shaders/stencilVertex.glsl";
    char *fragmentShader = "resources/shaders/stencilFragment.glsl";

    /**
     * Returns vector intersect x point, intersect y point,
     * intersect angle (sorted on angle)
     * */
    std::vector<glm::vec3> buildSightPolygon(float sightX, float sightY);
    Maze *maze;
    Shader *createShaders();
    Shader *shaders;
    glm::mat4 viewTransform, projectionTransform;
public:
    RayTracing(Maze *maze);
    void drawStencil(float sightY, float sightX);
    void setCameraAndProjection(glm::mat4 camera, glm::mat4 projection);
};

#endif //NOTAMONGUS_SRC_RAYTRACING_H_
