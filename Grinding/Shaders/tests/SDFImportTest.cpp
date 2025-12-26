#include "preprocess.h"
#include <gtest/gtest.h>
#include <iostream>
#include <memory>

TEST(GrindingShadersTest, preprocessShaderTest)
{
	auto preprocessor = std::make_unique<glsl::Preprocessor>();
	auto path = std::string(TESTS_SOURCE_DIR) + "/test.glsl";
	auto shaderSources = preprocessor->preprocess(path);
	std::cout << "SHADERS: " << shaderSources << std::endl;
}
