#include "GeometryPrimitives.h"

#include <glm/glm.hpp>

namespace GeometryPrimitives {

std::vector<Vertex> createCubeVertices()
{
    std::vector<Vertex> result;
    result.reserve(36);

    glm::vec3 positions[] = {
        {-0.5f, -0.5f, -0.5f}, // 0
        { 0.5f, -0.5f, -0.5f}, // 1
        { 0.5f,  0.5f, -0.5f}, // 2
        {-0.5f,  0.5f, -0.5f}, // 3
        {-0.5f, -0.5f,  0.5f}, // 4
        { 0.5f, -0.5f,  0.5f}, // 5
        { 0.5f,  0.5f,  0.5f}, // 6
        {-0.5f,  0.5f,  0.5f}  // 7
    };

    glm::vec3 normals[] = {
        { 0.0f,  0.0f, -1.0f}, // front
        { 0.0f,  0.0f,  1.0f}, // back
        {-1.0f,  0.0f,  0.0f}, // left
        { 1.0f,  0.0f,  0.0f}, // right
        { 0.0f, -1.0f,  0.0f}, // bottom
        { 0.0f,  1.0f,  0.0f}  // top
    };

    // всі грані червоні (як в методичці)
    glm::vec3 colors[] = {
        {1.0f, 0.0f, 0.0f},
        {1.0f, 0.0f, 0.0f},
        {1.0f, 0.0f, 0.0f},
        {1.0f, 0.0f, 0.0f},
        {1.0f, 0.0f, 0.0f},
        {1.0f, 0.0f, 0.0f}
    };

    unsigned int indices[] = {
        0,1,2, 2,3,0, // front
        4,5,6, 6,7,4, // back
        0,3,7, 7,4,0, // left
        1,5,6, 6,2,1, // right
        0,1,5, 5,4,0, // bottom
        3,2,6, 6,7,3  // top
    };

    for (int i = 0; i < 36; ++i) {
        unsigned int idx  = indices[i];
        int          face = i / 6;

        Vertex v;
        v.position = positions[idx];
        v.normal   = normals[face];
        v.color    = colors[face];
        result.push_back(v);
    }

    return result;
}

// 4-х стороння піраміда (як доп.примітив)
std::vector<Vertex> createPyramidVertices()
{
    std::vector<Vertex> result;

    glm::vec3 base0(-0.5f, 0.0f, -0.5f);
    glm::vec3 base1( 0.5f, 0.0f, -0.5f);
    glm::vec3 base2( 0.5f, 0.0f,  0.5f);
    glm::vec3 base3(-0.5f, 0.0f,  0.5f);
    glm::vec3 top  ( 0.0f, 0.8f,  0.0f);

    glm::vec3 baseNormal(0.0f, -1.0f, 0.0f);
    glm::vec3 baseColor (0.7f, 0.7f, 0.7f);

    // основа
    result.push_back(Vertex{base0, baseNormal, baseColor});
    result.push_back(Vertex{base1, baseNormal, baseColor});
    result.push_back(Vertex{base2, baseNormal, baseColor});

    result.push_back(Vertex{base2, baseNormal, baseColor});
    result.push_back(Vertex{base3, baseNormal, baseColor});
    result.push_back(Vertex{base0, baseNormal, baseColor});

    auto makeSide = [&result](const glm::vec3& a,
                              const glm::vec3& b,
                              const glm::vec3& top,
                              const glm::vec3& color)
    {
        glm::vec3 edge1  = b   - a;
        glm::vec3 edge2  = top - a;
        glm::vec3 normal = glm::normalize(glm::cross(edge1, edge2));

        result.push_back(Vertex{a,   normal, color});
        result.push_back(Vertex{b,   normal, color});
        result.push_back(Vertex{top, normal, color});
    };

    makeSide(base0, base1, top, {1.0f, 0.0f, 0.0f});
    makeSide(base1, base2, top, {0.0f, 1.0f, 0.0f});
    makeSide(base2, base3, top, {0.0f, 0.0f, 1.0f});
    makeSide(base3, base0, top, {1.0f, 1.0f, 0.0f});

    return result;
}

} // namespace GeometryPrimitives
