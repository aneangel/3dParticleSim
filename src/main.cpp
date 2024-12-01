#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

#include "Kinematics.h"
#include "Particles.h"

float rotationX = 0.0f;
float rotationY = 0.0f;
double lastMouseX, lastMouseY;
bool isDragging = false;

float lastFrameTime = 0.0f;

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT) {
        if (action == GLFW_PRESS) {
            isDragging = true;
            glfwGetCursorPos(window, &lastMouseX, &lastMouseY);
        } else if (action == GLFW_RELEASE) {
            isDragging = false;
        }
    }
}

void cursor_position_callback(GLFWwindow* window, double xpos, double ypos) {
    if (isDragging) {
        float dx = static_cast<float>(xpos - lastMouseX);
        float dy = static_cast<float>(ypos - lastMouseY);

        rotationX += dx * 0.5f;
        rotationY += dy * 0.5f;

        lastMouseX = xpos;
        lastMouseY = ypos;
    }
}

glm::mat4 getCubeRotationMatrix(float rotationX, float rotationY) {
    glm::mat4 rotation = glm::mat4(1.0f);
    rotation = glm::rotate(rotation, glm::radians(rotationX), glm::vec3(1.0f, 0.0f, 0.0f));
    rotation = glm::rotate(rotation, glm::radians(rotationY), glm::vec3(0.0f, 1.0f, 0.0f));
    return rotation;
}

void drawWireframeCube() {
    glColor3f(1.0f, 1.0f, 1.0f);
    glBegin(GL_LINES);

    // Define cube vertices
    GLfloat vertices[8][3] = {
        {-0.5f, -0.5f,  0.5f},  // Front-bottom-left
        { 0.5f, -0.5f,  0.5f},  // Front-bottom-right
        { 0.5f,  0.5f,  0.5f},  // Front-top-right
        {-0.5f,  0.5f,  0.5f},  // Front-top-left
        {-0.5f, -0.5f, -0.5f},  // Back-bottom-left
        { 0.5f, -0.5f, -0.5f},  // Back-bottom-right
        { 0.5f,  0.5f, -0.5f},  // Back-top-right
        {-0.5f,  0.5f, -0.5f}   // Back-top-left
    };

    // Define edges (pairs of indices into the vertex array)
    GLuint edges[12][2] = {
        {0, 1}, {1, 2}, {2, 3}, {3, 0},  // Front face
        {4, 5}, {5, 6}, {6, 7}, {7, 4},  // Back face
        {0, 4}, {1, 5}, {2, 6}, {3, 7}   // Connecting edges
    };

    // Draw edges
    for (int i = 0; i < 12; ++i) {
        glVertex3fv(vertices[edges[i][0]]);
        glVertex3fv(vertices[edges[i][1]]);
    }

    glEnd();
}

Particle particle(0.0f, 0.0f, 0.0f, 0.05f);

int main() {
    if (!glfwInit()) {
        std::cerr << "Failed to initalize GLFW" << std::endl;
        return -1;
    }

    GLFWwindow* window = glfwCreateWindow(800, 600, "3D Particle Sim", nullptr, nullptr);
    if (!window) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetCursorPosCallback(window, cursor_position_callback);

    if (glewInit() != GLEW_OK) {
        std::cerr << "Failed to initalize GLEW" << std::endl;
        glfwDestroyWindow(window);
        glfwTerminate();
        return -1;
    }

    glEnable(GL_DEPTH_TEST);

    while (!glfwWindowShouldClose(window)) {
        float currentFrameTime = static_cast<float>(glfwGetTime());
        float deltaTime = currentFrameTime - lastFrameTime;
        lastFrameTime = currentFrameTime;

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluPerspective(45.0, 800.0 / 600.0, 0.1, 100.0);
        
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        gluLookAt(0.0, 0.0, 3.0,
                    0.0, 0.0, 0.0,
                    0.0, 1.0, 0.0);

        glRotatef(rotationX, 1.0f, 0.0f, 0.0f);
        glRotatef(rotationY, 0.0f, 1.0f, 0.0f);

        glm::mat4 rotationMatrix = getCubeRotationMatrix(rotationX, rotationY);
        drawWireframeCube();

        particle.update(deltaTime, rotationMatrix);
        particle.render();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;


}