#pragma once

#include <vector>
#include "Shader.h"
#include "Camera.h"
#include "LightSrc.h"
#include "ShaderProvider.h"


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
		std::unique_ptr<LightSrc> lightsrc;

	public:
		PointLight(const glm::vec3& position,
			const glm::vec3& ambient,
			const glm::vec3& diffuse,
			const glm::vec3& specular,
			const GLfloat& constant,
			const GLfloat& linear,
			const GLfloat& quadratic)
			: position(position),
			ambient(ambient), 
			diffuse(diffuse), 
			specular(specular), 
			constant(constant), 
			linear(linear), 
			quadratic(quadratic),
			lightsrc(std::make_unique<LightSrc>(position, 0.5f))
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

		void move(const glm::vec3& displacement)
		{
			lightsrc->move(displacement);
			position += displacement;
		}

		void setPosition(const glm::vec3& newPosition)
		{
			lightsrc->move(newPosition - position);
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

		void drawLightSrc(std::shared_ptr<Shader> shader)
		{
			lightsrc->draw(shader);
		}
	};

	std::shared_ptr<Shader> lightSrcShader;
	std::unique_ptr<DirectionalLight> dirLight;
	std::shared_ptr<PointLight> pointLight;
	
	Camera camera;
	
public:
	LightManager(const Camera& camera)
	{
		this->camera = camera;
		dirLight = std::make_unique<DirectionalLight>(DirectionalLight());
	}

	void setDirLight()
	{
		setDirLight({1.0f, -0.62f, 0.175f},
					{0.35f, 0.35f, 0.35f},
					{0.5f, 0.5f, 0.5f},
					{0.3f, 0.3f, 0.3f});
	}

	void setDirLight(const glm::vec3& direction, 
					const glm::vec3& ambient, 
					const glm::vec3& diffuse, 
					const glm::vec3& specular)
	{
		dirLight->setDirection(direction);
		dirLight->setAmbient(ambient);
		dirLight->setDiffuse(diffuse);
		dirLight->setSpecular(specular);
	}

	void movePointLight(glm::vec3 displacement)
	{
		if (!pointLight)
			return;
		pointLight->move(displacement);
	}

	std::shared_ptr<PointLight> getPointLight()
	{
		if (!pointLight)
			return getPointLight({ -3.0f, 4.2f, 2.0f },
				glm::vec3(0.2f),
				glm::vec3(1.0f),
				glm::vec3(0.8f),
				1.0f,
				0.09f,
				0.032f);

		return pointLight;
	}

	std::shared_ptr<PointLight> getPointLight(const glm::vec3& position,
											const glm::vec3& ambient,
											const glm::vec3& diffuse,
											const glm::vec3& specular,
											const GLfloat& constant,
											const GLfloat& linear,
											const GLfloat& quadratic)
	{
		if (!pointLight)
		{
			lightSrcShader = LightSrc::getShaderPtr();
			pointLight = std::make_shared<PointLight>(PointLight(position,
				ambient,
				diffuse,
				specular,
				constant,
				linear,
				quadratic));
		}
		return pointLight;
	}

	void applyLightToShader(std::shared_ptr<Shader> shader)
	{
		shader->use();
		shader->setVec3f("viewPos", camera.Position);
		shader->setVec3f("dirLight.direction", dirLight->getDirection());
		shader->setVec3f("dirLight.ambient", dirLight->getAmbient() + 0.15f );
		shader->setVec3f("dirLight.diffuse", dirLight->getDiffuse());
		shader->setVec3f("dirLight.specular", dirLight->getSpecular());

		shader->setVec3f("pointLight.position", pointLight->getPosition());
		shader->setVec3f("pointLight.ambient", pointLight->getAmbient());
		shader->setVec3f("pointLight.diffuse", pointLight->getDiffuse());
		shader->setVec3f("pointLight.specular", pointLight->getSpecular());
		shader->setFloat("pointLight.constant", pointLight->getConstant());
		shader->setFloat("pointLight.linear", pointLight->getLinear());
		shader->setFloat("pointLight.quadratic", pointLight->getQuadratic());
		pointLight->drawLightSrc(this->lightSrcShader);

	}


};