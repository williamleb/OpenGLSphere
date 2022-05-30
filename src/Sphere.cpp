/**
 * @file Sphere.cpp
 *
 * @brief Sphere geometry that can be drawn on screen with a material.
 *
 * Martin Johnson
 * Billy-Joe Lacasse
 * William Lebel
 */

#include <cassert>

#include "Sphere.h"
#include "Material.h"
#include "glm/ext/matrix_transform.hpp"

#define BUFFER_OFFSET(i) ((char *)NULL + (i))

Sphere::Sphere(float radius, int longitude, int latitude, std::shared_ptr<const Material> material):
	m_radius(radius), m_longitude(longitude), m_latitude(latitude), m_material(material),
	m_VAOs(), m_buffers()
{
	assert(material != nullptr);

	updateNumTriSphere();
	initGeometryBuffersAndVAO();
}

void Sphere::render()
{
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	m_material->bind();
	glBindVertexArray(m_VAOs[VAO_Sphere]);
	glDrawElements(GL_TRIANGLES, m_numTriSphere * 3, GL_UNSIGNED_INT, 0);
}

void Sphere::setRadius(float radius)
{
	if (radius <= 0.0f)
		return;

	m_radius = radius;

	updateBuffers();
}

void Sphere::setLongitude(int longitude)
{
	if (m_longitude == longitude || longitude < 1)
		return;

	m_longitude = longitude;
	updateNumTriSphere();

	updateBuffers();
}

void Sphere::setLatitude(int latitude)
{
	if (m_latitude == latitude || latitude < 1)
		return;

	m_latitude = latitude;
	updateNumTriSphere();

	updateBuffers();
}

void Sphere::setMaterial(std::shared_ptr<const Material> material)
{
	assert(material != nullptr);

	m_material = material;

	updateBuffers();
}

void Sphere::initGeometryBuffersAndVAO()
{
	std::vector<GLfloat> vertices;
	std::vector<GLfloat> normals;
	std::vector<GLuint> indices;

	generateVertices(vertices, normals);
	generateIndices(indices);

	initBuffersAndVAO(vertices, normals, indices);
}

void Sphere::generateVertices(std::vector<GLfloat>& outVertices, std::vector<GLfloat>& outNormals) const
{
	generateSurroundingVertices(outVertices, outNormals);
	generateCapVertices(outVertices, outNormals);
}

static const float PI = 3.14159265f;

void Sphere::generateSurroundingVertices(std::vector<GLfloat>& outVertices, std::vector<GLfloat>& outNormals) const
{
	// Le code de cette m�thode provient de la d�monstration de cr�ation d'une sph�re distribu�e en classe
	const float thetaInc = 2.0f * PI / static_cast<float>(m_longitude);
	const float phiInc = PI / static_cast<float>(m_latitude + 1);

	for (int row = 0; row < m_latitude; ++row)
	{
		// You can think of Phi as sweeping the sphere from the South pole to the North pole
		const float phi = PI - (static_cast<float>(row + 1) * phiInc);
		for (int col = 0; col < m_longitude; ++col)
		{
			// You can think of Theta as circling around the sphere, East to West
			float theta = col * thetaInc;

			// Spherical coordinates 
			glm::vec3 coordinates(m_radius * sin(theta) * sin(phi), m_radius * cos(phi), m_radius * cos(theta) * sin(phi));
			outVertices.push_back(coordinates.x);
			outVertices.push_back(coordinates.y);
			outVertices.push_back(coordinates.z);

			// Normal
			// Since the center of the sphere is at (0,0,0), the normal direction of the points is just their normalized location. 
			auto normal = glm::normalize(coordinates);
			outNormals.push_back(normal.x);
			outNormals.push_back(normal.y);
			outNormals.push_back(normal.z);
		}
	}
}

void Sphere::generateCapVertices(std::vector<GLfloat>& outVertices, std::vector<GLfloat>& outNormals) const
{
	// Le code de cette m�thode provient de la d�monstration de cr�ation d'une sph�re distribu�e en classe
	outVertices.push_back(0.0f);
	outVertices.push_back(-m_radius);
	outVertices.push_back(0.0f);

	outNormals.push_back(0.0f);
	outNormals.push_back(-1.0f);
	outNormals.push_back(0.0f);

	outVertices.push_back(0.0f);
	outVertices.push_back(m_radius);
	outVertices.push_back(0.0f);

	outNormals.push_back(0.0f);
	outNormals.push_back(1.0f);
	outNormals.push_back(0.0f);
}

void Sphere::generateIndices(std::vector<GLuint>& outIndices) const
{
	generateSurroundingIndices(outIndices);
	generateCapIndices(outIndices);
}

void Sphere::generateSurroundingIndices(std::vector<GLuint>& outIndices) const
{
	// Le code de cette m�thode provient de la d�monstration de cr�ation d'une sph�re distribu�e en classe
	for (int row = 0; row < m_latitude - 1; ++row)
	{
		unsigned int rowStart = row * m_longitude;
		unsigned int topRowStart = rowStart + m_longitude;

		for (int col = 0; col < m_longitude; ++col)
		{
			// Compute quad vertices
			unsigned int v = rowStart + col;
			unsigned int vi = (col < m_longitude - 1) ? v + 1 : rowStart;
			unsigned int vj = topRowStart + col;
			unsigned int vji = (col < m_longitude - 1) ? vj + 1 : topRowStart;

			// Add to indices
			outIndices.push_back(v);
			outIndices.push_back(vi);
			outIndices.push_back(vj);
			outIndices.push_back(vi);
			outIndices.push_back(vji);
			outIndices.push_back(vj);
		}
	}
}

void Sphere::generateCapIndices(std::vector<GLuint>& outIndices) const
{
	// Le code de cette m�thode provient de la d�monstration de cr�ation d'une sph�re distribu�e en classe
	for (int col = 0; col < m_longitude; ++col)
	{
		outIndices.push_back(m_longitude * m_latitude);
		outIndices.push_back((col < m_longitude - 1) ? col + 1 : 0);
		outIndices.push_back(col);

		unsigned int rowStart = (m_latitude - 1) * m_longitude;
		outIndices.push_back(m_longitude * m_latitude + 1);
		outIndices.push_back(rowStart + col);
		outIndices.push_back((col < m_longitude - 1) ? (rowStart + col + 1) : rowStart);
	}
}


void Sphere::initBuffersAndVAO(const std::vector<GLfloat>& vertices, const std::vector<GLfloat>& normals, const std::vector<GLuint>& indices)
{
	glGenVertexArrays(NumVAOs, m_VAOs);
	glGenBuffers(NumBuffers, m_buffers);

	fillBuffers(vertices, normals, indices);

	// Do not desactivate EBO when the VAO is still activated
	// as it will desactivate the EBO for this VAO 
	glBindVertexArray(0);
}

void Sphere::fillBuffers(const std::vector<GLfloat>& vertices, const std::vector<GLfloat>& normals, const std::vector<GLuint>& indices)
{
	glBindVertexArray(m_VAOs[VAO_Sphere]);

	glBindBuffer(GL_ARRAY_BUFFER, m_buffers[VBO_Sphere]);
	glBufferData(GL_ARRAY_BUFFER, long(sizeof(GLfloat) * vertices.size() + long(sizeof(GLfloat) * vertices.size())), nullptr, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, long(sizeof(GLfloat) * vertices.size()), vertices.data());
	glBufferSubData(GL_ARRAY_BUFFER, long(sizeof(GLfloat) * vertices.size()), long(sizeof(GLfloat) * normals.size()), normals.data());

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_buffers[EBO_Sphere]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * indices.size(), indices.data(), GL_STATIC_DRAW);

	updateAttributeLocations(vertices);
}

void Sphere::updateBuffers()
{
	std::vector<GLfloat> vertices;
	std::vector<GLfloat> normals;
	std::vector<GLuint> indices;

	generateVertices(vertices, normals);
	generateIndices(indices);

	fillBuffers(vertices, normals, indices);
}

void Sphere::updateAttributeLocations(const std::vector<GLfloat>& vertices)
{
	int positionLocation = m_material->positionAttribLocation();
	glVertexAttribPointer(positionLocation, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
	glEnableVertexAttribArray(positionLocation);

	int normalLocation = m_material->normalAttribLocation();
	if (normalLocation > -1)
	{
		glVertexAttribPointer(normalLocation, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(GLfloat) * vertices.size()));
		glEnableVertexAttribArray(normalLocation);
	}
}

void Sphere::updateNumTriSphere()
{
	m_numTriSphere = m_longitude * (m_latitude - 1) * 2 + 2 * m_longitude;
}
