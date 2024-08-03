#include "pch.h"

#include "Ray.h"

Ray::Ray(const glm::vec3 dir, const glm::vec3 org) {
    direction = dir;
    origin = org;
}

Ray::Ray() {
    direction = glm::vec3(0, 0, 0);
    origin = glm::vec3(0, 0, 0);
}

void Ray::UpdateRay(const glm::vec3 dir, const glm::vec3 org) {
    direction = dir;
    origin = org;
}

bool Ray::intersectsTriangle(std::vector<glm::vec3> vertices, glm::mat4 ModelMatrix) const {
    int i = -1;
    while (i < vertices.size() - 3) {
        glm::vec4 point1 = glm::vec4(vertices[++i].x, vertices[++i].y, vertices[++i].z, 1) * ModelMatrix; //a
        glm::vec4 point2 = glm::vec4(vertices[++i].x, vertices[++i].y, vertices[++i].z, 1) * ModelMatrix; //b
        glm::vec4 point3 = glm::vec4(vertices[++i].x, vertices[++i].y, vertices[++i].z, 1) * ModelMatrix; //c

        glm::vec3 AB = glm::vec3(point2.x - point1.x, point2.y - point1.y, point2.z - point1.z);
        glm::vec3 AC = glm::vec3(point3.x - point1.x, point3.y - point1.y, point3.z - point1.z);

        glm::vec3 normal = glm::cross(AB, AC);
        float n_dot_d = glm::dot(direction, normal);

        if (n_dot_d > 0.0001f) {
            float n_dot_ps = glm::dot(normal, glm::vec3(point1.x, point1.y, point1.z) - origin);

            float t = n_dot_ps / n_dot_d;

            glm::vec3 planePoint = origin + direction * t;

            glm::vec3 testPA = glm::vec3(planePoint.x - point1.x, planePoint.y - point1.y, planePoint.z - point1.z);
            //glm::vec3 testC = vec3(planePoint.x - point3.x, planePoint.y - point3.y, planePoint.z- point3.z);
            glm::vec3 Testnormal = glm::cross(AB, testPA);
			
            return glm::dot(Testnormal, normal);
        }
        return false;
    }
    return false;
}
