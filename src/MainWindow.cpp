/**
 * @file MainWindow.cpp
 *
 * @brief Represents the application.
 *
 * Martin Johnson
 * Billy-Joe Lacasse
 * William Lebel
 */

#include "MainWindow.h"

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <iostream>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtc/quaternion.hpp>

MainWindow::MainWindow()
{
}

int MainWindow::initialisation()
{
	// OpenGL version (usefull for imGUI and other libraries)
	const char* glsl_version = "#version 430 core";

	// glfw: initialize and configure
	// ------------------------------
	glfwInit();

	// Request OpenGL 4.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	// glfw window creation
	// --------------------
	m_window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Labo 1", NULL, NULL);
	if (m_window == NULL)
	{
		std::cerr << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return 1;
	}

    glfwMakeContextCurrent(m_window);

	initializeCallback();

    // glad: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cerr << "Failed to initialize GLAD" << std::endl;
		return 2;
	}

	// imGui: create interface
	// ---------------------------------------
	// Setup Dear ImGui context
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;// Enable Keyboard Controls

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();

	// Setup Platform/Renderer backends
	ImGui_ImplGlfw_InitForOpenGL(m_window, true);
	ImGui_ImplOpenGL3_Init(glsl_version);

	// Other openGL initialization
	// -----------------------------
	return initializeGL();
}

void MainWindow::initializeCallback() {
	glfwSetWindowUserPointer(m_window, reinterpret_cast<void*>(this));
	glfwSetFramebufferSizeCallback(m_window, [](GLFWwindow* window, int width, int height) {
		MainWindow* w = reinterpret_cast<MainWindow*>(glfwGetWindowUserPointer(window));
		w->framebufferSizeCallback(width, height);
	});
    glfwSetCursorPosCallback(m_window, [](GLFWwindow* w, double x, double y) {
        MainWindow* mw = reinterpret_cast<MainWindow*>(glfwGetWindowUserPointer(w));
        mw->handleMouse(x, y);
    });
    glfwSetScrollCallback(m_window, [](GLFWwindow* w, double x, double y) {
        MainWindow* mw = reinterpret_cast<MainWindow*>(glfwGetWindowUserPointer(w));
        mw->handleScroll(y);
    });
}

int MainWindow::initializeGL()
{
	m_sphereMaterial = std::make_shared<BasicMaterial>();
	if (!m_sphereMaterial->init()) {
		return 3;
	}

	m_sphereLitMaterial = std::make_shared<LitMaterial>();
	if (!m_sphereLitMaterial->init()) {
		return 3;
	}

	m_sphere = std::make_unique<Sphere>(m_radius, m_longitude, m_latitude, m_sphereLitMaterial);

	glEnable(GL_DEPTH_TEST);

	return 0;
}

void MainWindow::renderImgui()
{
	// Start the Dear ImGui frame
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	//imgui
	{
		ImGui::Begin("Labo 1");

		// Material
		const char* materialNames[] = { "Lit", "Unlit", "Wireframe" };
		static int materialIndex = 0;
		if (ImGui::Combo("Shading", &materialIndex, materialNames, IM_ARRAYSIZE(materialNames))) 
		{
			switch (materialIndex)
			{
			case 0:
				m_materialType = MaterialType::Lit;
				m_sphere->setMaterial(m_sphereLitMaterial);
				break;
			case 1:
				m_materialType = MaterialType::Unlit;
				m_sphere->setMaterial(m_sphereMaterial);
				m_sphereMaterial->setWireframe(false);
				break;
			default:
				m_materialType = MaterialType::Wireframe;
				m_sphere->setMaterial(m_sphereMaterial);
				m_sphereMaterial->setWireframe(true);
				break;
			}
		}
		
		// Color
		ImGui::Separator();
		ImGui::Text("Colors: ");
		if (m_materialType == MaterialType::Lit)
		{
			ImGui::ColorEdit3("Ambiant", &m_ambiant[0]);
			ImGui::ColorEdit3("Diffuse", &m_diffuse[0]);
			ImGui::ColorEdit3("Specular", &m_specular[0]);
			ImGui::InputFloat("Specular exponent", &m_sExponent, 1.0f);

			m_sphereLitMaterial->setAmbiantColor(m_ambiant);
			m_sphereLitMaterial->setDiffuseColor(m_diffuse);
			m_sphereLitMaterial->setSpecularColor(m_specular);
			m_sphereLitMaterial->setSpecularExponent(m_sExponent);
		}
		else
		{
			ImGui::ColorEdit3("Base color", &m_diffuse[0]);
			m_sphereMaterial->setColor(m_diffuse);
		}

		// Light
		ImGui::Separator();
		ImGui::Text("Light: ");
		ImGui::InputFloat("X", &m_lightPosition[0], 0.05f);
		ImGui::InputFloat("Y", &m_lightPosition[1], 0.05f);
		ImGui::InputFloat("Z", &m_lightPosition[2], 0.05f);
		ImGui::ColorEdit4("Color", &m_lightColor[0]);

		if (m_materialType == MaterialType::Lit)
		{
			m_sphereLitMaterial->setLightPosition(m_lightPosition);
			m_sphereLitMaterial->setLightColor(m_lightColor);
		}


		// Other options
		ImGui::Separator();
		ImGui::Text("Sphere: ");
		bool changed = false;
		changed |= ImGui::InputFloat("Radius", &m_radius, 0.05f);
		changed |= ImGui::InputInt("Longitude", &m_longitude);
		changed |= ImGui::InputInt("Latitude", &m_latitude);
		if (changed) {
			m_sphere->setRadius(m_radius);
			m_sphere->setLongitude(m_longitude);
			m_sphere->setLatitude(m_latitude);
		}

        ImGui::Separator();
        ImGui::Text("Extra features");
        ImGui::Text("Lighting model");
        static int selection = 0;
        ImGui::RadioButton("Phong", &selection, 0);
        ImGui::SameLine();
        ImGui::RadioButton("Blinn-Phong", &selection, 1);
        phong = selection == 0;
        currentMaterial()->setPhong(phong);

        ImGui::Text("Use camera?");
        static int camSelection = 1;
        ImGui::RadioButton("Yes", &camSelection, 0);
        ImGui::SameLine();
        ImGui::RadioButton("No", &camSelection, 1);
        camEnable = camSelection == 0;

		ImGui::End();
	}

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void MainWindow::renderScene()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	m_sphere->render();
}


int MainWindow::renderLoop()
{

	while (!glfwWindowShouldClose(m_window))
	{
        auto currentFrame = static_cast<float>(glfwGetTime());
        m_deltaTime = currentFrame - m_lastFrame;
        m_lastFrame = currentFrame;

		processInput();
        updateCamera();

		renderScene();
		renderImgui();

		// Show rendering and get events
		glfwSwapBuffers(m_window);
		glfwPollEvents();
	}

	// Cleanup
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwDestroyWindow(m_window);
	glfwTerminate();

	return 0;
}

void MainWindow::framebufferSizeCallback(int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}

void MainWindow::processInput() {
    // Check inputs: Does ESC was pressed?
    if (glfwGetKey(m_window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(m_window, true);

    mouseRightDown = glfwGetMouseButton(m_window,GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS;
    if (mouseRightDown && camEnable) {
        glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        cam.ProcessKeyboard(m_window, m_deltaTime);
    }
    else {
        glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }
}

void MainWindow::updateCamera() {
	Material* material = currentMaterial();
    if (camEnable)
    {
        material->setProjection(
                glm::perspective(glm::radians(cam.Zoom), (float) SCR_WIDTH / (float) SCR_HEIGHT, 0.1f, 100.0f));
        material->setView(cam.GetViewMatrix());
        material->setViewPost(cam.GetPosition());
    }
    else
    {
        material->setProjection(glm::mat4(1.0f));
        material->setView(glm::mat4(1.0f));
        material->setViewPost(glm::vec3{0,0,-1});
    }
}

Material* MainWindow::currentMaterial()
{
	Material* material = nullptr;
	switch(m_materialType)
	{
	case MaterialType::Lit:
		material = m_sphereLitMaterial.get();
		break;
	case MaterialType::Unlit:
	case MaterialType::Wireframe:
		material = m_sphereMaterial.get();
		break;
	}
	return material;
}

void MainWindow::handleScroll(double yDelta) {
    if(mouseRightDown && camEnable) {
        cam.ProcessMouseScroll(static_cast<float>(yDelta));
    }
}

void MainWindow::handleMouse(double xpos, double ypos) {
	auto fxpos = static_cast<float>(xpos);
	auto fypos = static_cast<float>(ypos);

    if (firstMouse){
        lastX = fxpos;
        lastY = fypos;
        firstMouse = false;
    }

    auto xDelta = fxpos - lastX;
	auto yDelta = fypos - lastY;

    lastX = fxpos;
    lastY = fypos;
    if(mouseRightDown && camEnable) {
        cam.ProcessMouseMovement(xDelta, yDelta);
    }

}
