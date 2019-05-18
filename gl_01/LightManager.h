#pragma once

#include <vector>
#include "Shader.h"
#include "Camera.h"
#include "LightSrc.h"

class LightManager {
private:
	class DirectionalLight {
	private:
		glm::vec3 direction;
		glm::vec3 ambient;
		glm::vec3 diffuse;
		glm::vec3 specular;

	public:
		glm::vec3 getDirection()
		{
			return direction;
		}

		glm::vec3 getAmbient()
		{
			return ambient;
		}

		glm::vec3 getDiffuse()
		{
			return diffuse;
		}

		glm::vec3 getSpecular()
		{
			return specular;
		}

		void setDirection(glm::vec3 newDirection)
		{
			direction = newDirection;
		}

		void setAmbient(glm::vec3 newAmbient)
		{
			ambient = newAmbient;
		}

		void setDiffuse(glm::vec3 newDiffuse)
		{
			diffuse = newDiffuse;
		}

		void setSpecular(glm::vec3 newSpecular)
		{
			specular = newSpecular;
		}
	};

	class PointLight {
	private:
		glm::vec3 position;
		glm::vec3 ambient;
		glm::vec3 diffuse;
		glm::vec3 specular;
		GLfloat constant;
		GLfloat linear;
		GLfloat quadratic;
		LightSrc lightsrc;

	public:
		PointLight(const glm::vec3& position,
			const glm::vec3& ambient,
			const glm::vec3& diffuse,
			const glm::vec3& specular,
			const GLfloat& constant,
			const GLfloat& linear,
			const GLfloat& quadratic)
			: ambient(ambient), 
			diffuse(diffuse), 
			specular(specular), 
			constant(constant), 
			linear(linear), 
			quadratic(quadratic),
			lightsrc(LightSrc(position, 0.5f))
		{}
		
		glm::vec3 getPosition()
		{
			return position;
		}

		glm::vec3 getAmbient()
		{
			return ambient;
		}

		glm::vec3 getDiffuse()
		{
			return diffuse;
		}

		glm::vec3 getSpecular()
		{
			return specular;
		}

		GLfloat getConstant()
		{
			return constant;
		}

		GLfloat getLinear()
		{
			return linear;
		}

		GLfloat getQuadratic()
		{
			return quadratic;
		}

		void setPosition(const glm::vec3& newPosition)
		{
			position = newPosition;
		}

		void setAmbient(const glm::vec3& newAmbient)
		{
			ambient = newAmbient;
		}

		void setDiffuse(const glm::vec3& newDiffuse)
		{
			diffuse = newDiffuse;
		}

		void setSpecular(const glm::vec3& newSpecular)
		{
			specular = newSpecular;
		}

		void setConstant(const GLfloat& newConstant)
		{
			constant = newConstant;
		}

		void setLinear(const GLfloat& newLinear)
		{
			linear = newLinear;
		}

		void setQuadratic(const GLfloat& newQuadratic)
		{
			quadratic = newQuadratic;
		}

		void drawLightSrc()
		{
			lightsrc.draw();
		}
	};

	std::shared_ptr<Shader> shader;
	DirectionalLight dirLight;
	std::vector<std::unique_ptr<PointLight>> pointLights;
	
	Camera* camera;
	
public:
	LightManager(Camera* camera)
	{
		this->camera = camera;
		dirLight = DirectionalLight();
	}

	/*void setShader(std::shared_ptr<Shader> shader)
	{
		this->shader = shader;
	}*/

	void setDirLight(const glm::vec3& direction, 
					const glm::vec3& ambient, 
					const glm::vec3& diffuse, 
					const glm::vec3& specular)
	{
		dirLight.setDirection(direction);
		dirLight.setAmbient(ambient);
		dirLight.setDiffuse(diffuse);
		dirLight.setSpecular(specular);
	}

	void addNewPointLight()
	{
		addNewPointLight({ 0.0f, 0.0f, 0.0f },
						{ 0.05f, 0.05f, 0.05f },
						{ 0.5f, 0.5f, 0.5f },
						{ 0.3f, 0.3f, 0.3f },
						1.0f,
						0.09f,
						0.032f);
	}

	void addNewPointLight(const glm::vec3& position,
						const glm::vec3& ambient,
						const glm::vec3& diffuse,
						const glm::vec3& specular,
						const GLfloat& constant,
						const GLfloat& linear,
						const GLfloat& quadratic)
	{

		pointLights.push_back(std::make_unique<PointLight>(PointLight(position,
										ambient,
										diffuse,
										specular,
										constant,
										linear,
										quadratic)));

	}

	void popLastLight()
	{
		pointLights.pop_back();
	}

	void applyLightToShader(std::shared_ptr<Shader> shader)
	{
		shader->use();
		shader->setInt("nr_point_lights", pointLights.size());
		shader->setVec3f("viewPos", camera->Position);
		shader->setVec3f("dirLight.direction", dirLight.getDirection());
		shader->setVec3f("dirLight.ambient", dirLight.getAmbient());
		shader->setVec3f("dirLight.diffuse", dirLight.getDiffuse());
		shader->setVec3f("dirLight.specular", dirLight.getSpecular());

		for (size_t i = 0; i < pointLights.size(); i++)
		{
			pointLights[i]->drawLightSrc();
			shader->use();
			shader->setVec3f("pointLights[" + std::to_string(i) + "].position", pointLights[i]->getPosition());
			shader->setVec3f("pointLights[" + std::to_string(i) + "].ambient", pointLights[i]->getAmbient());
			shader->setVec3f("pointLights[" + std::to_string(i) + "].diffuse", pointLights[i]->getDiffuse());
			shader->setVec3f("pointLights[" + std::to_string(i) + "].specular", pointLights[i]->getSpecular());
			shader->setFloat("pointLights[" + std::to_string(i) + "].constant", pointLights[i]->getConstant());
			shader->setFloat("pointLights[" + std::to_string(i) + "].linear", pointLights[i]->getLinear());
			shader->setFloat("pointLights[" + std::to_string(i) + "].quadratic", pointLights[i]->getQuadratic());
		}
	}

	int getLightSrcsCount()
	{
		return pointLights.size();
	}


};