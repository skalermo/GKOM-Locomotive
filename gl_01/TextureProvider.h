#pragma once
#include <unordered_map>
#include <string>
#include <tuple>
#include <SOIL.h>

class TextureProvider {
	std::unordered_map<std::string, std::tuple<unsigned char*, int, int>> textures;
public:
	static TextureProvider& instance() {
		static TextureProvider provider;
		return provider;
	}

	//returns texture pointer, width, height
	auto getTexture(const std::string& texturePath) {
		if (textures.find(texturePath) == textures.end())
		{
			int width, height;
			const auto image = SOIL_load_image(texturePath.c_str(), &width, &height, nullptr, SOIL_LOAD_AUTO);
			if (image == nullptr)
				throw std::exception(std::string("Failed to load texture file " + texturePath).c_str());

			textures[texturePath] = std::make_tuple(image, width, height);
		}


		return textures[texturePath];
	}

	void flushTextures()
	{
		for(auto& texture : textures)
		{
			const auto texturePointer = std::get<0>(texture.second);
			SOIL_free_image_data(texturePointer);
		}
		textures.clear();
	}
};
