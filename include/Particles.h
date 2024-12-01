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

static bool checkCollision(const Particle& p1, const Particle& p2) {
    float dx = p1.x - p2.x;
    float dy = p1.y - p2.y;
    float dz = p1.z - p2.z;

    float distanceSquared = dx * dx + dy * dy + dz * dz;
    float radiusSum = p1.radius + p2.radius;

    return distanceSquared < (radiusSum * radiusSum);
}

static void resolveCollision(Particle& p1, Particle& p2) {
    float dx = p1.x - p2.x;
    float dy = p1.y - p2.y;
    float dz = p1.z - p2.z;

    float distance = std::sqrt(dx * dx + dy * dy + dz * dz);
    if (distance == 0.0f) return;

    float nx = dx / distance;
    float ny = dy / distance;
    float nz = dz / distance;

    float vx = p1.velocityX - p2.velocityX;
    float vy = p1.velocityY - p2.velocityY;
    float vz = p1.velocityZ - p2.velocityZ;

    float dot = vx * nx + vy * ny + vz * nz;

    if (dot > 0.0f) return;

    float restitution = 0.0f;
    float impulse = -(1.0f + restitution) * dot / 2.0f;

    p1.velocityX += impulse * nx;
    p1.velocityY += impulse * ny;
    p1.velocityZ += impulse * nz;

    p2.velocityX -= impulse * nx;
    p2.velocityY -= impulse * ny;
    p2.velocityZ -= impulse * nz;

    // Separate the particles slightly to avoid overlapping
    float overlap = 0.5f * (p1.radius + p2.radius - distance);
    p1.x += overlap * nx;
    p1.y += overlap * ny;
    p1.z += overlap * nz;

    p2.x -= overlap * nx;
    p2.y -= overlap * ny;
    p2.z -= overlap * nz;
}

void update(float deltaTime, const glm::mat4& rotationMatrix) {
    
    // glm::vec3 velocityVec(velocityX, velocityY, velocityZ);
    // velocityVec = glm::vec3(rotationMatrix * glm::vec4(velocityVec, 0.0f));

    // velocityX = velocityVec.x;
    // velocityY = velocityVec.y;
    // velocityZ = velocityVec.z;

    // Apply gravity
    Kinematics::applyGravity(velocityX, velocityY, velocityZ, deltaTime, rotationMatrix);

     // Update position based on velocity
    x += velocityX * deltaTime;
    y += velocityY * deltaTime;
    z += velocityZ * deltaTime;

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