#pragma once
#ifndef BASICMATERIAL_H
#define BASICMATERIAL_H

/**
 * @file BasicMaterial.h
 *
 * @brief Material that doesn't lighting into account.
 *
 * Martin Johnson
 * Billy-Joe Lacasse
 * William Lebel
 */

#include "Material.h"

class BasicMaterial : public Material {
public:
	BasicMaterial();

	virtual void bind() const override;
	virtual GLint positionAttribLocation() const override;
	virtual GLint normalAttribLocation() const override;

	void setColor(const glm::vec3& color);
	void setWireframe(bool value);

protected:
	virtual bool init_impl() override;

	virtual inline std::string vertexShader() const override { return "basicShader.vert"; }
	virtual inline std::string fragmentShader() const override { return "basicShader.frag"; }

private:
	int m_vPositionLocation = -1;

	const std::string vPositionAttributeName = "vPosition";
	const std::string uColorAttributeName = "uColor";

	bool m_wireframe = false;
};
#endif
