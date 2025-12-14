#pragma once

#include <vector>
#include <glm/glm.hpp>

#include "Vertex.h"

class OpenglRenderSystem
{
public:
    OpenglRenderSystem();

    void init();
    void clearDisplay(float r, float g, float b);
    void setViewport(double x, double y, double width, double height);

    void renderTriangleSoup(const std::vector<Vertex>& vertices);
    void renderLines       (const std::vector<Vertex>& vertices);

    void setupLight(uint32_t index,
                    const glm::vec3& position,
                    const glm::vec3& Ia,
                    const glm::vec3& Id,
                    const glm::vec3& Is);

    void turnLight(uint32_t index, bool enable);

    void setWorldMatrix     (const glm::mat4& matrix) { m_world      = matrix; }
    void setViewMatrix      (const glm::mat4& matrix) { m_view       = matrix; }
    void setProjectionMatrix(const glm::mat4& matrix) { m_projection = matrix; }

    const glm::mat4& getWorldMatrix()      const { return m_world; }
    const glm::mat4& getViewMatrix()       const { return m_view; }
    const glm::mat4& getProjectionMatrix() const { return m_projection; }

private:
    void renderData(const std::vector<Vertex>& vertices, unsigned int mode);

    glm::mat4 m_world{1.0f};
    glm::mat4 m_view{1.0f};
    glm::mat4 m_projection{1.0f};
};
