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
    glm::dvec3 getIntersection(glm::dvec2 rayA,
                               glm::dvec2 rayB,
                               glm::dvec2 segA,
                               glm::dvec2 segB);

    char *vertexShader = "resources/shaders/stencilVertex.glsl";
    char *fragmentShader = "resources/shaders/stencilFragment.glsl";

    /**
     * Returns vector intersect x point, intersect y point,
     * intersect angle (sorted on angle)
     * */
    std::vector<glm::dvec3> buildSightPolygon(double sightX, double sightY);
    Maze *maze;
    Shader *createShaders();
    Shader *shaders;
    glm::mat4 viewTransform, projectionTransform;
    float offset;
public:
    RayTracing(Maze *maze);
    void drawStencil(float sightY, float sightX);
    void setCameraAndProjection(glm::mat4 camera, glm::mat4 projection);
};

#endif //NOTAMONGUS_SRC_RAYTRACING_H_
