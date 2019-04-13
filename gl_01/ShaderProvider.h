#pragma once
#include "Shader.h"
#include <vector>
#include <tuple>
#include <string>
#include <memory>
#include <algorithm>

class ShaderProvider {
	std::vector < std::tuple<std::string, std::string, std::shared_ptr<Shader>>> shaders;
public:
	static ShaderProvider& instance() {
		static ShaderProvider provider;
		return provider;
	}

	std::shared_ptr<Shader> getShader(std::string vertexName, std::string fragmentName) {
		auto shader = std::find_if(shaders.begin(), shaders.end(), [&](auto item) {
			auto vertex = std::get<0>(item);
			auto fragment = std::get<1>(item);
			return vertex == vertexName && fragment == fragmentName;
		});

		if (shader != shaders.end()) {
			return std::get<2>(*shader);
		}

		auto newShader = std::make_shared<Shader>(vertexName.c_str(), fragmentName.c_str());
		shaders.emplace_back(std::make_tuple(vertexName, fragmentName, newShader));
		return newShader;
	}
};
