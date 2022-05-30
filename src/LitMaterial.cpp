/**
 * @file LitMaterial.cpp
 *
 * @brief Material that takes lighting into account.
 *
 * Martin Johnson
 * Billy-Joe Lacasse
 * William Lebel
 */

#include "LitMaterial.h"

#include <glad/glad.h>
#include <iostream>

LitMaterial::LitMaterial() : Material()
{
}

void LitMaterial::bind() const
{
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	Material::bind();
}


GLint LitMaterial::positionAttribLocation() const
{
	return m_vPositionLocation;
}

GLint LitMaterial::normalAttribLocation() const
{
	return m_vNormalLocation;
}

void LitMaterial::setAmbiantColor(const glm::vec3& color)
{
	m_shaderProgram->setVec3(uAmbiantColorAttributeName, color);
}

void LitMaterial::setDiffuseColor(const glm::vec3& color)
{
	m_shaderProgram->setVec3(uDiffuseColorAttributeName, color);
}

void LitMaterial::setSpecularColor(const glm::vec3& color)
{
	m_shaderProgram->setVec3(uSpecularColorAttributeName, color);
}

void LitMaterial::setSpecularExponent(float exponent)
{
	m_shaderProgram->setFloat(uSpecularExponentAttributeName, exponent);
}

void LitMaterial::setLightPosition(const glm::vec3& position)
{
	m_shaderProgram->setVec3(uLightPositionAttributeName, position);
}

void LitMaterial::setLightColor(const glm::vec4& color)
{
	m_shaderProgram->setVec4(uLightColorAttributeName, color);
}

bool LitMaterial::init_impl()
{
	if ((m_vPositionLocation = m_shaderProgram->attributeLocation(vPositionAttributeName)) < 0) {
		std::cerr << "Unable to find shader location for " << vPositionAttributeName << std::endl;
		return false;
	}

	if ((m_vNormalLocation = m_shaderProgram->attributeLocation(vNormalAttributeName)) < 0) {
		std::cerr << "Unable to find shader location for " << vNormalAttributeName << std::endl;
		return false;
	}

	return true;
}

