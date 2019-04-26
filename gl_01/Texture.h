#pragma once
#include <string>
#include <memory>
#include "Shader.h"
#include "SOIL.h"

class Texture {
	int width;
	int height;
	int textureIndex;
	std::string textureName;
	GLuint texture;

	void setActiveTexture() const
	{
		if (textureIndex == 0)
			glActiveTexture(GL_TEXTURE0);
		else if (textureIndex == 1)
			glActiveTexture(GL_TEXTURE1);
		else if (textureIndex == 2)
			glActiveTexture(GL_TEXTURE2);
		else if (textureIndex == 3)
			glActiveTexture(GL_TEXTURE3);
	}
public:

	Texture() {}

	/**
	 * \brief loads an image from disc and sets it to the program
	 * \param texturePath path to the texture on the disc
	 * \param shader shared pointer to the object shader
	 * \param textureName name of a texture that will be visible in shader
	 * \param textureIndex index of the texture in the shader(only range (0...3) is available
	 */
	Texture(const std::string& texturePath, std::shared_ptr<Shader> shader, std::string textureName = "Texture0", const int textureIndex = 0)
		: textureIndex(textureIndex), textureName(textureName) {
		const auto image = SOIL_load_image(texturePath.c_str(), &width, &height, nullptr, SOIL_LOAD_RGB);
		if (image == nullptr)
			throw std::exception("Failed to load texture file");

		glGenTextures(1, &texture);

		setActiveTexture();
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glGenerateMipmap(GL_TEXTURE_2D);
		SOIL_free_image_data(image);
	}

	GLuint getIndex() const {
		return texture;
	}

	int getWidth() const
	{
		return width;
	}
	int getHeight() const
	{
		return height;
	}

	/**
	 * \brief use in in the `draw()` method of an object
	 * \param shader pointer to the object's shader
	 */
	void useTexture(std::shared_ptr<Shader> shader) const
	{
		shader->use();
		setActiveTexture();
		glBindTexture(GL_TEXTURE_2D, texture);
		shader->setInt(textureName, textureIndex);
	}
};
