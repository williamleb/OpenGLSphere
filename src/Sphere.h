#pragma once
#ifndef SPHERE_H
#define SPHERE_H

/**
 * @file Sphere.h
 *
 * @brief Sphere geometry that can be drawn on screen with a material.
 *
 * Martin Johnson
 * Billy-Joe Lacasse
 * William Lebel
 */

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <vector>
#include <memory>

class Material;

class Sphere {
public:
	Sphere(float radius, int longitude, int latitude, std::shared_ptr<const Material> material);

	void render();

	void setRadius(float radius);
	void setLongitude(int longitude);
	void setLatitude(int latitude);
	void setMaterial(std::shared_ptr<const Material> material);

private:
	void initGeometryBuffersAndVAO();

	void generateVertices(std::vector<GLfloat>& outVertices, std::vector<GLfloat>& outNormals) const;
	void generateSurroundingVertices(std::vector<GLfloat>& outVertices, std::vector<GLfloat>& outNormals) const;
	void generateCapVertices(std::vector<GLfloat>& outVertices, std::vector<GLfloat>& outNormals) const;

	void generateIndices(std::vector<GLuint>& outIndices) const;
	void generateSurroundingIndices(std::vector<GLuint>& outIndices) const;
	void generateCapIndices(std::vector<GLuint>& outIndices) const;

	void initBuffersAndVAO(const std::vector<GLfloat>& vertices, const std::vector<GLfloat>& normals, const std::vector<GLuint>& indices);
	void fillBuffers(const std::vector<GLfloat>& vertices, const std::vector<GLfloat>& normals, const std::vector<GLuint>& indices);
	void updateBuffers();
	void updateAttributeLocations(const std::vector<GLfloat>& vertices);

	void updateNumTriSphere();

	std::shared_ptr<const Material> m_material;

	int m_numTriSphere;
	
	float m_radius;
	int m_longitude;
	int m_latitude;

	enum VAO_IDs { VAO_Sphere, NumVAOs };
	enum Buffer_IDs { VBO_Sphere, EBO_Sphere, NumBuffers };
	GLuint m_VAOs[NumVAOs];
	GLuint m_buffers[NumBuffers];
};
#endif
