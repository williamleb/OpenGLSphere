#pragma once
#ifndef MATERIAL_H
#define MATERIAL_H

/**
 * @file Material.h
 *
 * @brief ShaderProgram wrapper that allows easy parametrization.
 *
 * Martin Johnson
 * Billy-Joe Lacasse
 * William Lebel
 */

#include <memory>
#include <string>

#include "ShaderProgram.h"

class Material {
public:
	Material();

	bool init();
	virtual void bind() const;

	virtual GLint positionAttribLocation() const = 0;
	virtual GLint normalAttribLocation() const = 0;

	void setProjection(const glm::mat4& projection);
	void setView(const glm::mat4& view);
    void setViewPost(glm::vec3 viewPosition);
    void setPhong(bool phong);

protected:
	virtual bool init_impl() { return true; };

	virtual inline std::string vertexShader() const = 0;
	virtual inline std::string fragmentShader() const = 0;
	virtual inline std::string tessControlShader() const { return ""; };
	virtual inline std::string tessEvaluationShader() const { return ""; };
	virtual inline std::string geometryShader() const { return ""; };
	virtual inline std::string computeShader() const { return ""; };

private:
	bool initShaders();

protected:
	std::unique_ptr<ShaderProgram> m_shaderProgram = nullptr;

private:
	const std::string directory = SHADERS_DIR;

	const std::string projectionAttributeName = "projection";
	const std::string viewAttributeName = "view";
	const std::string viewPosAttributeName = "viewPos";
	const std::string phongAttributeName = "phong";

};
#endif
