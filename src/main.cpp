#include "ViewerWindow.h"
#include "OpenglRenderSystem.h"
#include "GeometryPrimitives.h"

#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <stdexcept>

void renderScene(OpenglRenderSystem& rs)
{
    static const auto cube = GeometryPrimitives::createCubeVertices();
    rs.renderTriangleSoup(cube);
}

void moveCube(OpenglRenderSystem& rs, const glm::vec3& offset)
{
    glm::mat4 view = rs.getViewMatrix();
    view = glm::translate(view, offset);
    rs.setViewMatrix(view);
}

int main()
{
    constexpr int width  = 640;
    constexpr int height = 480;

    try {
        ViewerWindow       window("MeshViewer Lab8", width, height);
        OpenglRenderSystem rs;

        rs.init();

        // світло
        rs.setupLight(
            0,
            glm::vec3(0.0f, 5.0f, 0.0f),
            glm::vec3(0.2f, 0.2f, 0.2f),  // ambient
            glm::vec3(0.8f, 0.8f, 0.8f),  // diffuse
            glm::vec3(1.0f, 1.0f, 1.0f)   // specular
        );
        rs.turnLight(0, true);

        // камера
        glm::vec3 cameraPosition(0.0f, 0.0f, 3.0f);
        glm::vec3 lookAtCenter  (0.0f, 0.0f, 0.0f);
        glm::vec3 cameraUp      (0.0f, 1.0f, 0.0f);

        glm::mat4 view = glm::lookAt(cameraPosition, lookAtCenter, cameraUp);
        glm::mat4 proj = glm::perspective(
            glm::radians(60.0f),
            static_cast<float>(width) / static_cast<float>(height),
            0.1f,
            100.0f
        );

        rs.setViewMatrix(view);
        rs.setProjectionMatrix(proj);

        // Повертаємо куб, щоб виглядав обʼємно
        glm::mat4 world =
            glm::rotate(glm::mat4(1.0f),
                        glm::radians(35.0f),
                        glm::vec3(1.0f, 1.0f, 0.0f));
        rs.setWorldMatrix(world);

        // обробка клавіш
        window.setKeyCallback([&rs, &window](KeyCode key, Action action, Modifier)
        {
            if (action != Action::Press)
                return;

            switch (key) {
            case KeyCode::Up:
                moveCube(rs, glm::vec3(0.f,  0.1f, 0.f)); break;
            case KeyCode::Down:
                moveCube(rs, glm::vec3(0.f, -0.1f, 0.f)); break;
            case KeyCode::Left:
                moveCube(rs, glm::vec3(-0.1f, 0.f, 0.f)); break;
            case KeyCode::Right:
                moveCube(rs, glm::vec3( 0.1f, 0.f, 0.f)); break;
            case KeyCode::Escape:
                glfwSetWindowShouldClose(window.getGLFWHandle(), GLFW_TRUE);
                break;
            default:
                break;
            }
        });

        // головний цикл
        while (!window.shouldClose())
        {
            rs.setViewport(
                0, 0,
                static_cast<double>(window.getWidth()),
                static_cast<double>(window.getHeight())
            );

            rs.clearDisplay(0.5f, 0.5f, 0.5f);
            renderScene(rs);

            window.swapBuffers();
            window.pollEvents();
        }
    }
    catch (const std::exception& ex) {
        std::cerr << "Помилка: " << ex.what() << std::endl;
        return -1;
    }

    return 0;
}
