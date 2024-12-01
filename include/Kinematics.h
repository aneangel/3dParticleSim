#ifndef KINEMATICS_H
#define KINEMATICS_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <cmath>

class Kinematics {
public:
    static constexpr float gravity = -1.8f;

    static void applyGravity(float& veloticyY, float& posY, float deltaTime, const glm::mat4& rotationMatrix) {
        
        glm::vec3 gravityVec(0.0f, gravity, 0.0f);

        gravityVec = glm::vec3(rotationMatrix * glm::vec4(gravityVec, 0.0f));
        
        veloticyY += gravity * deltaTime;
        posY += veloticyY * deltaTime;
    }

    static void handleCollision(float& x, float& y, float& z,
                            float& velocityX, float& velocityY, float& velocityZ,
                            float radius, float restitution = 0.5f) {
        const float min = -0.5f;  // Cube minimum boundary
        const float max = 0.5f;   // Cube maximum boundary

        // Collision detection with bottom face
         if (y - radius < min) {
            y = min + radius;
            velocityY = -velocityY * restitution;
        }

        // Reset the particle to top face as soon as collision happens with bottom face
        // if (y - radius < min) {
        //     // Reset particle to top face when it hits the bottom face
        //     y = max - radius;  // Reset to just below the top face
        //     x = static_cast<float>(rand()) / RAND_MAX * (1.0f - 2 * radius) - (0.5f - radius);
        //     z = static_cast<float>(rand()) / RAND_MAX * (1.0f - 2 * radius) - (0.5f - radius);
        //     velocityY = 0.0f;  // Initial downward velocity
        //     return;  // Skip further collision logic for this frame
        // }

        // Collision detection with top face
        if (y + radius > max) {
            y = max - radius - 0.001f;  // Push slightly inside the cube
            velocityY = -velocityY * restitution;
        }
    }
};

#endif