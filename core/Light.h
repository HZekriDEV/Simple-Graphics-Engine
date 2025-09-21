#ifndef LIGHT_H
#define LIGHT_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include "shader.h"

class Light
{
	public:
		glm::vec3 color;
		float intensity;

		Light(const glm::vec3& color = glm::vec3(1.0f), float intensity = 1.0f)
			: color(color), intensity(intensity) {}

		virtual void ApplyToShader(Shader& shader, int index) const = 0;
};

class DirectionalLight : public Light
{
	public:
		glm::vec3 direction;

		DirectionalLight(const glm::vec3& direction, const glm::vec3& color, float intensity)
			: Light(color, intensity), direction(direction) {}

		void ApplyToShader(Shader& shader, int index) const override
		{
			std::string prefix = "directionalLights[" + std::to_string(index) + "].";
			shader.SetVec3(prefix + "direction", direction);

			glm::vec3 cxi = color * intensity;
			shader.SetVec3(prefix + "color", cxi);
		}
};

class PointLight : public Light
{
	public:
		glm::vec3 position;
		float constant;
		float linear;
		float quadratic;

		PointLight(const glm::vec3& position, const glm::vec3& color, float intensity, float constant = 1.0f, float linear = 0.09f, float quadratic = 0.032f)
			: Light(color, intensity), position(position), constant(constant), linear(linear), quadratic(quadratic) {}

		void ApplyToShader(Shader& shader, int index) const override
		{
			std::string prefix = "pointLights[" + std::to_string(index) + "].";
			shader.SetVec3(prefix + "position", position);
		
			glm::vec3 cxi = color * intensity;
			shader.SetVec3(prefix + "color", cxi);

			shader.SetFloat(prefix + "constant", constant);
			shader.SetFloat(prefix + "linear", linear);
			shader.SetFloat(prefix + "quadratic", quadratic);
		}
};

class SpotLight : public Light
{
	public:
		glm::vec3 position, direction;
		float cutOff, outerCutOff;

		SpotLight(const glm::vec3& position, const glm::vec3& direction, const glm::vec3& color, float intensity, float cutOff, float outerCutOff)
			: Light(color, intensity), position(position), direction(direction), cutOff(cutOff), outerCutOff(outerCutOff) {}

		void ApplyToShader(Shader& shader, int index) const override
		{
			std::string prefix = "spotLights[" + std::to_string(index) + "].";
			shader.SetVec3(prefix + "position", position);
			shader.SetVec3(prefix + "direction", direction);

			glm::vec3 cxi = color * intensity;
			shader.SetVec3(prefix + "color", cxi);

			shader.SetFloat(prefix + "cutOff", glm::cos(glm::radians(cutOff)));
			shader.SetFloat(prefix + "outerCutOff", glm::cos(glm::radians(outerCutOff)));
		}
};

class LightManager
{
	public:
		std::vector<PointLight> pointLights;
		std::vector<DirectionalLight> directionalLights;
		std::vector<SpotLight> spotLights;

		void ApplyLightsToShader(Shader& shader) const
		{
			shader.Activate();

			// Apply directional lights
			for (size_t i = 0; i < directionalLights.size(); ++i) 
			{
				directionalLights[i].ApplyToShader(shader, static_cast<int>(i));
			}
			
			// Apply point lights
			for (size_t i = 0; i < pointLights.size(); ++i) 
			{
				pointLights[i].ApplyToShader(shader, static_cast<int>(i));
			}

			// Apply spotlights
			for (size_t i = 0; i < spotLights.size(); ++i) 
			{
				spotLights[i].ApplyToShader(shader, static_cast<int>(i));
			}

			shader.SetInt("numDirLights", directionalLights.size());
			shader.SetInt("numPointLights", pointLights.size());
			shader.SetInt("numSpotLights", spotLights.size());
		}
};

#endif