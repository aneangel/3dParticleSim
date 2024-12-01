#ifndef PARTICLES_H
#define PARTICLES_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <cmath>

#include "Kinematics.h"

class Particle {
public:
    float x, y, z;
    float radius;
    float velocityX;
    float velocityY;
    float velocityZ;
    float color[3];

    Particle(float startX, float startY, float startZ, float startRadius)
        : x(startX), y(startY), z(startZ), radius(startRadius), velocityY(0.0f) {
            color[0] = 0.0f;
            color[1] = 0.0f;
            color[2] = 1.0f;
        }

void update(float deltaTime, const glm::mat4& rotationMatrix) {
    
    glm::vec3 velocityVec(velocityX, velocityY, velocityZ);
    velocityVec = glm::vec3(rotationMatrix * glm::vec4(velocityVec, 0.0f));

    velocityX = velocityVec.x;
    velocityY = velocityVec.y;
    velocityZ = velocityVec.z;

    // Apply gravity
    Kinematics::applyGravity(velocityY, y, deltaTime, rotationMatrix);

    // Handle collisions with cube boundaries
    Kinematics::handleCollision(x, y, z, velocityX, velocityY, velocityZ, radius);

    float speed = std::sqrt(velocityX * velocityX + velocityY * velocityY + velocityZ * velocityZ);
    color[0] = std::min(1.0f, speed / 10.f);
    color[2] = std::max(0.0f, 1.0f - speed / 10.f);
}

void render() {
    glColor3f(color[0], color[1], color[2]);
    glPushMatrix();
    glTranslatef(x, y, z);
    drawSphere(radius);
    glPopMatrix();
}

public:
    void drawSphere(float r) {
        int slices = 16, stacks = 16;
        for (int i = 0; i <= stacks; i++) {
            float lat0 = M_PI * (-0.5f + (float)(i - 1) / stacks);
            float z0 = r * sin(lat0);
            float zr0 = r * cos(lat0);

            float lat1 = M_PI * (-0.5f + (float)i / stacks);
            float z1 = r * sin(lat1);
            float zr1 = r * cos(lat1);

            glBegin(GL_QUAD_STRIP);
            for (int j = 0; j <= slices; j++) {
                float lng = 2 * M_PI * (float)(j - 1) / slices;
                float x = cos(lng);
                float y = sin(lng);

                glNormal3f(x * zr0, y * zr0, z0);
                glVertex3f(x * zr0, y * zr0, z0);
                glNormal3f(x * zr1, y * zr1, z1);
                glVertex3f(x * zr1, y * zr1, z1);
            }

            glEnd();
        }
    };
};


#endif