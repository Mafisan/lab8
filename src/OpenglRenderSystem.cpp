#include "OpenglRenderSystem.h"

#include <stdexcept>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/type_ptr.hpp>

OpenglRenderSystem::OpenglRenderSystem() = default;

void OpenglRenderSystem::init()
{
    // ГОЛОВНЕ: ініціалізувати GLAD після того, як є контекст GLFW
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        throw std::runtime_error("Failed to initialize glad");
    }

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glEnable(GL_NORMALIZE);

    glEnable(GL_LIGHTING);
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
}

void OpenglRenderSystem::clearDisplay(float r, float g, float b)
{
    glClearColor(r, g, b, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void OpenglRenderSystem::setViewport(double x, double y, double width, double height)
{
    glViewport(static_cast<GLint>(x), static_cast<GLint>(y),
               static_cast<GLsizei>(width), static_cast<GLsizei>(height));
}

void OpenglRenderSystem::renderTriangleSoup(const std::vector<Vertex>& vertices)
{
    renderData(vertices, GL_TRIANGLES);
}

void OpenglRenderSystem::renderLines(const std::vector<Vertex>& vertices)
{
    renderData(vertices, GL_LINES);
}

static GLenum GetLightEnum(uint32_t index)
{
    static const GLenum lights[] = {
        GL_LIGHT0, GL_LIGHT1, GL_LIGHT2, GL_LIGHT3,
        GL_LIGHT4, GL_LIGHT5, GL_LIGHT6, GL_LIGHT7
    };
    if (index >= sizeof(lights) / sizeof(lights[0])) {
        return GL_LIGHT0;
    }
    return lights[index];
}

void OpenglRenderSystem::setupLight(uint32_t index,
                                    const glm::vec3& position,
                                    const glm::vec3& Ia,
                                    const glm::vec3& Id,
                                    const glm::vec3& Is)
{
    GLenum light = GetLightEnum(index);

    glm::vec4 pos4    (position, 1.0f);
    glm::vec4 ambient (Ia,       1.0f);
    glm::vec4 diffuse (Id,       1.0f);
    glm::vec4 specular(Is,       1.0f);

    glEnable(GL_LIGHTING);
    glLightfv(light, GL_POSITION, glm::value_ptr(pos4));
    glLightfv(light, GL_AMBIENT,  glm::value_ptr(ambient));
    glLightfv(light, GL_DIFFUSE,  glm::value_ptr(diffuse));
    glLightfv(light, GL_SPECULAR, glm::value_ptr(specular));
}

void OpenglRenderSystem::turnLight(uint32_t index, bool enable)
{
    GLenum light = GetLightEnum(index);
    if (enable)
        glEnable(light);
    else
        glDisable(light);
}

void OpenglRenderSystem::renderData(const std::vector<Vertex>& vertices, unsigned int mode)
{
    // MODELVIEW = View * World
    glMatrixMode(GL_MODELVIEW);
    glm::mat4 viewWorld = m_view * m_world;
    glLoadMatrixf(glm::value_ptr(viewWorld));

    // PROJECTION
    glMatrixMode(GL_PROJECTION);
    glLoadMatrixf(glm::value_ptr(m_projection));

    glBegin(mode);
    for (const auto& v : vertices)
    {
        glNormal3f (v.normal.x,   v.normal.y,   v.normal.z);
        glColor3f  (v.color.r,    v.color.g,    v.color.b);
        glVertex3f (v.position.x, v.position.y, v.position.z);
    }
    glEnd();
}
