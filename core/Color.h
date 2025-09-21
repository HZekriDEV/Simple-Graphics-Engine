#ifndef COLOR_H
#define COLOR_H

#include <glm/glm.hpp>

class Color
{
	public:
        static glm::vec3 White() { return glm::vec3(1.0f, 1.0f, 1.0f); }
        static glm::vec3 Black() { return glm::vec3(0.0f, 0.0f, 0.0f); }
        static glm::vec3 Red() { return glm::vec3(1.0f, 0.0f, 0.0f); }
        static glm::vec3 Green() { return glm::vec3(0.0f, 1.0f, 0.0f); }
        static glm::vec3 Blue() { return glm::vec3(0.0f, 0.0f, 1.0f); }
        static glm::vec3 Yellow() { return glm::vec3(1.0f, 1.0f, 0.0f); }
        static glm::vec3 Cyan() { return glm::vec3(0.0f, 1.0f, 1.0f); }
        static glm::vec3 Magenta() { return glm::vec3(1.0f, 0.0f, 1.0f); }
        static glm::vec3 Gray() { return glm::vec3(0.5f, 0.5f, 0.5f); }

        static glm::vec3 NewColor(float r, float g, float b) { return glm::vec3(r, g, b); }
};

#endif 

