#pragma once
#ifndef LITMATERIAL_H
#define LITMATERIAL_H

/**
 * @file LitMaterial.h
 *
 * @brief Material that takes lighting into account.
 *
 * Martin Johnson
 * Billy-Joe Lacasse
 * William Lebel
 */

#include "Material.h"

class LitMaterial : public Material {
public:
	LitMaterial();

	virtual void bind() const override;
	virtual GLint positionAttribLocation() const override;
	virtual GLint normalAttribLocation() const override;

	void setAmbiantColor(const glm::vec3& color);
	void setDiffuseColor(const glm::vec3& color);
	void setSpecularColor(const glm::vec3& color);
	void setSpecularExponent(float exponent);

	void setLightPosition(const glm::vec3& position);
	void setLightColor(const glm::vec4& color);


protected:
	virtual bool init_impl() override;

	virtual inline std::string vertexShader() const override { return "litShader.vert"; }
	virtual inline std::string fragmentShader() const override { return "litShader.frag"; }

private:
	int m_vPositionLocation = -1;
	int m_vNormalLocation = -1;

	const std::string vPositionAttributeName = "vPosition";
	const std::string vNormalAttributeName = "vNormal";

	const std::string uAmbiantColorAttributeName = "uAmbiantColor";
	const std::string uDiffuseColorAttributeName = "uDiffuseColor";
	const std::string uSpecularColorAttributeName = "uSpecularColor";
	const std::string uSpecularExponentAttributeName = "uSpecularExponent";

	const std::string uLightPositionAttributeName = "uLightPosition";
	const std::string uLightColorAttributeName = "uLightColor";
};
#endif
