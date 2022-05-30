#pragma once

/**
 * @file MainWindow.h
 *
 * @brief Represents the application.
 *
 * Martin Johnson
 * Billy-Joe Lacasse
 * William Lebel
 */

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/glm.hpp>

#include <iostream>
#include <vector>
#include <memory>
#include <glm/gtx/euler_angles.hpp>

#include "ShaderProgram.h"
#include "Sphere.h"
#include "BasicMaterial.h"
#include "LitMaterial.h"
#include "Camera.h"

class MainWindow
{
public:
	MainWindow();

	// Main functions (initialization, run)
	int initialisation();
	int renderLoop();

	// Callback to intersept GLFW calls
	void framebufferSizeCallback(int width, int height);

private:
	// Initialize GLFW callbacks
	void initializeCallback();
	// Intiialize OpenGL objects (shaders, ...)
	int initializeGL(); 
	
	// Rendering scene (OpenGL)
	void renderScene();
	// Rendering interface ImGUI
	void renderImgui();

    void handleMouse(double xpos, double ypos);
    void handleScroll(double yDelta);
    void processInput();
    void updateCamera();

private:
	Material* currentMaterial();

	// Settings
	const unsigned int SCR_WIDTH = 900;
	const unsigned int SCR_HEIGHT = 900;
	GLFWwindow* m_window = nullptr;

	// Material type
	enum class MaterialType {Lit, Unlit, Wireframe};
	MaterialType m_materialType = MaterialType::Lit;

	// Shading information
	glm::vec3 m_ambiant = glm::vec3(0.05f, 0.05f, 0.1f);
	glm::vec3 m_diffuse = glm::vec3(0.0f, 0.0f, 1.0f);
	glm::vec3 m_specular = glm::vec3(0.1f, 0.5f, 1.0f);
	float m_sExponent = 128.0f;

	glm::vec3 m_lightPosition = glm::vec3(1.0f, 1.0f, 1.0f);
	glm::vec4 m_lightColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);

	// Sphere subdivision information
	float m_radius = 0.9f;
	int m_longitude = 22;
	int m_latitude = 20;

	std::shared_ptr<BasicMaterial> m_sphereMaterial;
	std::shared_ptr<LitMaterial> m_sphereLitMaterial;
	std::unique_ptr<Sphere> m_sphere;


    Camera cam = Camera(glm::vec3(3.0,0.0,0.0));
    float m_deltaTime = 0.0f;
    float m_lastFrame = 0.0f;

    bool mouseRightDown = false;
    float lastX, lastY;
    bool firstMouse = true;

    bool phong = false;
    bool camEnable = true;
};
