/**
 * @file BasicMaterial.cpp
 *
 * @brief Material that doesn't lighting into account.
 *
 * Martin Johnson
 * Billy-Joe Lacasse
 * William Lebel
 */

#include "BasicMaterial.h"

#include <glad/glad.h>
#include <iostream>

BasicMaterial::BasicMaterial() : Material()
{
}

void BasicMaterial::bind() const
{
	if (m_wireframe)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	else 
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	Material::bind();
}

GLint BasicMaterial::positionAttribLocation() const
{
	return m_vPositionLocation;
}

GLint BasicMaterial::normalAttribLocation() const
{
	return GLint(-1);
}

void BasicMaterial::setColor(const glm::vec3& color)
{
	m_shaderProgram->setVec3(uColorAttributeName, color);
}

void BasicMaterial::setWireframe(bool value)
{
	m_wireframe = value;
}

bool BasicMaterial::init_impl()
{
	if ((m_vPositionLocation = m_shaderProgram->attributeLocation(vPositionAttributeName)) < 0) {
		std::cerr << "Unable to find shader location for " << vPositionAttributeName << std::endl;
		return false;
	}

	return true;
}

